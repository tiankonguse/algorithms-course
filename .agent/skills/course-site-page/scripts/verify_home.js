// 首页 / 目录页校验（三层目录版，2026-10-12 起）
//   index.html   落地页：只核对统计数字（篇数 / 一级 / 二级 / 高阶）与无 JS 错误
//   catalog.html 目录页：顶栏一行一级分类，正文一次只渲染一个一级分类；
//                逐个点开分类，核对「分类名 == 顶栏标签名」「二级分区数 / 组名 / 卡片数 / 卡片顺序 == json」
// 用法（必须与「起 Chrome」写在同一条命令里）：
//   node verify_home.js            # 期望值直接从 algorithms.json 读
const fs = require('fs');
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const ROOT = SITE.replace(/\/$/, '');

const json = JSON.parse(fs.readFileSync(ROOT + '/assets/data/algorithms.json', 'utf8'));

// 与 shared.js 的 tree 构造保持一致：先按 group 分桶，再按 categories 顺序拼，空分类剔除
const byGroup = {};
json.algorithms.forEach(a => { (byGroup[a.group] = byGroup[a.group] || []).push(a); });
const tree = (json.categories || [])
  .map(name => ({
    name,
    subs: (json.groups || [])
      .filter(g => g.cat === name && byGroup[g.name] && byGroup[g.name].length)
      .map(g => ({ name: g.name, items: byGroup[g.name] }))
  }))
  .filter(c => c.subs.length);
const expectTotal = tree.reduce((n, c) => n + c.subs.reduce((m, s) => m + s.items.length, 0), 0);
const expectSubs = tree.reduce((n, c) => n + c.subs.length, 0);
const expectHigh = json.algorithms.filter(a => (a.level || 1) >= 3).length;

// 组名 / 分类名都靠「克隆节点后删掉编号与篇数两个子 span」取，别用正则剥数字
// （组名里有 2-SAT、K-D 树这类，^\d+ 会误伤）
const PICK_TEXT = `function(sel, drop){
  var c = sel.cloneNode(true);
  drop.forEach(function(s){ var d = c.querySelector(s); if (d) d.remove(); });
  return c.textContent.trim();
}`;

const SNAP = `(function(){
  var out = {};
  var pick = ${PICK_TEXT};
  out.ready = document.readyState;
  out.catTabs = Array.prototype.slice.call(document.querySelectorAll('#catNav .cat-tab'))
    .map(function(t){ return pick(t, ['.cat-count']); });
  out.title = (document.getElementById('catalogTitle') || {}).textContent;
  out.groups = Array.prototype.slice.call(document.querySelectorAll('#index-list .group-title'))
    .map(function(t){ return pick(t, ['.group-idx', '.group-count']); });
  out.cards = Array.prototype.slice.call(document.querySelectorAll('#index-list .cards .card'))
    .map(function(a){ return (a.getAttribute('href') || '').replace(/^.*?algorithms\\//, 'algorithms/'); });
  out.stats = {
    total: (document.getElementById('statTotal') || {}).textContent,
    count: (document.getElementById('statCount') || {}).textContent,
    l1: (document.getElementById('statL1') || {}).textContent,
    groups: (document.getElementById('statGroups') || {}).textContent,
    l2: (document.getElementById('statL2') || {}).textContent,
    groupCount: (document.getElementById('statGroupCount') || {}).textContent,
    high: (document.getElementById('statHighCount') || {}).textContent
  };
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
  const { send, events } = await cdp(ws);
  let fails = 0;

  async function open(page) {
    const { targetId } = await send('Target.createTarget', { url: 'about:blank' });
    const { sessionId } = await send('Target.attachToTarget', { targetId, flatten: true });
    await send('Log.enable', {}, sessionId);
    await send('Runtime.enable', {}, sessionId);
    await send('Page.enable', {}, sessionId);
    const before = events.length;
    await send('Page.navigate', { url: 'file://' + ROOT + '/' + page }, sessionId);
    for (let i = 0; i < 40; i++) {
      await sleep(250);
      const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId);
      if (r.result.value === 'complete') break;
    }
    await sleep(1200);
    const ev = async expr => (await send('Runtime.evaluate', { expression: expr, returnByValue: true }, sessionId)).result.value;
    return { sessionId, targetId, ev, errors: () => events.slice(before).filter(m =>
      (m.method === 'Log.entryAdded' && m.params.entry.level === 'error') || m.method === 'Runtime.exceptionThrown') };
  }

  // ===== catalog.html：顶栏 + 逐分类核对 =====
  {
    const p = await open('catalog.html');
    const tabs = (await p.ev(SNAP)).catTabs;
    let bad = 0;
    const tabsOk = JSON.stringify(tabs) === JSON.stringify(tree.map(c => c.name));
    if (!tabsOk) {
      bad++;
      console.log('   !! 顶栏分类不符，页面:', tabs.join(' / '));
      console.log('      期望:', tree.map(c => c.name).join(' / '));
    }
    console.log('   顶栏', tabs.length, '个一级分类（期望 ' + tree.length + '）');
    let sumCards = 0, sumGroups = 0;
    for (let i = 0; i < tree.length; i++) {
      await p.ev(`document.querySelectorAll('#catNav .cat-tab')[${i}].click()`);
      await sleep(160);
      const s = await p.ev(SNAP);
      const expectFiles = tree[i].subs.reduce((acc, sub) => acc.concat(sub.items.map(a => a.file)), []);
      const nameOk = s.title === tree[i].name;
      const groupsOk = JSON.stringify(s.groups) === JSON.stringify(tree[i].subs.map(sub => sub.name));
      const cardsOk = JSON.stringify(s.cards) === JSON.stringify(expectFiles);
      sumCards += s.cards.length; sumGroups += s.groups.length;
      const ok = nameOk && groupsOk && cardsOk;
      if (!ok) bad++;
      console.log('   [' + (ok ? 'OK' : 'FAIL') + '] ' + tree[i].name + '  标题' + (nameOk ? '' : '✗') +
        '  分区 ' + s.groups.length + '/' + tree[i].subs.length + (groupsOk ? '' : ' ✗') +
        '  卡片 ' + s.cards.length + '/' + expectFiles.length + (cardsOk ? '' : ' ✗'));
    }
    const sumOk = sumCards === expectTotal && sumGroups === expectSubs;
    if (!sumOk) bad++;
    console.log('   合计 ' + sumCards + ' 篇 / ' + sumGroups + ' 个二级分类（期望 ' + expectTotal + ' / ' + expectSubs + '）' + (sumOk ? '' : ' ✗'));
    const errs = p.errors();
    if (errs.length) { bad++; console.log('   JS错误:', errs.slice(0, 3).map(m => m.method).join(',')); }
    console.log('--- catalog.html', bad ? 'FAIL' : 'PASS');
    if (bad) fails++;
    await send('Target.closeTarget', { targetId: p.targetId });
  }

  // ===== index.html：落地页统计数字 =====
  {
    const p = await open('index.html');
    const s = (await p.ev(SNAP)).stats;
    const num = x => String(x).replace(/[^\d]/g, '');
    const checks = [
      ['总篇数', num(s.total), String(expectTotal)],
      ['算法页面数', num(s.count), String(expectTotal)],
      ['一级分类', num(s.l1), String(tree.length)],
      ['一级分类(徽标)', num(s.groups), String(tree.length)],
      ['二级分类', num(s.l2), String(expectSubs)],
      ['二级分类(徽标)', num(s.groupCount), String(expectSubs)],
      ['高阶及以上', num(s.high), String(expectHigh)]
    ];
    let bad = 0;
    checks.forEach(([n, got, want]) => { if (got !== want) { bad++; console.log('   !! ' + n + ' ' + got + ' != ' + want); } });
    const errs = p.errors();
    if (errs.length) { bad++; console.log('   JS错误:', errs.slice(0, 3).map(m => m.method).join(',')); }
    console.log('--- index.html', bad ? 'FAIL' : 'PASS');
    console.log('   统计: ' + expectTotal + ' 篇 / ' + tree.length + ' 个一级 / ' + expectSubs + ' 个二级 / 高阶 ' + expectHigh + ' 篇');
    if (bad) fails++;
    await send('Target.closeTarget', { targetId: p.targetId });
  }

  ws.close();
  console.log(fails ? '\n=== 首页校验 FAIL ===' : '\n=== 首页校验 全通过 ===');
  process.exit(fails ? 1 : 0);
})();
