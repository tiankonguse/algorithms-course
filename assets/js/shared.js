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
    top:'<svg viewBox="0 0 20 20" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round" stroke-linejoin="round"><path d="M10 15V5M5 10l5-5 5 5"/></svg>',
    chevron:'<svg class="nav-group-chevron" viewBox="0 0 16 16" fill="none" stroke="currentColor" stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path d="M4 6l4 4 4-4"/></svg>'
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
  // afterApply: 可选的搜索后回调，参数为当前是否有搜索词（侧边栏用它临时展开命中组 / 清空后恢复折叠）
  const searches = [];
  function registerSearch(box, groups, emptyEl, extra, afterApply){
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
      if (afterApply) afterApply(keys.length > 0);
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
      const fileName = a => a.file.split('/').pop().toLowerCase();
      const esc = s => String(s == null ? '' : s)
        .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');

      // 首页三条学习路线：写死二级分类名，目录变了也能自动算篇数与篇目
      const ROUTES = [
        { tag:'第 1 站', tone:'', title:'刚上手',
          desc:'先把一个数组摆平：排好序、二分掉、把一段区间的和压成两个前缀之差。后面很多页面都要借这里的手法。',
          subs:['排序','二分','前缀和与差分'] },
        { tag:'第 2 站', tone:'orange', title:'一个数据结构管一片',
          desc:'同一个「区间加、区间求和」的需求，三种做法思路完全不同：常数最小的树状数组、什么都管的线段树、粗糙但够用的分块。',
          subs:['树状数组','线段树','分块'] },
        { tag:'第 3 站', tone:'gray', title:'往深了走',
          desc:'换个维度看：询问可以重排就去掉一维，树可以先摊平再统计，以及按时间锯开再各自算的 CDQ 分治。',
          subs:['莫队算法','树分治','CDQ 分治'] }
      ];

      // 8-0. 三层目录：一级分类 → 二级分类 → 页面（三级就是页面本身）
      const byGroup = {};
      algos.forEach(function(a){ (byGroup[a.group] = byGroup[a.group] || []).push(a); });
      const tree = (data.categories || [])
        .map(function(name){
          return {
            name: name,
            subs: (data.groups || [])
              .filter(function(g){ return g.cat === name && byGroup[g.name] && byGroup[g.name].length; })
              .map(function(g){ return { name: g.name, desc: g.desc, items: byGroup[g.name] }; })
          };
        })
        .filter(function(c){ return c.subs.length; });
      const catCount = c => c.subs.reduce((n, s) => n + s.items.length, 0);
      const subCount = tree.reduce((n, c) => n + c.subs.length, 0);
      const flat = tree.reduce(function(acc, c){
        c.subs.forEach(function(s){ s.items.forEach(function(a){ acc.push(a); }); });
        return acc;
      }, []);

      const curAlgo = algos.filter(a => fileName(a) === current)[0];
      const curSub = curAlgo ? curAlgo.group : null;
      const curCatName = curAlgo
        ? ((data.groups || []).filter(g => g.name === curAlgo.group)[0] || {}).cat
        : null;

      function catIndex(n){ for (let i = 0; i < tree.length; i++) if (tree[i].name === n) return i; return -1; }
      function subKey(cat, sub){ return cat + '>' + sub; }

      // 8-1. 一级分类顶栏（一行全部显示，空的分类不出现）
      const catNav = (function(){
        let el = document.getElementById('catNav');
        if (el) return el;
        el = document.createElement('nav');
        el.className = 'cat-nav';
        el.id = 'catNav';
        el.setAttribute('aria-label', '一级分类');
        const tb = document.querySelector('.topbar');
        const layout = document.querySelector('.layout');
        if (tb) tb.parentNode.insertBefore(el, tb.nextSibling);
        else if (layout) body.insertBefore(el, layout);
        else body.insertBefore(el, body.firstChild);
        return el;
      })();

      catNav.innerHTML = tree.map((c, i) =>
        '<a class="cat-tab" href="' + ROOT + 'catalog.html#cat-' + encodeURIComponent(c.name) + '"' +
        ' data-cat="' + i + '">' + c.name +
        '<span class="cat-count">' + catCount(c) + '</span></a>'
      ).join('');

      const isIndex = !!document.getElementById('index-list');
      let activeCat = -1;

      function markCatTabs(i){
        activeCat = i;
        Array.prototype.forEach.call(catNav.querySelectorAll('.cat-tab'), function(t){
          t.classList.toggle('active', +t.getAttribute('data-cat') === i);
        });
      }

      function selectCat(i, writeHash){
        if (i < 0 || i >= tree.length) return;
        markCatTabs(i);
        if (sidebar) renderSidebar(i); else renderHome(i);
        if (writeHash && history.replaceState) {
          history.replaceState(null, '', '#cat-' + encodeURIComponent(tree[i].name));
        }
      }

      Array.prototype.forEach.call(catNav.querySelectorAll('.cat-tab'), function(t){
        t.addEventListener('click', function(e){
          const i = +t.getAttribute('data-cat');
          if (isIndex) {
            // catalog.html：留在当前页切正文，并用 hash 记下当前分类
            e.preventDefault();
            selectCat(i, true);
          } else if (sidebar) {
            // 算法页：留在当前页，只把左侧目录换成该分类
            e.preventDefault();
            selectCat(i);
          }
          // 总览首页（既无 #index-list 也无侧栏）：不拦截，按 href 跳到 catalog.html#cat-<分类名>
        });
      });

      // 8-2. 二级分类折叠的状态由三样东西决定：三级（页面列表）默认折叠，
      // 当前页所在的那一组强制展开，其余由用户手动展开并跨页记住
      const OPEN_KEY = 'algoNavOpenSubs';
      const manualOpen = (function(){
        try { return new Set(JSON.parse(localStorage.getItem(OPEN_KEY) || '[]')); }
        catch(e){ return new Set(); }
      })();
      function persistManualOpen(){
        try { localStorage.setItem(OPEN_KEY, JSON.stringify(Array.from(manualOpen))); }
        catch(e){ /* 隐私模式等场景忽略 */ }
      }
      function subHtml(cat, s, j){
        const key = subKey(cat.name, s.name);
        const open = s.name === curSub || manualOpen.has(key);
        return '<div class="nav-group' + (open ? '' : ' collapsed') + '" data-sub="' + esc(s.name) + '">' +
            '<button type="button" class="nav-group-title" aria-expanded="' + open + '">' +
              '<span class="nav-group-idx">' + (j + 1) + '</span>' +
              '<span class="nav-group-name">' + s.name + '</span>' +
              '<span class="nav-group-count">' + s.items.length + '</span>' +
              ICON.chevron +
            '</button>' +
            '<ul class="algo-nav">' +
              s.items.map(a =>
                '<li><a class="algo-link' + (fileName(a) === current ? ' active' : '') + '"' +
                ' href="' + ROOT + a.file + '" data-title="' + esc(a.title) + '"' +
                ' data-desc="' + esc(a.description) + '">' + a.title + '</a></li>'
              ).join('') +
            '</ul>' +
          '</div>';
      }
      function bindSubToggles(host, onToggle){
        Array.prototype.forEach.call(host.querySelectorAll('.nav-group'), function(ge){
          const btn = ge.querySelector('.nav-group-title');
          const name = ge.getAttribute('data-sub');
          btn.addEventListener('click', function(){
            const collapsed = ge.classList.toggle('collapsed');
            btn.setAttribute('aria-expanded', String(!collapsed));
            const key = subKey(host.getAttribute('data-cat'), name);
            if (!collapsed) manualOpen.add(key); else manualOpen.delete(key);
            persistManualOpen();
            if (onToggle) onToggle();
          });
        });
      }

      // 8-3. 侧边栏：只列当前一级分类下的二级分类
      function renderSidebar(i){
        const cat = tree[i];
        searches.length = 0;
        sidebar.innerHTML =
          '<p class="site-title"><a href="' + ROOT + 'index.html">算法教程</a></p>' +
          '<div id="sidebar-search"></div>' +
          '<p class="sidebar-cat">' + cat.name + ' · ' + cat.subs.length + ' 个专题</p>' +
          '<div class="sidebar-subs" data-cat="' + esc(cat.name) + '">' +
            cat.subs.map((s, j) => subHtml(cat, s, j)).join('') +
          '</div>';
        sidebar.setAttribute('data-cat', esc(cat.name));

        const holder = sidebar.querySelector('#sidebar-search');
        const box = buildSearch('sidebarSearchInput', '搜索算法', '搜索 ' + cat.name + '…');
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

        function resetOpen(){
          domGroups.forEach(function(dg){
            const key = subKey(cat.name, dg.el.getAttribute('data-sub'));
            const open = dg.el.getAttribute('data-sub') === curSub || manualOpen.has(key);
            dg.el.classList.toggle('collapsed', !open);
            const btn = dg.el.querySelector('.nav-group-title');
            if (btn) btn.setAttribute('aria-expanded', String(open));
          });
        }
        bindSubToggles(sidebar.querySelector('.sidebar-subs'), null);

        registerSearch(box, domGroups, hint, null, function(searching){
          if (searching) domGroups.forEach(function(dg){ if (!dg.el.hidden) dg.el.classList.remove('collapsed'); });
          else resetOpen();
        });

        // 当前算法自动定位：所在组已展开，把高亮项滚动到侧边栏可视区正中间
        const active = sidebar.querySelector('.algo-link.active');
        if (active) {
          const ar = active.getBoundingClientRect();
          const sbRect = sidebar.getBoundingClientRect();
          const target = sidebar.scrollTop + (ar.top - sbRect.top) - (sbRect.height - ar.height) / 2;
          sidebar.scrollTop = Math.max(0, Math.min(target, sidebar.scrollHeight - sbRect.height));
        }
        const titleEl = document.querySelector('.topbar-title');
        if (titleEl && curAlgo) titleEl.textContent = curAlgo.title;
      }

      // 8-4. 首页：左侧三级目录 + 正文按二级分类铺卡片
      const list = document.getElementById('index-list');
      if (list && !document.getElementById('indexSearch')) {
        const searchWrap = document.createElement('div');
        searchWrap.id = 'indexSearch';
        searchWrap.appendChild(buildSearch('indexSearchInput', '搜索算法', '搜索算法…（按 / 聚焦）'));
        list.parentNode.insertBefore(searchWrap, list);
        const searchHint = document.createElement('p');
        searchHint.className = 'empty-hint';
        searchHint.textContent = '没有匹配的算法，换个关键词试试。';
        searchHint.hidden = true;
        list.parentNode.insertBefore(searchHint, list);
      }
      let spyGroups = [], spyLinks = [], spyTicking = false;
      window.addEventListener('scroll', function(){
        if (!spyLinks.length || spyTicking) return;
        spyTicking = true;
        requestAnimationFrame(function(){
          spyTicking = false;
          const y = window.scrollY;
          let cur = -1;
          for (let i = 0; i < spyGroups.length; i++) {
            const top = spyGroups[i].getBoundingClientRect().top + y - 76;
            if (top <= y + 10) cur = i; else break;
          }
          spyLinks.forEach(function(l, i){ l.classList.toggle('active', i === cur); });
        });
      }, { passive: true });

      function renderHome(i){
        const cat = tree[i];
        const homeNav = document.getElementById('homeNav');
        if (homeNav) {
          homeNav.innerHTML =
            '<p class="nav-title">' + cat.name + '</p>' +
            '<div class="sidebar-subs" data-cat="' + esc(cat.name) + '">' +
              cat.subs.map((s, j) => subHtml(cat, s, j)).join('') +
            '</div>';
          bindSubToggles(homeNav.querySelector('.sidebar-subs'), null);
        }

        const section = document.getElementById('catalogTitle');
        if (section) section.textContent = cat.name;
        const hintEl = document.getElementById('catalogDesc');
        if (hintEl) {
          hintEl.textContent = cat.subs.length + ' 个专题，' + catCount(cat) + ' 篇；'
            + '左侧展开任意专题可以直接跳到那一篇。';
        }

        searches.length = 0;
        list.innerHTML = cat.subs.map((s, j) =>
          '<section class="group" id="group-' + j + '">' +
            '<h2 class="group-title"><span class="group-idx">' + (j + 1) + '</span>' + s.name +
              '<span class="group-count">' + s.items.length + ' 篇</span></h2>' +
            (s.desc ? '<p class="group-desc">' + s.desc + '</p>' : '') +
            '<div class="cards">' +
              s.items.map(a => {
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

        spyGroups = Array.prototype.slice.call(list.querySelectorAll('.group'));
        spyLinks = homeNav ? Array.prototype.slice.call(homeNav.querySelectorAll('.nav-group-title')) : [];

        const domGroups = Array.prototype.map.call(list.querySelectorAll('.group'), function(ge){
          return {
            el: ge,
            items: Array.prototype.map.call(ge.querySelectorAll('.card'), function(c){
              return { el: c, text: c.textContent.toLowerCase(), level: +(c.getAttribute('data-level') || 1) };
            })
          };
        });

        let levelFilter = 0; // 0 = 全部
        const wrap = document.getElementById('indexSearch');
        const hint = wrap && wrap.parentNode.querySelector('.empty-hint');
        const applyIndex = registerSearch(wrap.querySelector('.search-box'), domGroups, hint,
          function(it){ return levelFilter === 0 || it.level === levelFilter; });

        // 难度筛选 chips 每次重渲染重建一遍（首页切换一级分类时跟着变）
        const chipsBox = document.getElementById('levelChips');
        if (chipsBox) {
          const counts = {};
          cat.subs.forEach(s => s.items.forEach(a => {
            const lv = a.level || 1; counts[lv] = (counts[lv] || 0) + 1;
          }));
          const all = catCount(cat);
          chipsBox.innerHTML = '';
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
          addChip(0, '全部', all);
          Object.keys(counts).map(Number).sort((x, y) => x - y)
            .forEach(lv => addChip(lv, LV[lv], counts[lv]));
        }
      }

      // 8-5. 顶部统计（数字全部从索引算，不写死）
      const setText = function(id, v){
        const el = document.getElementById(id);
        if (el) el.textContent = v;
      };
      setText('statTotal', algos.length);
      setText('statCount', algos.length);
      setText('statGroups', tree.length);
      setText('statGroupCount', subCount);
      setText('statL1', tree.length);
      setText('statL2', subCount);
      setText('statHighCount', algos.filter(a => (a.level || 1) >= 3).length);

      // 8-6. 三条学习路线（二级分类改名后照旧按名取篇目）
      const pathsBox = document.getElementById('routePaths');
      if (pathsBox) {
        pathsBox.innerHTML = ROUTES.map(function(r){
          const items = r.subs.reduce(function(acc, name){
            for (let i = 0; i < tree.length; i++) {
              const s = tree[i].subs.filter(x => x.name === name)[0];
              if (s) { return acc.concat(s.items); }
            }
            return acc;
          }, []);
          if (!items.length) return '';
          return '<a class="path-card" href="' + ROOT + items[0].file + '">' +
              '<span class="path-tag ' + r.tone + '">' + r.tag + '</span>' +
              '<h3>' + r.title + '</h3>' +
              '<p class="path-desc">' + r.desc + '</p>' +
              '<p class="path-list">' + items.slice(0, 4).map(a => '· ' + a.title).join('\n') + '</p>' +
              '<span class="path-more">' + r.subs.filter(function(n){
                return tree.some(c => c.subs.some(s => s.name === n));
              }).join(' + ') + ' · ' + items.length + ' 篇</span>' +
            '</a>';
        }).join('');
      }

      // 8-6b. 总览页：难度分布横向条形图（档位篇数全部从索引实时算）
      (function renderLevelChart(){
        const box = document.getElementById('levelChart');
        if (!box) return;
        const LV_COLOR = { 1:'#2563eb', 2:'#ea580c', 3:'#6b7280', 4:'#7c3aed', 5:'#be123c' };
        const counts = { 1:0, 2:0, 3:0, 4:0, 5:0 };
        algos.forEach(function(a){
          const lv = a.level || 1;
          if (counts[lv] == null) counts[lv] = 0;
          counts[lv]++;
        });
        const levels = [1, 2, 3, 4, 5];
        // 以篇数最多的那一档为满宽，其余按比例缩，条长与篇数成正比
        const maxCount = levels.reduce(function(m, lv){ return Math.max(m, counts[lv] || 0); }, 0) || 1;

        const W = 640, rowH = 46, padTop = 22, barH = 24;
        const labelX = 50, barX = 62, maxBarW = 498, countX = barX + maxBarW + 14;
        const H = padTop + levels.length * rowH + 6;

        let svg = '<svg viewBox="0 0 ' + W + ' ' + H + '" xmlns="http://www.w3.org/2000/svg"' +
          ' role="img" aria-label="五档难度的篇数分布"' +
          ' font-family="-apple-system,BlinkMacSystemFont,\'PingFang SC\',sans-serif">';
        levels.forEach(function(lv, i){
          const y0 = padTop + i * rowH;
          const mid = y0 + rowH / 2;
          const barY = y0 + (rowH - barH) / 2;
          const n = counts[lv] || 0;
          const w = Math.round((n / maxCount) * maxBarW);
          // 底衬：画出满宽轨迹，方便比长短
          svg += '<rect x="' + barX + '" y="' + barY + '" width="' + maxBarW + '" height="' + barH +
            '" rx="4" fill="#f0f2f5"/>';
          if (n > 0) {
            svg += '<rect x="' + barX + '" y="' + barY + '" width="' + Math.max(w, 3) + '" height="' + barH +
              '" rx="4" fill="' + LV_COLOR[lv] + '"/>';
          }
          svg += '<text x="' + labelX + '" y="' + (mid + 5) + '" text-anchor="end"' +
            ' font-size="13" font-weight="600" fill="#1f2328">' + LV[lv] + '</text>';
          svg += '<text x="' + countX + '" y="' + (mid + 5) + '" text-anchor="start"' +
            ' font-size="13" font-weight="600" fill="' + LV_COLOR[lv] + '"' +
            ' font-variant-numeric="tabular-nums">' + n + '</text>';
        });
        svg += '</svg>';
        box.innerHTML = svg;

        const noteEl = document.getElementById('levelChartNote');
        if (noteEl) {
          noteEl.textContent = '全站 ' + algos.length + ' 篇，其中高阶及以上 ' +
            algos.filter(a => (a.level || 1) >= 3).length + ' 篇。';
        }
      })();

      // 8-7. 上下篇（按三层目录铺平后的顺序；两个链接放在同一个 .pager 行内）
      if (main && sidebar) {
        const idx = curAlgo ? flat.findIndex(a => a.file === curAlgo.file) : -1;
        if (idx >= 0) {
          const nav = document.createElement('nav');
          nav.className = 'pager';
          const makeItem = function(a, dir){
            const link = document.createElement('a');
            link.className = 'pager-item ' + dir;
            link.href = ROOT + a.file;
            link.innerHTML = '<span class="pager-label">' + (dir === 'prev' ? '上一篇' : '下一篇') + '</span>' +
                             '<span class="pager-title">' + a.title + '</span>';
            return link;
          };
          if (idx > 0) { prevHref = ROOT + flat[idx - 1].file; nav.appendChild(makeItem(flat[idx - 1], 'prev')); }
          if (idx < flat.length - 1) { nextHref = ROOT + flat[idx + 1].file; nav.appendChild(makeItem(flat[idx + 1], 'next')); }
          if (nav.children.length) main.appendChild(nav);
        }
      }

      // 8-8. 初始选中：算法页看当前页属于哪一类，首页优先看 URL 里的 #cat-<分类名>
      {
        const m = decodeURIComponent(location.hash || '').match(/^#cat-(.+)$/);
        let start = -1;
        if (m) start = catIndex(m[1]);
        if (start < 0 && curCatName) start = catIndex(curCatName);
        if (start < 0) start = Math.max(0, catIndex('算法基础'));
        if (start < 0) start = 0;
        if (sidebar || isIndex) selectCat(start, false);
      }
    })
    .catch(err => {
      console.error('加载 algorithms.json 失败:', err);
    });
})();
