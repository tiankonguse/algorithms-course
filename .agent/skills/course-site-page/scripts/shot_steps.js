// 用法: node shot_steps.js <页面> <viz索引> <点击次数> <输出png>
const fs = require('fs');
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const BASE = SITE.replace(/\/$/, '') + '/algorithms/';
const sleep = ms => new Promise(r => setTimeout(r, ms));
async function cdp(ws) {
  let id = 0; const waiting = new Map();
  ws.addEventListener('message', ev => {
    const msg = JSON.parse(ev.data);
    if (msg.id && waiting.has(msg.id)) {
      const { resolve, reject } = waiting.get(msg.id); waiting.delete(msg.id);
      if (msg.error) reject(new Error(JSON.stringify(msg.error))); else resolve(msg.result);
    }
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
  return { send };
}
(async () => {
  const [page, vizIdx, clicks, out] = process.argv.slice(2);
  const ver = await (await fetch('http://127.0.0.1:' + PORT + '/json/version')).json();
  const ws = new WebSocket(ver.webSocketDebuggerUrl);
  await new Promise((res, rej) => { ws.addEventListener('open', res); ws.addEventListener('error', () => rej(new Error('ws'))); });
  const { send: rawSend, events } = await cdp(ws);
  const pg = require('./cdp_page.js')(PORT, { events: events, browserSend: rawSend });
  const send = pg.send;
  const tab = await pg.openTab('about:blank');
  const sessionId = tab.sessionId;
  await send('Runtime.enable', {}, sessionId);
  await send('Page.enable', {}, sessionId);
  await send('Emulation.setDeviceMetricsOverride', { width: 1440, height: 1400, deviceScaleFactor: 2, mobile: false }, sessionId);
  await send('Page.navigate', { url: 'file://' + BASE + page }, sessionId);
  for (let i = 0; i < 40; i++) {
    await sleep(250);
    const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId);
    if (r.result.value === 'complete') break;
  }
  await sleep(1200);
  const n = Number(clicks);
  for (let k = 0; k < n; k++) {
    await send('Runtime.evaluate', {
      expression: "(function(){var v=document.querySelectorAll('.viz')[" + vizIdx + "];var bs=v.querySelectorAll('button');for(var i=0;i<bs.length;i++){if(bs[i].textContent.indexOf('下一步')>=0){bs[i].click();return 'ok';}}return 'none';})()",
      returnByValue: true
    }, sessionId);
    await sleep(150);
  }
  const box = await send('Runtime.evaluate', {
    expression: "(function(){var e=document.querySelectorAll('.viz')[" + vizIdx + "];if(!e)return null;e.scrollIntoView({block:'center'});var r=e.getBoundingClientRect();return JSON.stringify({x:r.x,y:r.y,w:r.width,h:r.height});})()",
    returnByValue: true
  }, sessionId);
  const b = JSON.parse(box.result.value);
  const shot = await send('Page.captureScreenshot', { format: 'png', clip: { x: b.x, y: b.y, width: b.w, height: b.h, scale: 1 } , captureBeyondViewport: true}, sessionId);
  fs.writeFileSync(out, Buffer.from(shot.data, 'base64'));
  console.log('saved', out, Math.round(b.w) + 'x' + Math.round(b.h));
  await pg.closeTab(sessionId);
  process.exit(0);
})();
