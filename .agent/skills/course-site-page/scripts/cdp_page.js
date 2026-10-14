// 开页面标签的两种方式，本模块选的是后者（页面级 WS）：
//   ① 浏览器级 WS：Target.createTarget + Target.attachToTarget({flatten:true})
//   ② 页面级 WS  ：HTTP PUT /json/new?<url>，再连这个页自己的 webSocketDebuggerUrl
// 2026-10-14 实测 Chrome 155.0.8059.39：两条路都通，① 并没有失效。
// 之前把①判成「失效」，根因是自己的 send 包装漏传了 sessionId——浏览器级 WS 上
// 不带 sessionId 发 Log.enable 就会报 "'Log.enable' wasn't found"，看着像 Chrome 不支持。
// 遇到这个报错先查：sessionId 传了没、WS 连的是不是 /json/version 给的浏览器端点。
// 本模块仍然有用：它不依赖浏览器级 WS，且 events 直接灌进调用方的数组。
// 用法：const { send, openTab, closeTab } = require('./cdp_page.js')(PORT, { events, browserSend });
// events 是脚本原本用于筛 JS 错误的数组（页面级事件会继续灌进去），
// browserSend 是原浏览器级 WS 的 send，用于 Target.* 之类命令兜底。
function sleep(ms) { return new Promise(r => setTimeout(r, ms)); }

module.exports = function (PORT, opts) {
  opts = opts || {};
  const events = opts.events || [];
  const browserSend = opts.browserSend || (() => Promise.reject(new Error('无浏览器级 send')));
  const tabs = new Map();

  async function openTab(url) {
    const t = await (await fetch('http://127.0.0.1:' + PORT + '/json/new?' + encodeURIComponent(url), { method: 'PUT' })).json();
    await sleep(120);
    const ws = new WebSocket(t.webSocketDebuggerUrl);
    await new Promise((res, rej) => {
      ws.addEventListener('open', res);
      ws.addEventListener('error', () => rej(new Error('ws 连接失败')));
    });
    let id = 0;
    const waiting = new Map();
    ws.addEventListener('message', ev => {
      const msg = JSON.parse(ev.data);
      if (msg.id && waiting.has(msg.id)) {
        const { resolve, reject } = waiting.get(msg.id);
        waiting.delete(msg.id);
        if (msg.error) { reject(new Error(JSON.stringify(msg.error))); return; }
        // 各脚本里的 cdp() 都是 resolve(msg.result)，这里保持一致：
        // evaluate 拿到 {result:{type,value}}、captureScreenshot 拿到 {data}
        resolve(msg.result);
      } else if (msg.method) {
        events.push(msg);
      }
    });
    const send = (method, params) => {
      const mid = ++id;
      ws.send(JSON.stringify({ id: mid, method, params: params || {} }));
      return new Promise((resolve, reject) => {
        waiting.set(mid, { resolve, reject });
        setTimeout(() => {
          if (waiting.has(mid)) { waiting.delete(mid); reject(new Error('timeout ' + method)); }
        }, 20000);
      });
    };
    const sid = 'tab-' + t.id;
    tabs.set(sid, { ws: ws, id: t.id, send: send });
    return { sessionId: sid, targetId: t.id };
  }

  async function closeTab(sid) {
    const c = tabs.get(sid);
    if (!c) return;
    tabs.delete(sid);
    try { c.ws.close(); } catch (e) { /* ignore */ }
    try { await fetch('http://127.0.0.1:' + PORT + '/json/close/' + c.id); } catch (e) { /* ignore */ }
  }

  function send(method, params, sessionId) {
    if (sessionId) {
      const c = tabs.get(sessionId);
      if (c) return c.send(method, params);
      if (String(sessionId).indexOf('tab-') === 0) return Promise.reject(new Error('未知页面会话 ' + sessionId));
    }
    return browserSend(method, params, sessionId);
  }

  return { send: send, openTab: openTab, closeTab: closeTab };
};
