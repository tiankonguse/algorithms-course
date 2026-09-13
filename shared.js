/* shared.js —— 算法教程公共逻辑
 * - 加载 algorithms.json
 * - 渲染侧边栏算法导航（自动高亮当前页）
 * - 渲染首页卡片列表
 * - 自动生成右侧目录 + 滚动高亮当前小节
 */

(function(){
  // ===== 1. 立即生成 TOC（不依赖 JSON） =====
  const toc = document.getElementById('toc');
  if (toc) {
    const section = document.querySelector('.algo-section');
    const headings = section ? Array.from(section.querySelectorAll('h2')) : [];
    const links = [];

    headings.forEach((h, i) => {
      if (!h.id) h.id = 'sec-' + (i + 1);
      const li = document.createElement('li');
      const a = document.createElement('a');
      a.className = 'toc-item';
      a.href = '#' + h.id;
      a.textContent = h.textContent;
      li.appendChild(a);
      toc.appendChild(li);
      links.push(a);
    });

    function setActive(id) {
      links.forEach(a => a.classList.toggle('active', a.getAttribute('href') === '#' + id));
    }

    function updateToc() {
      if (!headings.length) return;
      let current = headings[0].id;
      for (const h of headings) {
        if (h.getBoundingClientRect().top <= 96) current = h.id;
      }
      if (window.innerHeight + window.scrollY >= document.documentElement.scrollHeight - 4) {
        current = headings[headings.length - 1].id;
      }
      setActive(current);
    }

    window.addEventListener('scroll', updateToc, { passive: true });
    window.addEventListener('resize', updateToc);
    updateToc();
  }

  // ===== 2. 异步加载算法索引 =====
  fetch('algorithms.json')
    .then(r => r.json())
    .then(data => {
      const algos = data.algorithms || [];
      const current = (location.pathname.split('/').pop() || 'index.html').toLowerCase();

      // 2a. 渲染侧边栏
      const sidebar = document.getElementById('sidebar');
      if (sidebar) {
        sidebar.innerHTML =
          '<p class="site-title"><a href="index.html">算法教程</a></p>' +
          '<ul class="algo-nav">' +
            algos.map(a => {
              const active = a.file.toLowerCase() === current ? ' active' : '';
              return '<li><a class="algo-link' + active + '" href="' + a.file + '">' + a.title + '</a></li>';
            }).join('') +
          '</ul>';
      }

      // 2b. 渲染首页卡片列表
      const list = document.getElementById('index-list');
      if (list) {
        list.innerHTML = algos.map(a =>
          '<a class="card" href="' + a.file + '">' +
            '<h2>' + a.title + '</h2>' +
            '<p>' + a.description + '</p>' +
          '</a>'
        ).join('');
      }
    })
    .catch(err => {
      console.error('加载 algorithms.json 失败:', err);
    });
})();