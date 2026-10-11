// 逐帧点到底：枚举每个演示的全部 select 组合，核对末帧答案与按钮状态
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const BASE = SITE.replace(/\/$/, '') + '/algorithms/';

function cdp(ws) {
  let id = 0;
  const waiting = new Map();
  const events = [];
  ws.addEventListener('message', ev => {
    const msg = JSON.parse(ev.data);
    if (msg.id && waiting.has(msg.id)) {
      const { resolve, reject } = waiting.get(msg.id);
      waiting.delete(msg.id);
      if (msg.error) reject(new Error(JSON.stringify(msg.error)));
      else resolve(msg.result);
    } else if (msg.method) events.push(msg);
  });
  function send(method, params, sessionId) {
    const mid = ++id;
    const payload = { id: mid, method, params: params || {} };
    if (sessionId) payload.sessionId = sessionId;
    ws.send(JSON.stringify(payload));
    return new Promise((resolve, reject) => {
      waiting.set(mid, { resolve, reject });
      setTimeout(() => { if (waiting.has(mid)) { waiting.delete(mid); reject(new Error('timeout ' + method)); } }, 30000);
    });
  }
  return { send, events };
}
const sleep = ms => new Promise(r => setTimeout(r, ms));

const PROBE = `(async function(){
  var CFG = __CFG__;
  function tick(){ return new Promise(function(r){ setTimeout(r, 0); }); }
  var out = [];
  for (var d = 0; d < CFG.length; d++) {
    var c = CFG[d];
    var sels = c.selects.map(function(id){ return document.getElementById(id); });
    var btn = document.getElementById(c.next), rst = document.getElementById(c.reset), ans = document.getElementById(c.answer);
    if (sels.some(function(s){ return !s; }) || !btn) { out.push({demo:c.name, error:'控件缺失'}); continue; }
    var counts = sels.map(function(s){ return s.options.length; });
    var total = counts.reduce(function(a,b){ return a*b; }, 1);
    for (var k = 0; k < total; k++) {
      var t = k, idxs = [];
      for (var q = sels.length - 1; q >= 0; q--) {
        var idx = t % counts[q]; t = Math.floor(t / counts[q]);
        sels[q].selectedIndex = idx;
        sels[q].dispatchEvent(new Event('change'));
        idxs.unshift(idx);
      }
      await tick();
      if (rst) { rst.click(); await tick(); }
      var clicks = 0;
      while (!btn.disabled && clicks < 400) { btn.click(); clicks++; await tick(); }
      await tick();
      out.push({demo:c.name, k:k, idxs:idxs, clicks:clicks, disabled:!!btn.disabled,
                answer:(ans ? (ans.textContent||'').trim() : '')});
    }
  }
  return JSON.stringify(out);
})()`;

// ---- 期望值（来自 C++ 参考实现 / 暴力） ----
const EXP = {
  cycle: {
    cases: ['aa', 'abcabcab', 'babaababaa', 'ababab', 'aabaabaab'],
    // 正确版：朴素枚举块长
    ok: s => { for (let L = 1; L <= s.length; L++) { let good = true; for (let i = 0; i < s.length; i++) if (s[i] !== s[i % L]) { good = false; break; } if (good) return L; } return s.length; },
    // 两个错法，按页面同款语义独立写一遍
    last: s => { const n = s.length, nx = nextArr(s, n - 1); return n - nx[n - 1]; },
    noback: s => { const n = s.length, nx = new Array(n + 2).fill(0); nx[0] = -1; nx[1] = 0; let i = 2, cn = 0; while (i <= n) { if (s[i - 1] === s[cn]) { cn++; nx[i++] = cn; } else { nx[i++] = 0; } } return n - nx[n]; }
  },
  stack: {
    cases: [['bbaa', 'ba'], ['baaa', 'aa'], ['aabab', 'ab'], ['abcabc', 'abc'], ['aabbaa', 'ab'], ['abccba', 'cb']],
    ok: ['', 'ba', 'a', '', 'aa', 'abca']
  },
  tree: { expect: [true, true, false, true, true, false, true] },
  good: { expect: [27, 0, 600, 27, 26] }
};
function nextArr(s, upto) {
  const n = s.length, nx = new Array(n + 2).fill(0);
  nx[0] = -1; nx[1] = 0;
  let i = 2, cn = 0;
  while (i <= upto) { if (s[i - 1] === s[cn]) { cn++; nx[i++] = cn; } else if (cn > 0) cn = nx[cn]; else nx[i++] = 0; }
  return nx;
}

const CFG_BY_PAGE = {
  cycle: [
    { name: 'cycle', selects: ['cyc-str', 'cyc-ver'], next: 'cyc-next', reset: 'cyc-reset', answer: 'cyc-answer' },
    { name: 'stack', selects: ['stk-case', 'stk-ver'], next: 'stk-next', reset: 'stk-reset', answer: 'stk-answer' }
  ],
  jump: [
    { name: 'tree', selects: ['tree-case'], next: 'tree-next', reset: 'tree-reset', answer: 'tree-answer' },
    { name: 'good', selects: ['good-case', 'good-ver'], next: 'good-next', reset: 'good-reset', answer: 'good-answer' }
  ]
};

(async () => {
  const ver = await (await fetch('http://127.0.0.1:' + PORT + '/json/version')).json();
  const ws = new WebSocket(ver.webSocketDebuggerUrl);
  await new Promise((res, rej) => { ws.addEventListener('open', res); ws.addEventListener('error', () => rej(new Error('ws error'))); });
  const { send: rawSend, events } = cdp(ws);
  const pg = require('./cdp_page.js')(PORT, { events, browserSend: rawSend });
  const send = pg.send;

  const files = process.argv.slice(2);
  let fails = 0, checks = 0, jsErr = 0;
  for (const f of files) {
    const { sessionId } = await pg.openTab('file://' + BASE + f);
    try { await send('Runtime.enable', {}, sessionId); } catch (e) {}
    await sleep(1200);
    try {
      const cfg = f.indexOf('kmp-cycle') >= 0 ? CFG_BY_PAGE.cycle : CFG_BY_PAGE.jump;
      const r = await send('Runtime.evaluate', {
        expression: PROBE.replace('__CFG__', JSON.stringify(cfg)),
        awaitPromise: true, returnByValue: true
      }, sessionId);
      if (r.exceptionDetails) throw new Error(JSON.stringify(r.exceptionDetails).slice(0, 300));
      const rows = JSON.parse(r.result.value);
      for (const row of rows) {
        if (row.error) { console.log('  FAIL', f, row.demo, row.error); fails++; continue; }
        checks++;
        const tag = f + ' / ' + row.demo + ' #' + row.k + ' [' + row.idxs.join(',') + ']';
        if (!row.disabled) { console.log('  FAIL ' + tag + ' 点到底后按钮没 disabled'); fails++; continue; }
        if (row.answer === '') { console.log('  FAIL ' + tag + ' 末帧答案为空'); fails++; continue; }
        if (row.demo === 'cycle') {
          const s0 = EXP.cycle.cases[row.idxs[0]], ver = row.idxs[1];
          const m = row.answer.match(/=\s*(-?\d+)\s*（/);
          if (!m) { console.log('  FAIL ' + tag + ' 答案解析失败: ' + row.answer); fails++; continue; }
          const got = Number(m[1]);
          const want = ver === 0 ? EXP.cycle.ok(s0) : (ver === 1 ? EXP.cycle.last(s0) : EXP.cycle.noback(s0));
          if (got !== want) { console.log('  FAIL ' + tag + ' 期望 ' + want + ' 得到 ' + got); fails++; }
          else if (ver === 1 && s0 === 'aa' && got !== 2) { console.log('  FAIL 锚点 ' + tag + ' C++ 给 2'); fails++; }
          else if (ver === 2 && s0 === 'babaababaa' && got !== 10) { console.log('  FAIL 锚点 ' + tag + ' C++ 给 10'); fails++; }
        } else if (row.demo === 'stack') {
          const m = row.answer.match(/剩下的串 = "([^"]*)"/);
          if (!m) { console.log('  FAIL ' + tag + ' 答案解析失败: ' + row.answer); fails++; continue; }
          if (row.idxs[1] === 0) {
            const want = EXP.stack.ok[row.idxs[0]];
            if (m[1] !== want) { console.log('  FAIL ' + tag + ' 期望 "' + want + '" 得到 "' + m[1] + '"'); fails++; }
          } else {
            const cs = EXP.stack.cases[row.idxs[0]];
            if (cs[0] === 'bbaa' && cs[1] === 'ba' && m[1] !== 'ba') { console.log('  FAIL 锚点 ' + tag + ' C++ 给 "ba"，得到 "' + m[1] + '"'); fails++; }
          }
        } else if (row.demo === 'tree') {
          const want = EXP.tree.expect[row.idxs[0]];
          const got = row.answer.indexOf('不存在') < 0;
          if (got !== want) { console.log('  FAIL ' + tag + ' 期望 ' + want + ' 得到 ' + got + ' (' + row.answer + ')'); fails++; }
        } else if (row.demo === 'good') {
          const m = row.answer.match(/答案\s*(-?\d+)\s*$/);
          if (!m) { console.log('  FAIL ' + tag + ' 答案解析失败: ' + row.answer); fails++; continue; }
          if (row.idxs[1] === 0) {
            const want = EXP.good.expect[row.idxs[0]];
            if (Number(m[1]) !== want) { console.log('  FAIL ' + tag + ' 期望 ' + want + ' 得到 ' + m[1]); fails++; }
          }
        }
      }
    } catch (e) {
      console.log('  FAIL', f, 'CDP', e.message);
      fails++;
    }
    await pg.closeTab(sessionId);
  }
  jsErr = events.filter(e => e.method === 'Runtime.exceptionThrown').length;
  const exAll = events.filter(e => e.method === 'Runtime.exceptionThrown');
  const exs = exAll.slice(0, 2).map(e => {
    const d = e.params.exceptionDetails;
    const fr = (d.stackTrace && d.stackTrace.callFrames || []).slice(0, 3)
      .map(f => f.lineNumber + ':' + f.columnNumber + ' ' + (f.functionName || '(anon)')).join(' <- ');
    return ((d.exception && d.exception.description) || d.text || '').split('\n').slice(0, 2).join(' | ') + '  @@ ' + fr;
  });
  if (exs.length) exs.forEach(x => console.log('  JS异常: ' + String(x).slice(0, 300)));
  console.log('组合数 ' + checks + '，失败 ' + fails + '，JS 异常 ' + jsErr);
  console.log(fails === 0 && jsErr === 0 ? '==== VIZ ALL OK ====' : '==== VIZ FAIL ====');
  process.exit(fails === 0 && jsErr === 0 ? 0 : 1);
})();
