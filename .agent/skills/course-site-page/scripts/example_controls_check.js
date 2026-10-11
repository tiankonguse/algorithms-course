// 004 课双演示分支自检：切算法/数组/形状到最后一帧，核对计数与 svg 越界
const PORT = process.env.CDP_PORT || '9333';
const os = require('os');
const SITE = (process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir());
const BASE = SITE.replace(/\/$/, '') + '/algorithms/';
const FILE = process.argv[2];
const sleep = ms => new Promise(r => setTimeout(r, ms));
async function cdp(ws) {
  let id = 0; const waiting = new Map(); const events = [];
  ws.addEventListener('message', ev => {
    const msg = JSON.parse(ev.data);
    if (msg.id && waiting.has(msg.id)) { const { resolve, reject } = waiting.get(msg.id); waiting.delete(msg.id); if (msg.error) reject(new Error(JSON.stringify(msg.error))); else resolve(msg.result); }
    else if (msg.method) events.push(msg);
  });
  const send = (method, params, sessionId) => {
    const mid = ++id; const payload = { id: mid, method: method, params: params || {} };
    if (sessionId) payload.sessionId = sessionId;
    ws.send(JSON.stringify(payload));
    return new Promise((resolve, reject) => { waiting.set(mid, { resolve, reject }); setTimeout(() => { if (waiting.has(mid)) { waiting.delete(mid); reject(new Error('timeout ' + method)); } }, 20000); });
  };
  return { send, events };
}
(async () => {
  const ver = await (await fetch('http://127.0.0.1:' + PORT + '/json/version')).json();
  const ws = new WebSocket(ver.webSocketDebuggerUrl);
  await new Promise((res, rej) => { ws.addEventListener('open', res); ws.addEventListener('error', () => rej(new Error('ws'))); });
  const { send: rawSend, events } = await cdp(ws);
  const pg = require('./cdp_page.js')(PORT, { events: events, browserSend: rawSend });
  const send = pg.send;
  const tab = await pg.openTab('about:blank');
  const sessionId = tab.sessionId;
  await send('Log.enable', {}, sessionId);
  await send('Runtime.enable', {}, sessionId);
  await send('Page.enable', {}, sessionId);
  const before = events.length;
  await send('Page.navigate', { url: 'file://' + BASE + FILE }, sessionId);
  for (let i = 0; i < 40; i++) { await sleep(250); const r = await send('Runtime.evaluate', { expression: 'document.readyState', returnByValue: true }, sessionId); if (r.result.value === 'complete') break; }
  await sleep(1200);
  const click = expr => send('Runtime.evaluate', { expression: expr, returnByValue: true }, sessionId);
  let fails = 0;

  // ---- 演示一：三种算法 × 四种数组 ----
  for (const algo of ['sel', 'bub', 'ins']) {
    for (const data of ['demo', 'sorted', 'rev', 'rand']) {
      await click(`(function(){var a=document.getElementById('svAlgo'),d=document.getElementById('svData');a.value='${algo}';a.dispatchEvent(new Event('change'));d.value='${data}';d.dispatchEvent(new Event('change'));})()`);
      await sleep(200);
      let bad = '', last = '';
      for (let k = 0; k < 200; k++) {
        const r = await send('Runtime.evaluate', {
          expression: `(function(){
            var st=document.getElementById('svStage').querySelector('svg'), b;
            try{ b=st.getBBox(); }catch(e){ return {err:e.message}; }
            var vb=(st.getAttribute('viewBox')||'').split(/\\s+/).map(Number);
            var bad='';
            if(b.x<vb[0]-2||b.y<vb[1]-2||b.x+b.width>vb[0]+vb[2]+2||b.y+b.height>vb[1]+vb[3]+2)
              bad='bbox=('+b.x.toFixed(1)+','+b.y.toFixed(1)+','+(b.x+b.width).toFixed(1)+','+(b.y+b.height).toFixed(1)+') vb='+vb.join(',');
            return {bad:bad, stat:document.getElementById('svStat').textContent, step:document.getElementById('svStep').textContent, arr:st.textContent.replace(/[^0-9]/g,'').slice(0,20)};
          })()`, returnByValue: true }, sessionId);
        const v = r.result.value;
        if (v.bad) { bad = v.bad; break; }
        last = v.stat.replace(/\s+/g, ' ') + ' | ' + v.step.replace(/\s+/g, ' ').slice(0, 40) + ' | arr=' + v.arr;
        const done = v.step.indexOf('步') >= 0 && v.step.indexOf('结束') >= 0;
        if (done) break;
        await click(`(function(){var bs=document.getElementById('vizStep').querySelectorAll('.viz-controls button');for(var i=0;i<bs.length;i++){if(bs[i].textContent.indexOf('下一步')>=0){bs[i].click();return;}}})()`);
        await sleep(60);
      }
      if (bad) fails++;
      console.log(`   ${algo}/${data} ${bad ? '越界 ' + bad : 'OK'}  ${last}`);
    }
  }

  // ---- 演示二：三种形状 × 若干 n ----
  for (const shape of ['rand', 'sorted', 'rev']) {
    for (const n of [4, 12, 36]) {
      await click(`(function(){var s=document.getElementById('svShape'),r=document.getElementById('svN');s.value='${shape}';s.dispatchEvent(new Event('change'));r.value='${n}';r.dispatchEvent(new Event('input'));})()`);
      await sleep(200);
      const r = await send('Runtime.evaluate', {
        expression: `(function(){
          var v=document.getElementById('vizCost'), st=v.querySelector('svg'), b;
          try{ b=st.getBBox(); }catch(e){ return {err:e.message}; }
          var vb=(st.getAttribute('viewBox')||'').split(/\\s+/).map(Number);
          var bad='';
          if(b.x<vb[0]-2||b.y<vb[1]-2||b.x+b.width>vb[0]+vb[2]+2||b.y+b.height>vb[1]+vb[3]+2)
            bad='bbox=('+b.x.toFixed(1)+','+b.y.toFixed(1)+','+(b.x+b.width).toFixed(1)+','+(b.y+b.height).toFixed(1)+') vb='+vb.join(',');
          return {bad:bad, note:document.getElementById('svChartNote').textContent.replace(/\\s+/g,' '), kids:st.childNodes.length};
        })()`, returnByValue: true }, sessionId);
      const v = r.result.value;
      if (v.bad) fails++;
      console.log(`   chart/${shape}/n=${n} ${v.bad ? '越界 ' + v.bad : 'OK kids=' + v.kids}  ${v.note}`);
    }
  }

  const errs = events.slice(before).filter(m => (m.method === 'Log.entryAdded' && m.params.entry.level === 'error') || m.method === 'Runtime.exceptionThrown');
  if (errs.length) { fails++; console.log('   JS错误', errs.slice(0, 3).map(m => (m.params && m.params.entry ? m.params.entry.text : m.method))); }
  console.log(fails ? '--- FAIL ' + fails : '--- ALL PASS');
  await pg.closeTab(sessionId);
  ws.close();
  process.exit(0);
})();
