# 项目约定（algorithms-course）

> 只放**规则**。进度 / 下一课看 `prompt.md`，目录与页清单看 `nav/`（生成物 `assets/data/algorithms.json` 只给浏览器读）。

## 导航：nav/ 目录树 + 启动生成（2026-10 起）
- **真值是 `nav/` 目录树，每层只管自己这层**：`nav/index.json` 一级（有序 `[{dir,name,desc}]`）→ `nav/<cat>/index.json` 二级 → `nav/<cat>/<group>/index.json` 页面（有序 `[{file,title,description,level}]`）。目录名英文 slug，中文名与备注写在**父层** json，不存两份。页面 `file` 只写文件名，生成器拼成 `algorithms/<file>`。共 89 个 index.json（1+11+77）。
- 一级 11 个：`lang` 语言基础 / `basics` 算法基础 / `search` 搜索 / `dp` 动态规划 / `string` 字符串 / `math` 数学 / `ds` 数据结构 / `graph` 图论 / `geometry` 计算几何 / `misc` 杂项 / `topics` 专题（无页面的不上顶栏）。二级名与 slug 都**全局唯一**。
- **加页** = 往所属组 `index.json` 插一条四字段（level **数字** 1~5）+ `python3 tools/build_nav.py` 重新生成 + 生成物一起提交。**开新组先写父层 `index.json` 再建目录**（反之报野目录）。忘了写 nav 的页面不会静默丢——下次生成报「孤儿页」。
- `assets/data/algorithms.json` 是**生成物，禁止手改**（`run.sh start` 自动覆盖，改了蒸发且 git diff 看不出）。生成器参数：`--check` 只校验 / `--strict` 警告当错 / `--stats` 打 406/77/9 / `--verify-against <旧json>` 六层比对。13 条硬错（缺/多字段、slug 不合规、`file` 不存在、**大小写不一致**、重复收录、level 非 1~5）+ 孤儿页 / 野目录告警。
- 生成物三键不变，**已无 `id`**（shared.js 翻页改用 `file`）。生成器按组输出 → `algorithms` 数组**必然重排**（406 里 400 个位置变），**定位按 `(group, file)`，别按下标**；`flat` 铺平序与组内相对序一字不差。
- 迁移脚本 `tools/migrate_json_to_nav.py` 是从旧格式恢复的唯一路径，别删。旧 json 无尾换行、`id` 全可由 `basename.replace('-','_')` 复原。
- UI：顶栏一级带篇数；左侧二级可展开出页面列表，**默认全折叠**只展开当前组；展开状态记 `localStorage['algoNavOpenSubs']`（键 `一级>二级`），搜索命中临时全展开。

## 内容
- 每页必含「复杂度分析」（`table.complexity` + 空间说明，明确 O(...)），空间按**题目上限**算。
- 「总结」后固定两节：「相关题目」（取自 PPT 与 Java 头注释）、「参考材料」（主参考固定「左程云 - 算法讲解NNN【难度】标题」→ github.com/algorithmzuo/algorithm-journey）。**没题目的方法论页只留参考材料**。
- **页面完全独立**：正文不写相邻算法链接 / 指代，页尾不插翻页标记（翻页由 shared.js 按 nav 铺平序生成）。
- 代码统一 C++（`bits/stdc++.h` + `using namespace std`），只贴关键函数不贴 main。正文不提 PPT / 课件，代码不提 Java，导航 / 标题不出现「模块」。
- 每页 ≥2 图、≥1 交互演示。默认内联 SVG（JS 生成后塞进 `<figure class="fig">` + 一句 `<figcaption>`），数组 / 表格 / 树可用 HTML+CSS 网格，不引位图。配色：主蓝 #2563eb（底 #dbeafe/#eef4ff）、次橙 #ea580c（底 #ffedd5）、灰 #9aa0a6、底 #f6f7f9、边框 #e6e8eb；SVG `<text>` ≥10px，中文按 12~13px/字估宽。画图时顺带核对正文结论，冲突改正文。

## 写作风格（去 AI 味，对齐 tiankonguse.github.io 的 OI 文章）
- 段落一到两句，用「可以发现 / 因此 / 具体来说 / 这里有个细节」推进。章节：零、背景 → 一、问题 → 二、朴素做法 → … → 复杂度分析 → 总结 → 最后。
- 引号用「」，数值范围写「0 到 10⁹」；少用 `<strong>`，不做总结式清单。禁 AI 模板句（「矛盾在哪？」「想一想：」「关键提醒」「核心思想」）、emoji、破折号滥用、通篇加粗。
- **「不是 X，是 Y」否定并列是最隐蔽的 AI 句式**：收口前 `grep -n '不是\|而是\|并非'` 全篇扫；技术对比留着，修辞强调一律删。
- 结尾「总结」段：具体要点 + 一句前瞻，禁空谈升华。范本：ST-RMQ / DFS序 / LCA / 虚树（要点在技能 SKILL.md「范本对齐」块，原文在技能 `references/`）。
- 杂字扫描放过 `√ ² Σ → ≤` 与「」，但**能用文字写就别用符号**（`Σ(1/r)` → 「1/51 一直到 1/100 加起来」）。

## 视觉与目录
- 纯静态（只有一步 `tools/build_nav.py`）：`index.html` + `catalog.html` + `algorithms/*.html`（平铺，不进子目录）+ `nav/` + `assets/{css/shared.css, js/shared.js, data/algorithms.json}`。本地预览 `./run.sh`（8080）；`file://` 直开会被 CORS 拦。
- **部署不在仓库里**：grep `rsync|deploy|lighthouse|scp` 零命中，remote 只是 GitHub 备份。上线走 WorkBuddy「发布为应用」，**打包本地工作目录**发到 `https://algorithms-course.app.workbuddy.host/`（链接与 sandboxId 固定），不是 git clone。所以**发布前必须先跑生成器**（线上 json 是静态文件）；`nav/` 与 `tools/` 一起传上去无害。发布后冒烟：curl 线上 json 数一遍页数/组数并与 `==` 比对本地生成物；**再抓一次线上首页 DOM**（`--headless=new --virtual-time-budget=8000 --dump-dom <url>` + 正则取 `statTotal/statL1/statL2` 与 `undefined` 计数），能抓到「json 对了但 JS 没跑起来」。发布前先把 `assets/data/` 下的 *.bak* 移走（会被一起打包）。轻量云 82.156.9.205 那条路因 CAM 缺 `tat:RunCommand`/`DescribeFirewallRules` 一直没通。
- **入口两页**：`index.html` 只回答「有什么、怎么读」（Hero + 统计 + 三条路线 + 难度 SVG），**不放目录列表 / 分类卡 / 搜索**；`catalog.html` 按一级铺目录（`#homeNav` 三级目录 + 搜索 + `#levelChips` + 卡片）。两页都带 `body.page-index`；数字与难度条由 shared.js 实时算，**不许写死**，加页后同步 meta description 与兜底值。
- **`#catNav` 按页面类型分流**：`.cat-tab` href 一律 `catalog.html#cat-<一级名>`；有 `#index-list` → 换正文，有 `#sidebar` → 换侧栏，都没有（总览页）→ 跳转。**别用文件名判断页面类型**。
- 骨架固定（`.layout` + `aside#sidebar` + `main .algo-section` + `aside.toc-panel ul#toc`），侧栏 / 目录 / 翻页 / 复制 / 搜索 / 进度条全由 shared.js 注入，**页面不自带骨架**。shared.css 只定义一份 `.fig` / `.legend` / `.dot` / `.breakdown` / `pre.code .cm|.kw|.fn` / `table.complexity`；保留 `[hidden]{display:none!important}`。
- **顺序 = 目录页分区 = 侧栏 = 上下篇翻页**（categories → groups.cat → 组内原序铺平）；侧栏高亮比 basename；磁盘文件名用连字符。页底**一个** `nav.pager` 含两个 `.pager-item` 并排（单侧时单卡片靠右）。顶部 sticky `#catNav`，`.sidebar`/`.toc-panel` 的 sticky top 用 `--navh` 让位（≤1180 时 98px）。

## 编排
- **从小到大转（001 → 207）**；159~207 是倒序阶段产物**不回炉**，下一课号以 `prompt.md` 为准。
- **转一课不开子智能体**；「连转 N 课」串行，一课收口干净（nav 条目 + 生成物 + prompt.md）再开下一课。
- **导学课不产页**（PPT 无方法 / `src/classNNN/` 无代码）→ 记 prompt.md「跳过」写原因；**方法论课要产页**（先例 005/007/019/020/026/029/106/107）。
- **跨会话抢锁防重**：开工第一步抢 `/tmp/algo-course-lock-<课号>`。**凭据是 lockdir 里的 `owner` 文件、不是目录**：`mkdir -p` 建容器，抢锁 = noclobber 下 `printf > owner`；抢不到且 owner ≤60 分钟就退出，死锁只看 `find owner -mmin +60`；收口只 `rm -f owner`，**别 rmdir**。
- **「下一课」与导航末条对不上 = 可能有失联会话已做完**：`grep -r '<该课页面名>.html' nav/*/*/index.json` + 看页面 mtime；页面与 nav 已落盘只差 prompt.md → 按死锁规则清 owner 重抢锁，**只补记进度、不重写页面**。
- **撞已有主题就合并**：开工先扫 `nav/*/index.json` 的 `name` 与各组 `title`。新二级 append 进对应一级（`{dir,name,desc}`），组内按课号升序。每课通常拆 2~4 页，按「技巧同源」合并。

## 转课流水线（技能 `course-site-page`）
- 站点根 `~/project/github/AIProject/algorithms-course`，课件根 `~/.../algorithm-journey`。**用户目录一律写 `~/`**，禁写 `/Users/<用户名>/...`；node 用 `~/.workbuddy/binaries/node/versions/$(cat .../current)/bin/node`。
- 材料：`ppt/算法讲解0NN【难度】标题.pptx`（zipfile + ElementTree 遍历 `ppt/slides/slideN.xml` 拼 `a:t`）+ `src/classNNN/`。**低号课只有一份 Java**（文件名是题目名、无 `Code0X_`、无 C++ 版），要自己改写成 C++ 再对拍；**中高号偶数 java 里藏着 C++**：`awk '/^\/\/#include/{f=1} f&&/^\/\//{sub(/^\/\//,"");print;next} f{print}' X.java > X.cpp`。
- 拼页先 Write 带 `@@片段名@@` 的占位版，再用 `apply_placeholders.py` 替换（占位名 = 片段文件名，只含 `[A-Za-z0-9_]`，**占位符别套 `<pre class="code">`**，要单独一行）。省上下文：PPT 抽到 `/tmp/pptNNN.txt` 后按段取，**别把新页面 HTML 读回来**。
- **技能装两个物理副本**：仓库 `.agent/skills/course-site-page/` + 用户级 `~/.workbuddy/skills/course-site-page/`，改完必须 cp 同步 + `diff -rq` 确认，否则下次会话可能命中旧版。**`~/.agents/skills/course-site-page/` 不是第三份**：它与 `~/.workbuddy` 那份共享同一批 inode（文件级硬链接，一边改另一边立刻可见），所以只需同步一次。CDP 类脚本依赖同目录 `cdp_page.js`。

## 自检（全自动，别只靠截图）
- **「起 Chrome + 跑 node」写在同一条命令里**（Chrome 随 Bash 结束被回收）；端口读 `CDP_PORT`（默认 9333，换端口两处都改）；**并发会话别 `pkill` 别人的 Chrome**，用空闲端口 + 独立 `--user-data-dir`。
  ```bash
  "$CHROME" --headless=new --no-sandbox --disable-gpu --remote-debugging-port=9333 \
    --user-data-dir=/tmp/cdpX --allow-file-access-from-files about:blank >/dev/null 2>&1 &
  sleep 8
  $N $SKILL/verify_pages.js a.html b.html   # JS 错误 / fig==svg / nav·toc·viz / svg 越界 / pager 链
  $N $SKILL/verify_viz_steps.js a.html      # 逐帧点（上限 40 帧，跑满 ≠ 失败）
  $N $SKILL/verify_home.js                  # 入口两页（index + catalog）
  ```
- 必查四项：① 无 JS 错误 ② `figure.fig` 数 == 其中 svg 数 ③ nav/toc/每个 `.viz` 有内容 ④ 每个 svg 用 `getBBox()` 比对 viewBox。`--allow-file-access-from-files` 必带，别加 `--window-size`。
- 页列表从 **nav** 生成（zsh 用 `${=PAGES}`，传裸文件名）：`PAGES=$(python3 -c "import json,glob;print(' '.join(i['file'] for p in sorted(glob.glob('nav/*/*/index.json')) for i in json.load(open(p, encoding='utf-8'))))")`；pager 链最灵敏（非末页 2、末页 1）。收口前跑杂字 / AI 味扫描 + `overlapNNN.js` 逐图比 `text` 的 `getBBox()`。
- **Chrome 155 起 `attachToTarget({flatten:true})` 失效**（脚本报全 0 或 `wasn't found`）：统一走技能里的 `cdp_page.js`（`PUT /json/new?<url>` 开页 → 连页面自己的 WS → `Page.enable/Runtime.enable` → evaluate）。自写脚本照抄它的顺序，检查项照抄 verify_pages 的 CHECK。
- **自检 PASS ≠ 演示能走到底**：脚本抓不到点击后的异常，末帧答案与按钮 disabled 要自写探针点到底。

## 踩过的坑

**演示与页面 JS**
- `verify_viz_steps.js` 的「下一步」按 `.viz-controls button` 兜底匹配，按钮语义特殊会点错，**用 id 显式点击**；控制按钮容器统一 `class="viz-controls"`，并同步 `disabled`（`nextBtn.disabled = pos >= cur.length - 1`），漏加会一直点到 40 帧上限。
- **`var el` 循环挂 handler 是雷**：闭包共享变量，`el.value` 恒为空串 → 演示整块空白。统一 `bind(id, ev, fn)` + `this.value`。按行替换易把 `if (el) …` 的 `)` 删掉留下 `};`（整段 SyntaxError），改完用 CDP 抓 `Runtime.exceptionThrown`。
- **别拿单个大写字母当维度变量**：`var W=660,H=300` 的 `H` 撞哈希函数 `H()` 抛 TypeError（图全空、自检照样 PASS）。画布高写 `HH`。
- 探针设下拉用 `selectedIndex`（`.value = String(下标)` 在 option value 非下标时赋成空串）；**要断言数值不只断言文本**；**别缓存会被 `innerHTML` 替换掉的节点**（每帧重新 `getElementById`）。「空 viz」判据按 `querySelectorAll('svg,.rowline,.cell,.pcell').length === 0`，别按 innerHTML 长度。
- **模运算用 BigInt**（`x*y%MOD` 且 MOD ≥ 1e6 越过 2^53 静默丢精度）；复现错法时赋值顺序跟源码一致；**演示随机数一律 mulberry32**（经典 LCG 在双精度丢低位）。演示要展示某结论时先用 Node 跑同款随机流遍历种子，挑出现象齐全的那个。
- `rebuild()` 里**不要写 `st.step = st.steps.length - 1`**（切 select 会把演示停末帧）；自写模拟器要带保护（取不到节点置 `broken`）。

**SVG 画图**
- **逐图检查必须覆盖每个预设**（长度可变时最后一格冲出 viewBox）；兜底 `cw = N > 8 ? Math.floor((664 - 100) / N) : CW`。
- **绘图 helper 别写成「吃进累积串再返回」**：`g += axis(g, …)` 会把累积内容整段复制，每个 `text` 多一个孪生兄弟（肉眼看不出，只有逐图比 `getBBox()` 报 100% 自重叠）。helper 只返回自己片段，调用点保持 `+=`。
- 标题与紧邻标签行距按 bbox 量（SVG text 的 bbox 比字面高得多，标题 `y=20` 与 `y=30` 会真叠 3~5px）；整块平移时画布高度从同一变量算。
- **画出来的示例数据要和图上结论逐格核**；**0 起数组喂给 1 起的 helper 是下标最容易漏的错**。改格子间距必须同步框宽（缝留 16~20px，高度重算）；对数轴刻度别用固定长度上标数组；标签溢出/重叠逐图比 `getBBox()`。

**代码与对拍**
- **课程代码 ≠ 题面直觉**：先用随机小用例确认代码真实语义再写暴力，别改代码迁就直觉；强制在线题要交互编码。
- 本机**没有 `bits/stdc++.h`**：自建 shim `/tmp/cppNNN/inc/bits/stdc++.h`，编译加 `-I inc -std=c++17`；对拍看 `grep -E 'OK|MISMATCH'`。
- **先怀疑数据，再怀疑代码**：树状数组上界写成 `n` 的题数据限 ≤ n；缩字母表时字母表常数一起改；连续覆盖题合并区间看能否接上；DAG 最长路暴力必须按拓扑序扫；对拍前核输入合法性。
- **C++ 排序比较符必须严格弱序**：Java 的 `<=` 抄成 `std::sort` 在相等键双向 true，行为未定义（实测段错误），一律换 `<`/`>`。
- **暴力要照顾「没被处理的元素」**；错法抓取率随值域变化方向不统一（有的小值域才现形，有的越大越易抓）；**「看着像错其实等价」要专门量一遍，0.00% 也是结论**（与真错法分开写）；抓取率分母跟着生成器改；统计量给分布不只给均值。
- **性能数字跑热再取**（首次触碰大数组是冷启动值）；**连同 `-O0` 一起量**；常量指数会被折叠，换变量指数才量得准。

**流程与文件**
- 开工先探残留：`ls -dt /tmp/cppNNN /tmp/hlNNN` + `find algorithms -newermt '<半小时前>'`；**复用残留前先核对它自写的测量脚本**（计数器一一对应、`printf` 标签与函数语义一致）。
- 写 nav 的 `index.json` 前 `cp` 备份 + 重读确认；**别按位置切片替换数组**，加新条目用 `splice`；**别用 `git diff HEAD` 核改动**（生成物整份重写会报上千行假改动），用 `diff` 对比写前备份。
- 就地改写页面 HTML 别用 `s.replace(s[idx(a):idx(b)], new)`（b 在 a 前切出空串，`str.replace('', new)` 会把新块插进每两个字符之间，59KB 变 80MB）；写 `i = s.index(a); j = s.index(b, i); assert i < j`，替换完 `wc -c` 查体积。python 替换锚点截断会残留旧句，替换后整行看一遍「下一课」。
- 大量条目变更时**生成物单独一笔提交**（数组重排几百行位移，混进来会让 `git log -p` / bisect 失效）。
