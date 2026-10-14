// 逐帧点开交互演示，检查每一步的 svg 是否越界、内容是否为空
// 用法：node verify_viz_steps.js 页1.html 页2.html   （只传文件名，文件都在 algorithms/ 下）
// 2026-10-14 修两处：
//   ① 参数写成 algorithms/xxx.html 会拼出重复路径，Chrome 开的是自己的报错页，
//      于是「no viz」「空 svg」全冒出来，看着像脚本坏了。现在参数归一化 + 打不开时显式报错。
//   ② 原来只跑页面里第一个 .viz，一页两个演示时第二个从不检查。现在逐个跑。
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const BASE = SITE.replace(/\/$/, '') + '/algorithms/';

// 允许 algorithms/xxx.html、xxx.html、xxx 三种写法，一律归一成裸文件名
function normFile(f) {
  f = String(f).split('/').pop();
  return /\.html?$/.test(f) ? f : f + '.html';
}

const FRAME = (idx) => `(function(){
  var vs = document.querySelectorAll('.viz');
  var v = vs[${idx}];
  if (!v) return {err:'no viz（页面只有 ' + vs.length + ' 个演示，要的是第 ${idx + 1} 个）'};
  var svg = v.querySelector('svg');
  var bd = v.querySelector('.breakdown');
  var btns = Array.prototype.slice.call(v.querySelectorAll('button'));
  var nx = btns.filter(function(b){ return /下一步|下一个|下一条/.test(b.textContent); })[0]
        || v.querySelectorAll('.viz-controls button')[1];
  // 有的演示是纯 HTML+CSS 网格，没有 svg，跳过 bbox 检查而不是判失败；
  // 但「是否到底」仍要看按钮 disabled，不能一律当结束（2026-10-02 修：否则只跑 1 帧）
  if (!svg) return {kids: v.querySelectorAll('*').length, text:'(无 svg，跳过 bbox)', bad:'', disabled: !!nx && !!nx.disabled};
  var b;
  try { b = svg.getBBox(); } catch(e) { return {err:'getBBox: '+e.message}; }
  var vb = (svg.getAttribute('viewBox')||'').split(/\\s+/).map(Number);
  var bad = '';
  if (b.x < vb[0]-2 || b.y < vb[1]-2 || b.x+b.width > vb[0]+vb[2]+2 || b.y+b.height > vb[1]+vb[3]+2) {
    bad = 'bbox=('+b.x.toFixed(1)+','+b.y.toFixed(1)+','+(b.x+b.width).toFixed(1)+','+(b.y+b.height).toFixed(1)+') vb='+vb.join(',');
  }
  var sn = v.querySelector('.stepnote') || document.querySelector('.stepnote');
  var txt = (bd ? bd.textContent : (sn || {}).textContent || '');
  return {kids: svg.childNodes.length, text: txt.trim().slice(0,60), bad: bad,
          disabled: !!nx && !!nx.disabled};
})()`;

// 点「重置」：先按文字找，找不到再退回老页面的 .viz-controls 第 3 个按钮
const CLICK_RESET = (idx) => `(function(){
  var v=document.querySelectorAll('.viz')[${idx}];
  if(!v) return 0;
  var b=Array.prototype.slice.call(v.querySelectorAll('button'))
        .filter(function(x){ return x.textContent.indexOf('重置')>=0; })[0]
      || v.querySelector('.viz-controls button:nth-child(3)');
  if (b) b.click();
  return 1;
})()`;
// 点「下一步」：同理，退回老页面的 .viz-controls 第 2 个按钮
const CLICK_NEXT = (idx) => `(function(){
  var v=document.querySelectorAll('.viz')[${idx}];
  if(!v) return 0;
  var b=Array.prototype.slice.call(v.querySelectorAll('button'))
        .filter(function(x){ return /下一步|下一个|下一条/.test(x.textContent); })[0]
      || v.querySelector('.viz-controls button:nth-child(2)');
  if (b) b.click();
  return 1;
})()`;

async function cdp(ws) {
  let id = 0; const waiting = new Map(); const events = [];
  ws.addEventListener('message', ev => {
    const msg = JSON.parse(ev.data);
    if (msg.id && waiting.has(msg.id)) {
      const { resolve, reject } = waiting.get(msg.id); waiting.delete(msg.id);
      if (msg.error) reject(new Error(JSON.stringify(msg.error))); else resolve(msg.result);
    } else if (msg.method) events.push(msg);
  });
  const send = (method, params, sessionId) => {
    const mid = ++id; const payload = { id: mid, method, params: params || {} };
    if (sessionId) payload.sessionId = sessionId;
    ws.send(JSON.stringify(payload));
    return new Promise((resolve, reject) => {
      waiting.set(mid, { resolve, reject });
      setTimeout(() => { if (waiting.has(mid)) { waiting.delete(mid); reject(new Error('timeout ' + method)); } }, 20000);
    });
  };
  return { send, events };
}
const sleep = ms => new Promise(r => setTimeout(r, ms));

(async () => {
  const ver = await (await fetch('http://127.0.0.1:' + PORT + '/json/version')).json();
  const ws = new WebSocket(ver.webSocketDebuggerUrl);
  await new Promise((res, rej) => { ws.addEventListener('open', res); ws.addEventListener('error', () => rej(new Error('ws'))); });
  const { send: rawSend, events } = await cdp(ws);
  // 开标签走页面级 WS（见 cdp_page.js 头部注释，浏览器级 flatten 那条路也能用）
  const pg = require('./cdp_page.js')(PORT, { events: events, browserSend: rawSend });
  const send = pg.send;
  let fails = 0;

  for (const raw of process.argv.slice(2)) {
    const f = normFile(raw);
    const url = 'file://' + BASE + f;
    const tab = await pg.openTab('about:blank');
    const sessionId = tab.sessionId;
    await send('Log.enable', {}, sessionId);
    await send('Runtime.enable', {}, sessionId);
    await send('Page.enable', {}, sessionId);
    const before = events.length;
    await send('Page.navigate', { url }, sessionId);
    // about:blank 一上来 readyState 就是 complete，只看 complete 会在导航提交前退出，
    // 必须等到 location 真的换掉（chrome-error 也算换掉了，下一步会把它认出来）
    for (let i = 0; i < 40; i++) {
      await sleep(250);
      const r = await send('Runtime.evaluate', { expression: 'document.readyState + " ~~" + location.href', returnByValue: true }, sessionId);
      const s = (r.result.value || '');
      if (s.indexOf('complete ~~') === 0 && s.indexOf('about:blank') < 0) break;
    }
    await sleep(1200);

    // 文件不存在时 Chrome 开的是自己的报错页：正文几十 KB、一个 .viz 都没有、也不抛 JS 错。
    // 不先认出来的话，只会看到「no viz / 空 svg」这种看不出根因的报错。
    const bad = await send('Runtime.evaluate', {
      expression: '/^chrome-error:/.test(location.href) || !!(document.body && /ERR_FILE_NOT_FOUND|无法访问您的文件/.test(document.body.innerText.slice(0,300)))',
      returnByValue: true
    }, sessionId);
    if (bad.result.value) {
      console.log('---', f, 'FAIL');
      console.log('   !! 打不开这个页面（文件不存在）：' + url);
      console.log('      参数只传文件名即可，本脚本会自动拼上 algorithms/。你传的是：' + raw);
      fails++;
      await pg.closeTab(sessionId);
      continue;
    }

    const vizCount = (await send('Runtime.evaluate', { expression: 'document.querySelectorAll(".viz").length', returnByValue: true }, sessionId)).result.value;
    if (!vizCount) { console.log('---', f, 'FAIL  页面里一个演示都没有'); fails++; await pg.closeTab(sessionId); continue; }

    for (let vi = 0; vi < vizCount; vi++) {
      await send('Runtime.evaluate', { expression: CLICK_RESET(vi), returnByValue: true }, sessionId);
      await sleep(200);
      let step = 0; const lines = [];
      for (let i = 0; i < 40; i++) {
        const r = await send('Runtime.evaluate', { expression: FRAME(vi), returnByValue: true }, sessionId);
        const v = r.result.value;
        step++;
        if (v && v.err) { lines.push('   !! ' + v.err); fails++; break; }
        lines.push('   #' + step + ' kids=' + v.kids + (v.bad ? ' 越界 ' + v.bad : '') + '  ' + (v.text || '').replace(/\s+/g, ' '));
        if (v.kids === 0) { lines.push('   !! 空 svg'); fails++; }
        if (v.bad) fails++;
        if (v.disabled) break;
        await send('Runtime.evaluate', { expression: CLICK_NEXT(vi), returnByValue: true }, sessionId);
        await sleep(120);
      }
      console.log('---', f, '演示 #' + (vi + 1) + '/' + vizCount, '帧数', step);
      console.log(lines.join('\n'));
    }
    const errs = events.slice(before).filter(m =>
      (m.method === 'Log.entryAdded' && m.params.entry.level === 'error') ||
      m.method === 'Runtime.exceptionThrown');
    if (errs.length) { fails++; console.log('   JS错误: ' + errs.slice(0, 3).map(m => m.method).join(',')); }
    await pg.closeTab(sessionId);
  }
  ws.close();
  process.exit(fails ? 1 : 0);
})();
