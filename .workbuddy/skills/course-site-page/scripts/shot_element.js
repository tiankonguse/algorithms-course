// 截图：node shot.js <页面> <css选择器> <输出png> [索引]
const fs = require('fs');
const PORT = process.env.CDP_PORT || '9333';
const SITE = process.env.SITE_DIR || '/Users/tiankonguse-m3/project/github/AIProject/algorithms-course';
const BASE = SITE.replace(/\/$/, '') + '/algorithms/';

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
const sleep = ms => new Promise(r => setTimeout(r, ms));

(async () => {
  const [page, sel, out, idxArg, clickArg] = process.argv.slice(2);
  const idx = idxArg ? Number(idxArg) : 0;
  const ver = await (await fetch('http://127.0.0.1:' + PORT + '/json/version')).json();
  const ws = new WebSocket(ver.webSocketDebuggerUrl);
  await new Promise((res, rej) => { ws.addEventListener('open', res); ws.addEventListener('error', () => rej(new Error('ws'))); });
  const { send } = await cdp(ws);

  const { targetId } = await send('Target.createTarget', { url: 'about:blank' });
  const { sessionId } = await send('Target.attachToTarget', { targetId, flatten: true });
  await send('Runtime.enable', {}, sessionId);
  await send('Page.enable', {}, sessionId);
  await send('Emulation.setDeviceMetricsOverride', { width: 1440, height: 1100, deviceScaleFactor: 2, mobile: false }, sessionId);
  await send('Page.navigate', { url: 'file://' + BASE + page }, sessionId);
  for (let i = 0; i < 40; i++) {
    await sleep(250);
    const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId);
    if (r.result.value === 'complete') break;
  }
  await sleep(1200);

  if (clickArg) {
    await send('Runtime.evaluate', {
      expression: "var b=document.querySelectorAll('.viz-controls button'); b[" + Number(clickArg) + "].click();"
    }, sessionId);
    await sleep(300);
  }

  const r = await send('Runtime.evaluate', {
    expression: "(function(){var e=document.querySelectorAll('" + sel + "')[" + idx + "]; if(!e) return null; e.scrollIntoView({block:'start'}); var r=e.getBoundingClientRect(); return JSON.stringify({x:r.x,y:r.y,w:r.width,h:r.height,top:window.scrollY});})()",
    returnByValue: true
  }, sessionId);
  if (!r.result.value) { console.error('元素不存在:', sel, idx); process.exit(1); }
  const box = JSON.parse(r.result.value);
  await sleep(300);
  const r2 = await send('Runtime.evaluate', {
    expression: "(function(){var e=document.querySelectorAll('" + sel + "')[" + idx + "]; var r=e.getBoundingClientRect(); return JSON.stringify({x:r.x+window.scrollX,y:r.y+window.scrollY,w:r.width,h:r.height});})()",
    returnByValue: true
  }, sessionId);
  const b2 = JSON.parse(r2.result.value);
  const shot = await send('Page.captureScreenshot', {
    format: 'png',
    captureBeyondViewport: true,
    clip: { x: Math.max(0, b2.x - 8), y: Math.max(0, b2.y - 8), width: Math.min(b2.w + 16, 1440), height: Math.min(b2.h + 16, 4000), scale: 2 }
  }, sessionId);
  fs.writeFileSync(out, Buffer.from(shot.data, 'base64'));
  console.log('saved', out, Math.round(b2.w) + 'x' + Math.round(b2.h));
  await send('Target.closeTarget', { targetId });
  ws.close();
  process.exit(0);
})().catch(e => { console.error('ERR', e.message); process.exit(1); });
