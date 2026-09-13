# 项目约定（algorithms-course）

## 内容规范
- **每个算法都必须包含「复杂度分析」小节**：各操作的时间复杂度、空间复杂度都要明确给出（用 O(...) 表示，含一张时间复杂度表 + 空间说明）。
- 代码样例统一用 C++（`#include <bits/stdc++.h>` + `using namespace std`）。
- 导航/标题/正文不含「模块」字样，导航直接写算法名。

## 视觉规范
- 纯静态、无构建。
- **每个算法独立一个 HTML 页面**：`index.html` 为目录首页（算法卡片列表），每个算法单独成页（如 `block.html` / `fenwick.html` / `mo.html`）。
- 共享样式与逻辑抽离到 `shared.css` / `shared.js`；算法索引统一在 `algorithms.json` 管理（id / file / title / description）。
- 各算法页共用同一套页面骨架：左侧侧边栏（站点标题链接回首页 + 算法导航，当前页高亮）+ 右侧目录面板 + 正文；侧边栏与目录由 `shared.js` 通过 fetch 异步渲染。
- 独有样式（如 fenwick 的 .fen-*、bfs 的 .gcell、pq 的 .heap-*、palindrome 的 .pal-*）和专属交互脚本留在各页 `<style>` / `<script>` 内。
- 极简浅色主题，只保留核心内容，无口号/元信息。
- 代码高亮配色：关键字蓝 #0550ae / 函数名紫 #8250df / 注释灰 #9aa0a6。

## 启动
- 由于 `shared.js` 用 `fetch('algorithms.json')`，**直接双击 `file://` 会被 CORS 拦截**；必须通过 `./run.sh` 启动（默认固定端口 8080），访问 http://localhost:8080/index.html。
- `./run.sh [start|stop|status]`，自动停旧启新；PID/日志写到 `.server.pid` / `.server.log`。
