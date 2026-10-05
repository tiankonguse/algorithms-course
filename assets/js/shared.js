/* shared.js —— 算法教程公共逻辑
 * - 加载 assets/data/algorithms.json
 * - 渲染侧边栏算法导航（按 json 里的 groups 分组，搜索过滤，自动高亮当前页）
 * - 渲染首页分组卡片列表（组内顺序即推荐学习顺序，支持搜索过滤）
 * - 自动生成右侧目录 + 滚动高亮当前小节
 * - 窄屏抽屉导航 / 抽屉目录、上下篇翻页、代码复制、阅读进度、回到顶部
 */

(function(){
  // ===== 0. 站点根路径（由本脚本自身的位置推导，首页和算法页深度不同也能对上） =====
  const selfPath = new URL(document.currentScript.src, location.href).pathname;
  const ROOT = selfPath.slice(0, selfPath.length - 'assets/js/shared.js'.length);
  const body = document.body;
  const main = document.querySelector('.main');
  const sidebar = document.getElementById('sidebar');
  const REDUCE = window.matchMedia('(prefers-reduced-motion: reduce)').matches;
  const scrollBehavior = REDUCE ? 'auto' : 'smooth';

  const ICON = {
    menu:'<svg viewBox="0 0 20 20" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round"><path d="M3 5h14M3 10h14M3 15h14"/></svg>',
    list:'<svg viewBox="0 0 20 20" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round"><path d="M4 5h12M4 10h12M4 15h8"/></svg>',
    search:'<svg viewBox="0 0 16 16" fill="none" stroke="currentColor" stroke-width="1.6"><circle cx="7" cy="7" r="4.2"/><path d="M10.2 10.2 14 14" stroke-linecap="round"/></svg>',
    top:'<svg viewBox="0 0 20 20" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round" stroke-linejoin="round"><path d="M10 15V5M5 10l5-5 5 5"/></svg>'
  };

  // ===== 1. 目录（不依赖 JSON，立刻生成） =====
  const tocHeadings = [];
  const tocLinks = [];
  (function buildToc(){
    const ul = document.getElementById('toc');
    if (!ul) return;
    const section = document.querySelector('.algo-section');
    const headings = section ? Array.from(section.querySelectorAll('h2')) : [];
    headings.forEach(function(h, i){
      if (!h.id) h.id = 'sec-' + (i + 1);
      const li = document.createElement('li');
      const a = document.createElement('a');
      a.className = 'toc-item';
      a.href = '#' + h.id;
      a.textContent = h.textContent;
      li.appendChild(a);
      ul.appendChild(li);
      tocLinks.push(a);
      tocHeadings.push(h);
    });

    function updateToc(){
      if (!tocHeadings.length) return;
      let current = tocHeadings[0].id;
      for (const h of tocHeadings) {
        if (h.getBoundingClientRect().top <= 96) current = h.id;
      }
      if (window.innerHeight + window.scrollY >= document.documentElement.scrollHeight - 4) {
        current = tocHeadings[tocHeadings.length - 1].id;
      }
      tocLinks.forEach(a => a.classList.toggle('active', a.getAttribute('href') === '#' + current));
    }
    window.addEventListener('scroll', updateToc, { passive:true });
    window.addEventListener('resize', updateToc);
    updateToc();
  })();

  // ===== 2. 搜索 =====
  function buildSearch(inputId, label, placeholder){
    const box = document.createElement('div');
    box.className = 'search-box';
    box.innerHTML =
      '<span class="search-icon">' + ICON.search + '</span>' +
      '<input class="search-input" type="search" id="' + inputId + '" autocomplete="off" spellcheck="false"' +
      ' aria-label="' + label + '" placeholder="' + placeholder + '">' +
      '<button class="search-clear" type="button" aria-label="清空搜索">&times;</button>';
    return box;
  }

  // groups: [{ el, items:[{ el, text }] }]，text 为预先小写化的匹配文本
  // extra: 可选的额外过滤函数（首页的难度筛选），返回 false 的条目一律隐藏
  const searches = [];
  function registerSearch(box, groups, emptyEl, extra){
    const input = box.querySelector('.search-input');
    const clear = box.querySelector('.search-clear');
    function apply(){
      const keys = input.value.trim().toLowerCase().split(/\s+/).filter(Boolean);
      let hit = 0;
      groups.forEach(function(g){
        let shown = 0;
        g.items.forEach(function(it){
          const ok = (!keys.length || keys.every(k => it.text.indexOf(k) >= 0)) && (!extra || extra(it));
          it.el.hidden = !ok;
          if (ok) shown++;
        });
        g.el.hidden = shown === 0;
        hit += shown;
      });
      if (emptyEl) emptyEl.hidden = hit !== 0;
      box.classList.toggle('has-value', input.value !== '');
    }
    input.addEventListener('input', apply);
    clear.addEventListener('click', function(){ input.value = ''; apply(); input.focus(); });
    searches.push({ input:input, apply:apply });
    return apply;
  }

  // ===== 3. 阅读进度 + 回到顶部 =====
  const progress = document.createElement('div');
  progress.className = 'progress';
  progress.innerHTML = '<span></span>';
  body.appendChild(progress);

  const toTop = document.createElement('button');
  toTop.className = 'to-top';
  toTop.type = 'button';
  toTop.setAttribute('aria-label', '回到顶部');
  toTop.innerHTML = ICON.top;
  body.appendChild(toTop);
  toTop.addEventListener('click', function(){ window.scrollTo({ top:0, behavior:scrollBehavior }); });

  const bar = progress.firstElementChild;
  let raf = 0;
  function updateScrollUi(){
    raf = 0;
    const max = document.documentElement.scrollHeight - window.innerHeight;
    const y = window.scrollY || window.pageYOffset;
    bar.style.width = (max > 0 ? Math.min(100, (y / max) * 100) : 0) + '%';
    toTop.classList.toggle('show', y > 400);
  }
  window.addEventListener('scroll', function(){ if (!raf) raf = requestAnimationFrame(updateScrollUi); }, { passive:true });
  window.addEventListener('resize', function(){ if (!raf) raf = requestAnimationFrame(updateScrollUi); });
  updateScrollUi();

  // ===== 4. 代码块复制 =====
  Array.prototype.forEach.call(document.querySelectorAll('pre.code'), function(pre){
    const wrap = document.createElement('div');
    wrap.className = 'code-wrap';
    pre.parentNode.insertBefore(wrap, pre);
    wrap.appendChild(pre);

    const btn = document.createElement('button');
    btn.className = 'copy-btn';
    btn.type = 'button';
    btn.textContent = '复制';
    wrap.appendChild(btn);

    function fallback(text, done){
      const ta = document.createElement('textarea');
      ta.value = text;
      ta.setAttribute('readonly', '');
      ta.style.cssText = 'position:fixed;top:0;left:0;opacity:0;';
      body.appendChild(ta);
      ta.select();
      try { document.execCommand('copy'); done(); } catch (e) { /* 浏览器不支持就什么都不做 */ }
      body.removeChild(ta);
    }

    btn.addEventListener('click', function(){
      const text = pre.innerText.replace(/\s+$/, '');
      const done = function(){
        btn.textContent = '已复制';
        btn.classList.add('done');
        setTimeout(function(){ btn.textContent = '复制'; btn.classList.remove('done'); }, 1600);
      };
      if (navigator.clipboard && navigator.clipboard.writeText) {
        navigator.clipboard.writeText(text).then(done, function(){ fallback(text, done); });
      } else {
        fallback(text, done);
      }
    });
  });

  // ===== 5. 窄屏抽屉：顶栏 + 遮罩 =====
  let navBtn = null, tocBtn = null, tocPanel = null;
  function openState(cls){ return body.classList.contains(cls); }
  function setDrawer(cls, panel, btn, open){
    body.classList.toggle(cls, open);
    btn.setAttribute('aria-expanded', String(open));
    if (open) panel.setAttribute('tabindex', '-1'), panel.focus({ preventScroll:true });
    else if (document.activeElement === panel) btn.focus({ preventScroll:true });
  }
  function closeDrawers(){
    if (navBtn && sidebar && openState('drawer-open')) setDrawer('drawer-open', sidebar, navBtn, false);
    if (tocBtn && tocPanel && openState('toc-open')) setDrawer('toc-open', tocPanel, tocBtn, false);
  }

  if (document.querySelector('.layout') && sidebar) {
    const topbar = document.createElement('header');
    topbar.className = 'topbar';
    topbar.innerHTML =
      '<button class="icon-btn" type="button" id="navToggle" aria-label="打开算法导航" aria-expanded="false" aria-controls="sidebar">' + ICON.menu + '</button>' +
      '<span class="topbar-title">算法教程</span>' +
      '<button class="icon-btn" type="button" id="tocToggle" aria-label="打开目录" aria-expanded="false" aria-controls="toc-panel">' + ICON.list + '</button>';
    body.insertBefore(topbar, body.firstChild);

    navBtn = topbar.querySelector('#navToggle');
    tocBtn = topbar.querySelector('#tocToggle');
    tocPanel = document.querySelector('.toc-panel');
    if (tocPanel) tocPanel.id = 'toc-panel';

    const scrim = document.createElement('div');
    scrim.className = 'scrim';
    body.appendChild(scrim);
    scrim.addEventListener('click', closeDrawers);

    navBtn.addEventListener('click', function(){
      if (tocPanel) setDrawer('toc-open', tocPanel, tocBtn, false);
      setDrawer('drawer-open', sidebar, navBtn, !openState('drawer-open'));
    });
    if (tocPanel) tocBtn.addEventListener('click', function(){
      setDrawer('drawer-open', sidebar, navBtn, false);
      setDrawer('toc-open', tocPanel, tocBtn, !openState('toc-open'));
    });
    // 点导航链接后关掉抽屉
    sidebar.addEventListener('click', function(e){
      if (e.target.closest('a')) setDrawer('drawer-open', sidebar, navBtn, false);
    });
    if (tocPanel) tocPanel.addEventListener('click', function(e){
      if (e.target.closest('a')) setDrawer('toc-open', tocPanel, tocBtn, false);
    });
    // 视口变宽后抽屉失效，顺手复位状态
    window.addEventListener('resize', function(){
      if (window.innerWidth > 1180 && tocPanel && openState('toc-open')) setDrawer('toc-open', tocPanel, tocBtn, false);
      if (window.innerWidth > 900 && openState('drawer-open')) setDrawer('drawer-open', sidebar, navBtn, false);
    });
  }

  // ===== 6. 上下篇（JSON 到位后填） =====
  let prevHref = '', nextHref = '';

  // ===== 7. 键盘：/ 与 Ctrl/Cmd+K 聚焦搜索，Esc 关闭，← → 翻篇 =====
  function focusSearch(){
    if (!searches.length) return;
    const s = searches[0];
    const drawerMode = navBtn && window.getComputedStyle(navBtn).display !== 'none';
    if (drawerMode && !openState('drawer-open')) setDrawer('drawer-open', sidebar, navBtn, true);
    const focusIt = function(){ s.input.focus(); s.input.select(); };
    if (drawerMode) setTimeout(focusIt, REDUCE ? 0 : 220);
    else focusIt();
  }
  document.addEventListener('keydown', function(e){
    const t = e.target;
    const typing = !!t && (t.tagName === 'INPUT' || t.tagName === 'TEXTAREA' || t.isContentEditable);
    if (e.key === '/' && !typing) { e.preventDefault(); focusSearch(); return; }
    if ((e.metaKey || e.ctrlKey) && e.key.toLowerCase() === 'k') { e.preventDefault(); focusSearch(); return; }
    if (e.key === 'Escape') {
      if (typing && t.value) {
        t.value = '';
        searches.forEach(function(s){ if (s.input === t) s.apply(); });
      } else if (typing) {
        t.blur();
      } else {
        closeDrawers();
      }
      return;
    }
    if (typing || e.metaKey || e.ctrlKey || e.altKey || e.shiftKey) return;
    if (e.key === 'ArrowLeft' && prevHref) location.href = prevHref;
    if (e.key === 'ArrowRight' && nextHref) location.href = nextHref;
  });

  // ===== 8. 异步加载算法索引 =====
  fetch(ROOT + 'assets/data/algorithms.json')
    .then(r => r.json())
    .then(data => {
      const algos = data.algorithms || [];
      const current = (location.pathname.split('/').pop() || 'index.html').toLowerCase();
      const LV = { 1:'入门', 2:'进阶', 3:'高阶', 4:'挑战', 5:'硬核' };
      // 首页三条学习路线：写死专题名，篇数与篇目从索引里算，避免数据变了文案过时
      const ROUTES = [
        { tag:'第 1 站', tone:'', title:'刚上手',
          desc:'先修的几个。后面很多算法要借这里的手法和代码习惯。', groups:['入门'] },
        { tag:'第 2 站', tone:'orange', title:'一个数组上的功夫',
          desc:'反复查询和修改一个数组，三种做法思路不同，复杂度从 O(log n) 到 O(√n)。', groups:['序列与区间','分块专题'] },
        { tag:'第 3 站', tone:'gray', title:'往深了走',
          desc:'多维约束、会变形的树、带环的图，以及一些只能靠重构换复杂度的题。', groups:['高阶数据结构','字符串','图论'] }
      ];
      const fileName = a => a.file.split('/').pop().toLowerCase();

      const grouped = (data.groups || [])
        .map(g => Object.assign({}, g, { items: algos.filter(a => a.group === g.name) }))
        .filter(g => g.items.length);
      const rest = algos.filter(a => !grouped.some(g => g.items.indexOf(a) >= 0));
      if (rest.length) grouped.push({ name:'其他', items:rest });

      // 8a. 侧边栏（分组 + 搜索，当前页高亮）
      if (sidebar) {
        sidebar.innerHTML =
          '<p class="site-title"><a href="' + ROOT + 'index.html">算法教程</a></p>' +
          '<div id="sidebar-search"></div>' +
          grouped.map((g, i) =>
            '<div class="nav-group">' +
              '<div class="nav-group-title"><span class="nav-group-idx">' + (i + 1) + '</span>' + g.name + '</div>' +
              '<ul class="algo-nav">' +
                g.items.map(a =>
                  '<li><a class="algo-link' + (fileName(a) === current ? ' active' : '') + '" href="' + ROOT + a.file + '"' +
                  ' data-title="' + a.title + '" data-desc="' + a.description + '">' + a.title + '</a></li>'
                ).join('') +
              '</ul>' +
            '</div>'
          ).join('');

        const holder = sidebar.querySelector('#sidebar-search');
        const box = buildSearch('sidebarSearchInput', '搜索算法', '搜索算法…');
        holder.appendChild(box);
        const hint = document.createElement('p');
        hint.className = 'empty-hint';
        hint.textContent = '没有匹配的算法。';
        hint.hidden = true;
        holder.appendChild(hint);

        const domGroups = Array.prototype.map.call(sidebar.querySelectorAll('.nav-group'), function(ge){
          return {
            el: ge,
            items: Array.prototype.map.call(ge.querySelectorAll('.algo-nav li'), function(li){
              const a = li.querySelector('a');
              return { el: li, text: (a.getAttribute('data-title') + ' ' + a.getAttribute('data-desc')).toLowerCase() };
            })
          };
        });
        registerSearch(box, domGroups, hint);

        const active = sidebar.querySelector('.algo-link.active');
        if (active) active.scrollIntoView({ block:'nearest' });
        const cur = algos.filter(a => fileName(a) === current)[0];
        if (cur) {
          const titleEl = document.querySelector('.topbar-title');
          if (titleEl) titleEl.textContent = cur.title;
        }
      }

      // 8b. 首页卡片（组内顺序即推荐学习顺序）
      const list = document.getElementById('index-list');
      if (list) {
        const wrap = document.createElement('div');
        wrap.id = 'indexSearch';
        wrap.appendChild(buildSearch('indexSearchInput', '搜索算法', '搜索算法…（按 / 聚焦）'));
        list.parentNode.insertBefore(wrap, list);

        const hint = document.createElement('p');
        hint.className = 'empty-hint';
        hint.textContent = '没有匹配的算法，换个关键词试试。';
        hint.hidden = true;
        list.parentNode.insertBefore(hint, list);

        list.innerHTML = grouped.map((g, i) =>
          '<section class="group">' +
            '<h2 class="group-title"><span class="group-idx">' + (i + 1) + '</span>' + g.name + '</h2>' +
            (g.desc ? '<p class="group-desc">' + g.desc + '</p>' : '') +
            '<div class="cards">' +
              g.items.map(a => {
                const lv = a.level || 1;
                return '<a class="card" href="' + ROOT + a.file + '" data-level="' + lv + '">' +
                  '<div class="card-head"><h3>' + a.title + '</h3>' +
                  '<span class="lv lv-' + lv + '">' + LV[lv] + '</span></div>' +
                  '<p>' + a.description + '</p>' +
                '</a>';
              }).join('') +
            '</div>' +
          '</section>'
        ).join('');

        const domGroups = Array.prototype.map.call(list.querySelectorAll('.group'), function(ge){
          return {
            el: ge,
            items: Array.prototype.map.call(ge.querySelectorAll('.card'), function(c){
              return { el: c, text: c.textContent.toLowerCase(), level: +(c.getAttribute('data-level') || 1) };
            })
          };
        });

        let levelFilter = 0; // 0 = 全部
        const applyIndex = registerSearch(wrap.querySelector('.search-box'), domGroups, hint,
          function(it){ return levelFilter === 0 || it.level === levelFilter; });

        // 8b-1. 顶部统计（数字全部从索引算，不写死）
        const setText = function(id, v){
          const el = document.getElementById(id);
          if (el) el.textContent = v;
        };
        const levels = Array.from(new Set(algos.map(a => a.level || 1))).sort(function(x, y){ return x - y; });
        setText('statTotal', algos.length);
        setText('statCount', algos.length);
        setText('statGroups', grouped.length);
        setText('statGroupCount', grouped.length);
        setText('statLevelCount', levels.length);
        setText('statHighCount', algos.filter(a => (a.level || 1) >= 3).length);

        // 8b-2. 难度筛选（点一下过滤下面的卡片，和搜索框是叠加的）
        const chipsBox = document.getElementById('levelChips');
        if (chipsBox) {
          const counts = {};
          algos.forEach(a => { const lv = a.level || 1; counts[lv] = (counts[lv] || 0) + 1; });
          const addChip = function(lv, label, n){
            const b = document.createElement('button');
            b.type = 'button';
            b.className = 'chip' + (lv === 0 ? ' active' : '');
            b.textContent = label + ' ' + n;
            b.addEventListener('click', function(){
              levelFilter = lv;
              Array.prototype.forEach.call(chipsBox.children, function(c){ c.classList.remove('active'); });
              b.classList.add('active');
              applyIndex();
            });
            chipsBox.appendChild(b);
          };
          addChip(0, '全部', algos.length);
          levels.forEach(lv => addChip(lv, LV[lv], counts[lv] || 0));
        }

        // 8b-3. 三条学习路线（篇目与篇数取自索引，点进去是该路线的第一篇）
        const pathsBox = document.getElementById('routePaths');
        if (pathsBox) {
          pathsBox.innerHTML = ROUTES.map(function(r){
            const items = r.groups.reduce(function(acc, name){
              const g = grouped.filter(x => x.name === name)[0];
              return g ? acc.concat(g.items) : acc;
            }, []);
            if (!items.length) return '';
            return '<a class="path-card" href="' + ROOT + items[0].file + '">' +
                '<span class="path-tag ' + r.tone + '">' + r.tag + '</span>' +
                '<h3>' + r.title + '</h3>' +
                '<p class="path-desc">' + r.desc + '</p>' +
                '<p class="path-list">' + items.slice(0, 4).map(a => '· ' + a.title).join('\n') + '</p>' +
                '<span class="path-more">' + r.groups.join(' + ') + ' · ' + items.length + ' 篇</span>' +
              '</a>';
          }).join('');
        }
      }

      // 8c. 上下篇（按 json 里的整体顺序）
      if (main && sidebar) {
        const flat = grouped.reduce((acc, g) => acc.concat(g.items), []);
        const idx = flat.findIndex(a => fileName(a) === current);
        if (idx >= 0) {
          const addPager = function(a, dir){
            const nav = document.createElement('nav');
            nav.className = 'pager';
            const link = document.createElement('a');
            link.className = 'pager-item ' + dir;
            link.href = ROOT + a.file;
            link.innerHTML = '<span class="pager-label">' + (dir === 'prev' ? '上一篇' : '下一篇') + '</span>' +
                             '<span class="pager-title">' + a.title + '</span>';
            nav.appendChild(link);
            main.appendChild(nav);
          };
          if (idx > 0) { prevHref = ROOT + flat[idx - 1].file; addPager(flat[idx - 1], 'prev'); }
          if (idx < flat.length - 1) { nextHref = ROOT + flat[idx + 1].file; addPager(flat[idx + 1], 'next'); }
        }
      }
    })
    .catch(err => {
      console.error('加载 algorithms.json 失败:', err);
    });
})();
