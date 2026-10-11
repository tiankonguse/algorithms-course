// 首页单独校验：卡片数 == algorithms.json 条数、组名列表与 json 一致、无 JS 错误
// 用法（必须与「起 Chrome」写在同一条命令里）：
//   node verify_home.js            # 期望值直接从 algorithms.json 读
const fs = require('fs');
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());

const json = JSON.parse(fs.readFileSync(SITE.replace(/\/$/, '') + '/assets/data/algorithms.json', 'utf8'));
const expectCards = json.algorithms.length;
const expectGroups = json.groups.map(g => g.name);

const CHECK = `(function(){
  var out = {};
  out.ready = document.readyState;
  out.cards = document.querySelectorAll('.cards .card').length;
  out.groups = Array.prototype.slice.call(document.querySelectorAll('.group-title'))
    .map(function(t){ return t.textContent.replace(/^\\s*\\d+/, '').trim(); });
  out.titles = Array.prototype.slice.call(document.querySelectorAll('.card-head h3')).map(function(h){ return h.textContent.trim(); });
  out.navGroups = document.querySelectorAll('.sidebar .nav-group').length;
  out.bad = [];
  return out;
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
  // Chrome 155 起 flatten 会话失效，改用页面级 WS（详见 cdp_page.js 头部注释）
  const pg = require('./cdp_page.js')(PORT, { events: events, browserSend: rawSend });
  const send = pg.send;
  let fails = 0;

  const tab = await pg.openTab('about:blank');
  const sessionId = tab.sessionId;
  await send('Log.enable', {}, sessionId);
  await send('Runtime.enable', {}, sessionId);
  await send('Page.enable', {}, sessionId);
  const before = events.length;
  await send('Page.navigate', { url: 'file://' + SITE.replace(/\/$/, '') + '/index.html' }, sessionId);
  for (let i = 0; i < 40; i++) {
    await sleep(250);
    const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId);
    if (r.result.value === 'complete') break;
  }
  await sleep(1500);
  const res = await send('Runtime.evaluate', { expression: CHECK, returnByValue: true }, sessionId);
  const v = res.result.value;
  const errs = events.slice(before).filter(m =>
    (m.method === 'Log.entryAdded' && m.params.entry.level === 'error') ||
    m.method === 'Runtime.exceptionThrown');

  const groupsOk = JSON.stringify(v.groups) === JSON.stringify(expectGroups);
  // 首页本身没有侧边栏（shared.js 不给 index 注入 sidebar），navGroups 为 0 属正常；
  // 若将来首页也带导航，则必须与组数一致。
  const navOk = v.navGroups === 0 || v.navGroups === expectGroups.length;
  const ok = v.cards === expectCards && groupsOk && navOk && errs.length === 0;
  if (!ok) fails++;

  console.log('--- index.html', ok ? 'PASS' : 'FAIL');
  console.log('   卡片:', v.cards, '(期望 ' + expectCards + ')  nav 组:', v.navGroups, '(首页无侧栏时为 0)');
  console.log('   组名:', v.groups.join(' / '));
  if (!groupsOk) {
    console.log('   组名不一致，期望:', expectGroups.join(' / '));
    fails++;
  }
  console.log('   末三张卡片:', v.titles.slice(-3).join(' / '));
  if (errs.length) { console.log('   JS错误:', errs.slice(0, 3).map(m => m.method).join(',')); }
  await pg.closeTab(sessionId);
  ws.close();
  process.exit(fails ? 1 : 0);
})();
