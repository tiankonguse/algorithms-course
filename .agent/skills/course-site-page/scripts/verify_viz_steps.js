// 逐帧点开交互演示，检查每一步的 svg 是否越界、内容是否为空
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const BASE = SITE.replace(/\/$/, '') + '/algorithms/';

const FRAME = `(function(){
  var v = document.querySelector('.viz');
  if (!v) return {err:'no viz'};
  var svg = v.querySelector('svg');
  var bd = v.querySelector('.breakdown');
  var btns = Array.prototype.slice.call(v.querySelectorAll('button'));
  var nx = btns.filter(function(b){ return /下一步|下一个|下一条/.test(b.textContent); })[0]
        || document.querySelectorAll('.viz-controls button')[1];
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
  var txt = (bd ? bd.textContent : (document.querySelector('.stepnote')||{}).textContent || '');
  return {kids: svg.childNodes.length, text: txt.trim().slice(0,60), bad: bad,
          disabled: !!nx && !!nx.disabled};
})()`;

// 点「重置」：先按文字找，找不到再退回老页面的 .viz-controls 第 3 个按钮
const CLICK_RESET = `(function(){
  var v=document.querySelector('.viz');
  var b=Array.prototype.slice.call(v.querySelectorAll('button'))
        .filter(function(x){ return x.textContent.indexOf('重置')>=0; })[0]
      || document.querySelector('.viz-controls button:nth-child(3)');
  if (b) b.click();
  return 1;
})()`;
// 点「下一步」：同理，退回老页面的 .viz-controls 第 2 个按钮
const CLICK_NEXT = `(function(){
  var v=document.querySelector('.viz');
  var b=Array.prototype.slice.call(v.querySelectorAll('button'))
        .filter(function(x){ return /下一步|下一个|下一条/.test(x.textContent); })[0]
      || document.querySelector('.viz-controls button:nth-child(2)');
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
  const { send, events } = await cdp(ws);
  let fails = 0;

  for (const f of process.argv.slice(2)) {
    const { targetId } = await send('Target.createTarget', { url: 'about:blank' });
    const { sessionId } = await send('Target.attachToTarget', { targetId, flatten: true });
    await send('Log.enable', {}, sessionId);
    await send('Runtime.enable', {}, sessionId);
    await send('Page.enable', {}, sessionId);
    const before = events.length;
    await send('Page.navigate', { url: 'file://' + BASE + f }, sessionId);
    for (let i = 0; i < 40; i++) {
      await sleep(250);
      const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId);
      if (r.result.value === 'complete') break;
    }
    await sleep(1200);
    await send('Runtime.evaluate', { expression: CLICK_RESET, returnByValue: true }, sessionId);
    await sleep(200);
    let step = 0; const lines = [];
    for (let i = 0; i < 40; i++) {
      const r = await send('Runtime.evaluate', { expression: FRAME, returnByValue: true }, sessionId);
      const v = r.result.value;
      step++;
      if (v && v.err) { lines.push('   !! ' + v.err); fails++; break; }
      lines.push('   #' + step + ' kids=' + v.kids + (v.bad ? ' 越界 ' + v.bad : '') + '  ' + (v.text || '').replace(/\s+/g, ' '));
      if (v.kids === 0) { lines.push('   !! 空 svg'); fails++; }
      if (v.bad) fails++;
      if (v.disabled) break;
      await send('Runtime.evaluate', { expression: CLICK_NEXT, returnByValue: true }, sessionId);
      await sleep(120);
    }
    const errs = events.slice(before).filter(m =>
      (m.method === 'Log.entryAdded' && m.params.entry.level === 'error') ||
      m.method === 'Runtime.exceptionThrown');
    if (errs.length) { fails++; lines.push('   JS错误: ' + errs.slice(0, 3).map(m => m.method).join(',')); }
    console.log('---', f, '帧数', step);
    console.log(lines.join('\n'));
    await send('Target.closeTarget', { targetId });
  }
  ws.close();
  process.exit(fails ? 1 : 0);
})();
