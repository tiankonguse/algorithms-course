// 首页单独校验（三层目录版）：顶栏一级分类一行且与非空分类一致，
// 切换每个一级分类后，正文分区数 / 卡片数与 json 对得上，无 JS 错误
// 用法（必须与「起 Chrome」写在同一条命令里）：
//   node verify_home.js            # 期望值直接从 algorithms.json 读
const fs = require('fs');
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());

const json = JSON.parse(fs.readFileSync(SITE.replace(/\/$/, '') + '/assets/data/algorithms.json', 'utf8'));
const byGroup = {};
json.algorithms.forEach(a => { (byGroup[a.group] = byGroup[a.group] || []).push(a); });
const TREE = (json.categories || [])
  .map(name => ({
    name,
    subs: json.groups.filter(g => g.cat === name && byGroup[g.name] && byGroup[g.name].length)
      .map(g => ({ name: g.name, items: byGroup[g.name] }))
  }))
  .filter(c => c.subs.length);
// 与 shared.js 的默认选中一致：优先「算法基础」，否则第一个非空分类
const DEFAULT_CAT = (TREE.find(c => c.name === '算法基础') || TREE[0]).name;

const CHECK = `(function(){
  var out = {};
  out.ready = document.readyState;
  out.title = document.getElementById('catalogTitle').textContent;
  out.tabs = Array.prototype.slice.call(document.querySelectorAll('.cat-tab')).map(function(t){ return t.textContent.replace(/\\s*\\d+\\s*$/, '').trim(); });
  var tops = Array.prototype.slice.call(document.querySelectorAll('.cat-tab')).map(function(t){ return Math.round(t.getBoundingClientRect().top); });
  out.oneRow = new Set(tops).size === 1;
  out.cards = document.querySelectorAll('.cards .card').length;
  out.groups = Array.prototype.slice.call(document.querySelectorAll('.group-title'))
    .map(function(t){ return t.textContent.replace(/^\\s*\\d+/, '').replace(/\\s*\\d+ 篇\\s*$/, '').trim(); });
  out.navGroups = document.querySelectorAll('#homeNav .nav-group').length;
  out.collapsedAll = Array.prototype.slice.call(document.querySelectorAll('#homeNav .nav-group')).every(function(g){ return g.classList.contains('collapsed'); });
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

  // 顶栏与非空一级分类一致，且恰好一行
  const wantTabs = TREE.map(c => c.name);
  if (JSON.stringify(v.tabs) !== JSON.stringify(wantTabs)) {
    console.log('   顶栏分类不一致，期望:', wantTabs.join(' / '), '实际:', v.tabs.join(' / '));
    fails++;
  }
  if (!v.oneRow) { console.log('   顶栏没有排成一行'); fails++; }

  // 默认分类的正文
  const def = TREE.find(c => c.name === v.title);
  if (!def) { console.log('   默认分类异常:', v.title); fails++; }
  else {
    const wantGroups = def.subs.map(s => s.name);
    const wantCards = def.subs.reduce((n, s) => n + s.items.length, 0);
    if (JSON.stringify(v.groups) !== JSON.stringify(wantGroups)) {
      console.log('   分区名不一致，期望:', wantGroups.join(' / '), '实际:', v.groups.join(' / '));
      fails++;
    }
    if (v.cards !== wantCards) { console.log('   卡片数', v.cards, '期望', wantCards); fails++; }
    if (v.navGroups !== def.subs.length) { console.log('   左侧二级数', v.navGroups, '期望', def.subs.length); fails++; }
    if (!v.collapsedAll) { console.log('   左侧第三层默认应全部折叠'); fails++; }
  }

  // 逐个一级分类点一遍，核对分区与卡片数
  for (let i = 0; i < TREE.length; i++) {
    const c = TREE[i];
    await send('Runtime.evaluate', { expression: `document.querySelectorAll('.cat-tab')[${i}].click()`, returnByValue: true }, sessionId);
    await sleep(300);
    const r = await send('Runtime.evaluate', {
      returnByValue: true,
      expression: `({ title: document.getElementById('catalogTitle').textContent,
                      groups: document.querySelectorAll('#index-list .group').length,
                      cards: document.querySelectorAll('#index-list .card').length })`
    }, sessionId);
    const got = r.result.value;
    const wantGroups = c.subs.length;
    const wantCards = c.subs.reduce((n, s) => n + s.items.length, 0);
    const catOk = got.title === c.name && got.groups === wantGroups && got.cards === wantCards;
    if (!catOk) {
      console.log('   分类「' + c.name + '」不符，实际', JSON.stringify(got), '期望', c.name, wantGroups, wantCards);
      fails++;
    }
  }

  const ok = fails === 0 && errs.length === 0;
  console.log('--- index.html', ok ? 'PASS' : 'FAIL');
  console.log('   顶栏:', v.tabs.join(' / '), v.oneRow ? '（一行）' : '（未排成一行！）');
  console.log('   默认分类:', v.title, '· 分区', v.groups.length, '· 卡片', v.cards);
  if (errs.length) { console.log('   JS错误:', errs.slice(0, 3).map(m => m.method).join(',')); fails++; }
  await pg.closeTab(sessionId);
  ws.close();
  process.exit(fails ? 1 : 0);
})();
