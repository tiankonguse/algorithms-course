// 演示分支测试：先执行一段设置脚本（如改滑块），再逐帧点下一步
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const FILE = process.argv[2];
const SETUP = process.argv[3] || '';

const FRAME = `(function(){
  var v = document.querySelector('.viz');
  var svg = v.querySelector('svg');
  var bd = v.querySelector('.breakdown');
  var b;
  try { b = svg.getBBox(); } catch(e) { return {err:'getBBox: '+e.message}; }
  var vb = (svg.getAttribute('viewBox')||'').split(/\\s+/).map(Number);
  var bad = '';
  if (b.x < vb[0]-2 || b.y < vb[1]-2 || b.x+b.width > vb[0]+vb[2]+2 || b.y+b.height > vb[1]+vb[3]+2) {
    bad = 'bbox=('+b.x.toFixed(1)+','+b.y.toFixed(1)+','+(b.x+b.width).toFixed(1)+','+(b.y+b.height).toFixed(1)+') vb='+vb.join(',');
  }
  return {kids: svg.childNodes.length, text:(bd?bd.textContent:'').trim().slice(0,70), bad: bad,
          disabled: document.querySelectorAll('.viz-controls button')[1].disabled};
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
  const pg = require('./cdp_page.js')(PORT, { events: events, browserSend: rawSend });
  const send = pg.send;
  let fails = 0;
  const tab = await pg.openTab('about:blank');
  const sessionId = tab.sessionId;
  await send('Log.enable', {}, sessionId);
  await send('Runtime.enable', {}, sessionId);
  await send('Page.enable', {}, sessionId);
  const before = events.length;
  await send('Page.navigate', { url: 'file://' + SITE + '/algorithms/' + FILE }, sessionId);
  for (let i = 0; i < 40; i++) {
    await sleep(250);
    const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId);
    if (r.result.value === 'complete') break;
  }
  await sleep(1200);
  if (SETUP) await send('Runtime.evaluate', { expression: SETUP, returnByValue: true }, sessionId);
  await sleep(300);
  await send('Runtime.evaluate', { expression: "document.querySelector('.viz-controls button:nth-child(3)').click()", returnByValue: true }, sessionId);
  await sleep(200);
  if (SETUP) await send('Runtime.evaluate', { expression: SETUP, returnByValue: true }, sessionId);
  await sleep(200);
  const lines = [];
  let step = 0;
  for (let i = 0; i < 20; i++) {
    const r = await send('Runtime.evaluate', { expression: FRAME, returnByValue: true }, sessionId);
    const v = r.result.value;
    step++;
    lines.push('   #' + step + ' kids=' + v.kids + (v.bad ? ' 越界 ' + v.bad : '') + '  ' + (v.text || '').replace(/\s+/g, ' '));
    if (v.bad) fails++;
    if (v.kids === 0) { lines.push('   !! 空 svg'); fails++; }
    if (v.disabled) break;
    await send('Runtime.evaluate', { expression: "document.querySelector('.viz-controls button:nth-child(2)').click()", returnByValue: true }, sessionId);
    await sleep(120);
  }
  const errs = events.slice(before).filter(m =>
    (m.method === 'Log.entryAdded' && m.params.entry.level === 'error') || m.method === 'Runtime.exceptionThrown');
  if (errs.length) { fails++; lines.push('   JS错误: ' + errs.slice(0, 3).map(m => m.method).join(',')); }
  console.log('---', FILE, SETUP.slice(0, 40), '帧数', step);
  console.log(lines.join('\n'));
  await pg.closeTab(sessionId);
  ws.close();
  process.exit(fails ? 1 : 0);
})();
