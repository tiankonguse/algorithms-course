// 用 CDP 校验页面：JS 错误、figure/svg 数量、nav/toc/viz、svg 越界
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const BASE = SITE.replace(/\/$/, '') + '/algorithms/';

const CHECK = `(function(){
  var out = {};
  out.ready = document.readyState;
  var figs = Array.prototype.slice.call(document.querySelectorAll('figure.fig'));
  out.figs = figs.length;
  out.figSvg = figs.filter(function(f){ return f.querySelector('svg'); }).length;
  out.navLinks = document.querySelectorAll('.sidebar .algo-link').length;
  out.navActive = (document.querySelector('.sidebar .algo-link.active')||{}).textContent || '';
  out.toc = document.querySelectorAll('#toc li').length;
  var vizs = Array.prototype.slice.call(document.querySelectorAll('.viz'));
  out.viz = vizs.length;
  // 演示区允许用 SVG，也允许用 HTML+CSS 网格，所以判空要把标题/说明/图例/按钮剥掉再看
  out.vizEmpty = vizs.filter(function(v){
    var c = v.cloneNode(true);
    ['.viz-title', '.intro', '.legend', '.viz-controls'].forEach(function(sel){
      var e = c.querySelector(sel); if (e && e.parentNode) e.parentNode.removeChild(e);
    });
    var kids = 0;
    c.querySelectorAll('svg').forEach(function(s){ if (s.childNodes.length > 0) kids++; });
    return kids === 0 && c.textContent.replace(/\s+/g, '').length === 0;
  }).length;
  out.codes = document.querySelectorAll('pre.code').length;
  out.pager = document.querySelectorAll('.pager').length;
  // 翻页文本链：插错位置时最先在这里露馅（末页应只有 1 个条目）
  out.pagerText = Array.prototype.slice.call(document.querySelectorAll('.pager-item')).map(function(a){
    var l = a.querySelector('.pager-label'), t = a.querySelector('.pager-title');
    return (l ? l.textContent : '?') + ':' + (t ? t.textContent : '?');
  }).join(' | ');
  out.bad = [];
  var svgs = Array.prototype.slice.call(document.querySelectorAll('svg'));
  // 顶栏/侧栏里由 shared.js 注入的图标 svg 藏在 button/a 里，隐藏时 getBBox 为 0，
  // 会被误判成「空 svg」。只校验正文里的图，图标直接跳过。
  out.svgSkipped = 0;
  svgs = svgs.filter(function(s){
    if (s.closest('button, a')) { out.svgSkipped++; return false; }
    return true;
  });
  svgs.forEach(function(s, i){
    var vb = (s.getAttribute('viewBox')||'').trim().split(/\\s+/).map(Number);
    if (vb.length !== 4) { out.bad.push('svg'+i+' 无 viewBox'); return; }
    var b;
    try { b = s.getBBox(); } catch(e) { out.bad.push('svg'+i+' getBBox 失败: '+e.message); return; }
    var x0 = vb[0], y0 = vb[1], x1 = vb[0]+vb[2], y1 = vb[1]+vb[3];
    var tol = 2;
    if (b.x < x0 - tol || b.y < y0 - tol || b.x + b.width > x1 + tol || b.y + b.height > y1 + tol) {
      out.bad.push('svg'+i+' 越界 bbox=('+b.x.toFixed(1)+','+b.y.toFixed(1)+','+(b.x+b.width).toFixed(1)+','+(b.y+b.height).toFixed(1)+') vb=('+vb.join(',')+')');
    }
    if (b.width === 0 && b.height === 0) out.bad.push('svg'+i+' 空');
  });
  out.svgCount = svgs.length;
  return out;
})()`;

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
    } else if (msg.method) {
      events.push(msg);
    }
  });
  function send(method, params, sessionId) {
    const mid = ++id;
    const payload = { id: mid, method, params: params || {} };
    if (sessionId) payload.sessionId = sessionId;
    ws.send(JSON.stringify(payload));
    return new Promise((resolve, reject) => {
      waiting.set(mid, { resolve, reject });
      setTimeout(() => {
        if (waiting.has(mid)) { waiting.delete(mid); reject(new Error('timeout ' + method)); }
      }, 20000);
    });
  }
  return { send, events };
}

function sleep(ms) { return new Promise(r => setTimeout(r, ms)); }

(async () => {
  const ver = await (await fetch('http://127.0.0.1:' + PORT + '/json/version')).json();
  const ws = new WebSocket(ver.webSocketDebuggerUrl);
  await new Promise((res, rej) => {
    ws.addEventListener('open', res);
    ws.addEventListener('error', e => rej(new Error('ws error')));
  });
  const { send, events } = cdp(ws);

  const files = process.argv.slice(2);
  let fails = 0;
  // 站内页数多时，Chrome 跑几十页后会偶发 createTarget/evaluate 超时。
  // 逐页 try/catch + 重试，别让一页的超时把整轮跑到一半掐死（实测 54 页在第 49 页中断）。
  for (const f of files) {
    try {
      fails += await runOne(f);
    } catch (e) {
      await sleep(1500);
      try {
        fails += await runOne(f);
        console.log('   （重试后通过）');
      } catch (e2) {
        console.log('---', f, 'FAIL (CDP)', e2.message);
        fails++;
      }
    }
  }
  ws.close();
  process.exit(fails ? 1 : 0);

  async function runOne(f) {
    const url = 'file://' + BASE + f;
    let targetId = null;
    try {
      const t = await send('Target.createTarget', { url: 'about:blank' });
      targetId = t.targetId;
      const { sessionId } = await send('Target.attachToTarget', { targetId, flatten: true });
      await send('Log.enable', {}, sessionId);
      await send('Runtime.enable', {}, sessionId);
      await send('Page.enable', {}, sessionId);
      const before = events.length;
      await send('Page.navigate', { url }, sessionId);
      for (let i = 0; i < 40; i++) {
        await sleep(250);
        try {
          const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId);
          if (r.result.value === 'complete') break;
        } catch (e) { /* ignore */ }
      }
      await sleep(1600);
      const res = await send('Runtime.evaluate', { expression: CHECK, returnByValue: true }, sessionId);
      const v = res.result.value;
      const errs = events.slice(before).filter(m =>
        (m.method === 'Log.entryAdded' && m.params.entry.level === 'error') ||
        m.method === 'Runtime.exceptionThrown');
      const errText = errs.map(m => m.method === 'Runtime.exceptionThrown'
        ? (m.params.exceptionDetails.exception && m.params.exceptionDetails.exception.description || m.params.exceptionDetails.text)
        : m.params.entry.text).slice(0, 3);

      const ok = v.figs === v.figSvg && v.figSvg >= 2 && v.navLinks > 0 && v.toc > 0 &&
                 v.viz === v.vizEmpty + v.viz && vizOk(v) && v.bad.length === 0 && errText.length === 0;
      console.log('---', f, ok ? 'PASS' : 'FAIL');
      console.log('   figs/svg:', v.figs + '/' + v.figSvg, ' svg总:', v.svgCount, '(跳过图标 ' + v.svgSkipped + ')',
                  ' nav:', v.navLinks, '(' + v.navActive + ')', ' toc:', v.toc,
                  ' viz:', v.viz, ' 空viz:', v.vizEmpty, ' code:', v.codes, ' pager:', v.pager);
      console.log('   pager链:', v.pagerText || '(无)');
      if (v.bad.length) console.log('   越界:', v.bad.join(' | '));
      if (errText.length) console.log('   JS错误:', errText.join(' | '));
      return ok ? 0 : 1;
    } finally {
      if (targetId) { try { await send('Target.closeTarget', { targetId }); } catch (e) { /* ignore */ } }
    }
  }
})();

function vizOk(v) { return v.vizEmpty === 0; }
