# 项目约定（algorithms-course）

## 内容规范
- 每页必须含「复杂度分析」小节：一张 `table.complexity` + 空间说明，明确 O(...)。
- 代码样例统一 C++（`#include <bits/stdc++.h>` + `using namespace std`）。页面上贴**关键函数**，不贴完整 main。
- 导航/标题/正文不出现「模块」字样，导航直接写算法名。

## 图示规范
- 每页至少 **2 张图**、至少 **1 个交互演示**；「一句话讲不清」的结论、结构、过程都要配图。
- 默认内联 SVG（JS 生成后塞进 `<figure class="fig">` + 一句 `<figcaption>`）；数组/表格/树也可 HTML+CSS 网格。不引入位图。
- 配图位置：① 问题定义 ② 朴素做法为什么慢 ③ 数据结构形态 ④ 一次操作完整过程 ⑤ 前后对比。
- 配色：主蓝 #2563eb（底 #dbeafe/#eef4ff）、次橙 #ea580c（底 #ffedd5）、灰 #9aa0a6、底 #f6f7f9、边框 #e6e8eb；浅色底 + 深色字。
- SVG `<text>` 字号 ≥10；中文按 12~13px/字估宽，`text-anchor="middle"` 的长句最容易左侧溢出。
- **画图时顺带核对正文结论**，发现冲突改正文，不要改图迁就错话。

## 写作风格（去 AI 味，对齐 tiankonguse.github.io 的 OI 文章）
- 段落极短，一到两句；用「可以发现 / 因此 / 具体来说 / 这里有个细节」推进。
- 章节：零、背景 → 一、问题 → 二、朴素做法 → … → 复杂度分析 → 最后；结尾「《完》」式短收尾。
- 禁用 AI 模板句：「矛盾在哪？」「想一想：」「关键提醒」「核心思想」、emoji、破折号滥用、通篇加粗。强调靠推导，少用 `<strong>`。
- 引号用「」，数值范围写「0 到 10⁹」。不做总结式清单。
- **「不是 X，是 Y」否定并列是最隐蔽的 AI 句式**（写作时完全无感，用户点名才认出来）。收口前 `grep -n '不是\|而是\|并非'` 全篇扫一遍。**技术对比留着**（在两个具体选项里挑一个，是人话）：「写 `1u << i` 而不是 `1 << i`」「向下取整而不是向零取整」；**拿它当修辞强调的一律删**。
- 别用「手滑」这类网络造词绕开直白说法，直接写「写错」。
- 自己抛出反问后，回答时先接一句「确实…」再转折，别甩「X，但 Y」这种四字判词。
- 结尾要有「总结」段（2026-10-04 用户定）：参考博客「最后」写法，具体要点 + 一句前瞻，不许空谈升华（「标志着/意味着」禁用）。正文不提 PPT/课件、代码不提 Java——课程页面是给学生看的。
- 四篇范本（tiankonguse.github.io 的 ST-RMQ / DFS序 / LCA / 虚树）的完整风格要点已提炼进 SKILL.md「写作风格」的「范本对齐」块：背景只定位 2~3 段 / 问题驱动逐层逼近 / 反问与「如果…就…」推进 / 复杂度跟着推导走 / 每节末一句定型小结 / 「最后」= 注意事项或要点+适用+前瞻 / 代码紧贴小节。四篇原文已收录在技能 `~/.workbuddy/skills/course-site-page/references/`，动笔前先翻。去 AI 味的完整规则清单（原「去AI味 / humanizer」技能）也已搬进同一目录，文件名 `humanize.md`（速览索引 `humanize-readme.md`），写作与收口前照它过一遍。

## 视觉与目录
- 纯静态无构建。`index.html` 首页 + `algorithms/` 每算法一页；`assets/css/shared.css`、`assets/js/shared.js`、`assets/data/algorithms.json`。
- 页面骨架固定：`<div class="layout">` + `<aside class="sidebar" id="sidebar">` + `<main class="main"><section class="algo-section">` + `<aside class="toc-panel"><ul class="toc-list" id="toc">`；侧栏/目录/翻页/复制/搜索/进度条全由 shared.js 注入，**页面一行都不要自带骨架**。
- 独有样式与专属脚本留在各页 `<style>` / `<script>` 内。
- `.fig`/`.fig svg`/`.legend`/`.dot`/`.breakdown`/`pre.code .cm|.kw|.fn`/`table.complexity` 只在 shared.css 定义一份，新页直接复用。
- 全局要保留 `[hidden]{display:none!important}`。
- json 里 `file` 写 `algorithms/xxx.html`；侧栏高亮比 basename。**顺序 = 首页卡片 = 侧栏 = 上下篇翻页**。
- 本地预览用 `./run.sh`（端口 8080）；`file://` 直开会因 fetch 被 CORS 拦。

## 从 algorithm-journey 转课的流水线（技能 `~/.workbuddy/skills/course-site-page`）
- 技能内含 gen_code.py / apply_placeholders.py / verify_pages.js / verify_viz_steps.js / verify_home.js / shot_element.js / check_branch.js。**进度表在 `prompt.md`**：2026-10-03 起**从小到大转（001 → 207）**；已完成 159~207 共 49 课（倒序阶段产物，不回炉），下一课 001，导学类记「跳过」。
- 材料：`ppt/算法讲解0NN【难度】标题.pptx` + `src/classNNN/Code0X_*.java`。PPT 文本：zipfile + ElementTree 遍历 `ppt/slides/slideN.xml` 的 `p:sp`，拼 `a:t`。
- **低号课（001~）只有一份 Java**：文件名是题目名（`class004/SelectBubbleInsert.java`），没有 `Code0X_` 前缀，也没有「C++ 塞注释」的第二份，有的课目录是空的（008）。页面统一 C++，得自己从 Java 改写再编译对拍。
- **偶数号 java 里藏着 C++**（这条只对中高号课成立）：`awk '/^\/\/#include/{f=1} f&&/^\/\//{sub(/^\/\//,"");print;next} f{print}' X.java > X.cpp`
- 写页面：先 Write 带 `@@片段名@@` 占位的版本，再用 apply_placeholders.py 替换（占位名必须等于片段文件名）。
- 省上下文：PPT 抽到 `/tmp/pptNNN.txt` 后用 grep/sed 按段取；源码只取要贴的那段；收口核验只用 grep/ls/find，**别把新页面 HTML 读回来**。

## 编排（2026-10-03 用户改定：从小到大）
- **转换顺序改为从小到大（001 → 207）**。倒着转会让页面「背景」先讲高阶算法、后头才铺垫基础，教学顺序反了；159~207 那 49 课是倒序阶段的产物，**不回炉重排**，从 001 接着往前推。
- **导学课不产页**：PPT 里没算法、或 `src/classNNN/` 下没代码的课（如 001「语言的语言问题」、002「从社会实验到入门提醒」），记进 prompt.md 的「跳过」并写原因，直接进下一课，别硬凑页面。
- **新组默认插在 `groups` 里「入门」之后**（`splice(1,0,新组)`，同阶段的多个组按课号升序排）——倒序阶段的「追加到最末」会让首页变成「二维树套树(160) → 排序(004)」。已有 26 个组的相对顺序不动。
- **低号课可能撞站内已有页**（早期建的「入门」「序列与区间」「批量处理查询」「高阶数据结构」等组）：开工前扫一遍 json 的 `title`，撞主题就并入已有组或给已有页补一节，不新建重复页。

## 编排（2026-10-02 用户定调，部分被上面覆盖）
- **转一节课不开子智能体**，主循环直接做；「连转N课」串行，一课收口干净再开下一课。只有用户明说「并行」才开子智能体。
- **新组位置判据 = 边界页互链**，不是「一律追加到最后」。先例：SCC 组（189~190）必须紧贴「双连通分量与圆方树」之前（`scc-dp` 结尾「下一讲换割边」、`ebcc-diameter` 开头「上一节已经给过了」）。
- **组内按教学序升序**（课号小 → 大）；后续同主题课插到组内对应位置，不追加到组尾。老组「高阶数据结构」是历史追加的降序，不要重排。
- 已有组顺序（首 → 末）：入门、序列与区间、CDQ 分治（170）、分块专题（172/173/174）、批量处理查询、图论、字符串、高阶数据结构（205~199）、优化建图与 2-SAT（195~198）、线段树的合并与分裂、虚树、点分治与点分树、树上 LCA（186）、边分治与边分树（187）、欧拉路径（188）、强连通分量与缩点（189~190）、双连通分量与圆方树（191~194）、根号分治（175）、整体二分、可持久化与可撤销并查集（165）、可持久化前缀树（159，插在 167 之前）、线段树分治（166/167）、Kruskal 重构树（164）、树上启发式合并（163）、树链剖分与长链剖分（161/162）、**二维树套树（160，最末）**。这份顺序是倒序阶段「新组追加到最末」堆出来的历史结果，不要按课号重排；从小到大阶段的新组按上面那条插到「入门」之后。
- 组名撞车要避开：`kdtree-of-tree.html`（高阶数据结构组）标题已占「树套树（树状数组套 K-D 树）」，160 课的新组因此叫「二维树套树」。
- 每课通常拆 2~4 页，按「技巧同源」合并（如 167：背包类 / 线性基类 / Trie 类）。

## 自检（全自动，别只靠截图）
- Chrome 会随 Bash 命令结束被回收：**「起 Chrome + 跑 node」必须写在同一条命令里**；开工前 `pkill -f "remote-debugging-port="` 清残留。
  ```bash
  rm -rf /tmp/cdpX; "$CHROME" --headless=new --no-sandbox --disable-gpu --remote-debugging-port=9333 \
    --user-data-dir=/tmp/cdpX --allow-file-access-from-files about:blank >/dev/null 2>&1 &
  sleep 8
  $N $SKILL/verify_pages.js a.html b.html        # JS 错误 / fig==svg 数 / nav·toc·viz / svg 越界 / pager 链
  $N $SKILL/verify_viz_steps.js a.html           # 逐帧点（只查第一个 .viz；第二个演示用 sed 换 id 再跑）
  $N $SKILL/verify_home.js
  ```
- 脚本默认端口 9333，读环境变量 `CDP_PORT`；换端口两处都要改，否则误判成「Chrome 没起来」。
- 别加 `--window-size`（隐藏图标 svg 会被判空图）；`--allow-file-access-from-files` 必带。
- 必查四项：① 无 JS 错误 ② `figure.fig` 数 == 其中 svg 数 ③ nav/toc/每个 `.viz` 有内容 ④ 每个 svg 用 `getBBox()` 比对 viewBox。bbox 报的是 (left, top, right, bottom)。
- 全站列表：`PAGES=$(python3 -c "import json;print(' '.join(a['file'].split('/')[-1] for a in json.load(open('assets/data/algorithms.json'))['algorithms']))")`，zsh 里必须 `${=PAGES}`。
- 首页单独查 verify_home.js；**首页没有侧栏，`navGroups` 为 0 正常**。
- 全站跑到第 49 页左右会偶发 createTarget/evaluate 超时，脚本已重试；真失败就换 `--user-data-dir` 补跑剩余页。
- pager 链是最灵敏的探针：非末页 2 个 pager、末页 1 个，且标题与 json 相邻项一致。

## 踩过的坑
- **课程代码 ≠ 题面直觉**：先用随机小用例做「假设检验」确认代码真实语义，再按此写暴力对拍，别反过来改代码（BZOJ2870 会把单点链算进答案）。
- 强制在线的题（CF757G 异或 lastAns）不能随机喂原始参数对拍，要**交互编码**：逐条喂解码后的操作、读暴力输出当 lastAns。暴力输出用 `endl` 不用 `"\n"`。
- 本机**没有 `bits/stdc++.h`**：自建 shim `/tmp/cppNNN/inc/bits/stdc++.h`，编译加 `-I inc -std=c++17`。对拍结果 `grep -E 'OK|MISMATCH'`。
- 并行（仅用户明确要求时）：立刻逐个确认「负责第几课 / 文件名 / /tmp 目录 / CDP 端口」；同名文件互踩时先 cp 快照、用备份裁定、`TaskStop` 不听话的那个、换干净目录端口另起。子智能体报告的「已完成」必须用磁盘事实验（mtime、字节数、fig 数、占位符残留、`id="sidebar"` 是空容器）。
- gen_code.py：`#include` 必须先换哨兵再跑关键词正则，否则 `class` 被二次包裹；fn 只高亮定义处。
- **演示的控制按钮容器统一写 `class="viz-controls"`**：`shot_element.js` 按 `.viz-controls button` 索引点击，自造 `.btnrow` 会点空、截图停在初始帧。纯 HTML 演示（无 svg）`verify_viz_steps.js` 只打印「跳过 bbox」，排版要自己用 `shot_steps.js` 点 N 帧截图看。
- **课程代码把树状数组上界写成 `n` 的题**（如 P4093 序列）：随机数据要把值限制在 ≤ n，否则 `more()` 静默不写、对拍假 MISMATCH。先怀疑数据再怀疑代码。
- **并发会话**：同一个站可能同时有几课在转（2026-10-02 有 168/170/171）。开工先探 `/tmp/cppNNN`、`/tmp/hlNNN`、`algorithms/` 的新文件，能复用就复用；组可能已被别人建好，同主题新课插到组内对应位置而不是新建组；写 json 前 cp 备份 + 重读，别动别人的半成品页面；不要 pkill 调试端口，用独立端口与 `--user-data-dir`。
