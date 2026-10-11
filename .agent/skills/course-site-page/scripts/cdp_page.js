// Chrome 155 起，浏览器级 WS 上的 Target.attachToTarget({flatten:true}) 返回的会话
// 不再支持 Log/Runtime/Page 等 domain 调用（一律报 "'Xxx.enable' wasn't found"），
// 自检脚本会因此把所有页面判成「全 0」。
// 本模块改成：用 HTTP PUT /json/new 开标签页，再连这个页自己的 webSocketDebuggerUrl，
// 对外仍暴露 send(method, params, sessionId)，改动面只有各脚本里取 target 的那两行。
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
