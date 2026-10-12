# 项目约定（algorithms-course）

> 只放**规则**。进度 / 下一课看 `prompt.md`，目录与页清单看 `assets/data/algorithms.json`。

## 目录结构（三层，参考 OI-wiki）
- json 三键：`categories`（一级有序数组）+ `groups`（二级 `{name, cat, desc}`）+ `algorithms`（页面，`group` = 二级名）。**第三层就是页面**。
- 一级：语言基础 / 算法基础 / 搜索 / 动态规划 / 字符串 / 数学 / 数据结构 / 图论 / 计算几何 / 杂项 / 专题（无页面的不上顶栏）。二级名**全局唯一**。
- 加页：条目 append 进 `algorithms`，六字段 `id`/`file`/`title`/`description`/`group`/`level`；目标二级已存在就不动 `groups`/`categories`。
- UI：顶栏一级（带篇数）；左侧二级可展开出页面列表，**默认全折叠**、只展开当前页那组；展开状态记 `localStorage['algoNavOpenSubs']`（键 `一级>二级`），搜索命中临时全展开。

## 内容
- 每页必含「复杂度分析」（`table.complexity` + 空间说明，明确 O(...)）。
- 「总结」后固定两节：「相关题目」（取自 PPT 与 Java 头注释）、「参考材料」（主参考固定「左程云 - 算法讲解NNN【难度】标题」→ github.com/algorithmzuo/algorithm-journey）。**没有题目的方法论页只留参考材料**。
- **页面完全独立**（用户定）：正文不写相邻算法链接 / 指代，页尾不插翻页标记（翻页由 shared.js 按 json 顺序生成）。
- 代码统一 C++（`bits/stdc++.h` + `using namespace std`），只贴关键函数不贴 main。正文不提 PPT / 课件，代码不提 Java，导航 / 标题不出现「模块」。
- 每页 ≥2 图、≥1 交互演示。默认内联 SVG（JS 生成后塞进 `<figure class="fig">` + 一句 `<figcaption>`），数组 / 表格 / 树可用 HTML+CSS 网格，不引位图。配色：主蓝 #2563eb（底 #dbeafe/#eef4ff）、次橙 #ea580c（底 #ffedd5）、灰 #9aa0a6、底 #f6f7f9、边框 #e6e8eb；SVG `<text>` ≥10px，中文按 12~13px/字估宽。画图时顺带核对正文结论，冲突改正文。

## 写作风格（去 AI 味，对齐 tiankonguse.github.io 的 OI 文章）
- 段落一到两句，用「可以发现 / 因此 / 具体来说 / 这里有个细节」推进。章节：零、背景 → 一、问题 → 二、朴素做法 → … → 复杂度分析 → 总结 → 最后。
- 引号用「」，数值范围写「0 到 10⁹」；少用 `<strong>`，不做总结式清单。禁 AI 模板句（「矛盾在哪？」「想一想：」「关键提醒」「核心思想」）、emoji、破折号滥用、通篇加粗，强调靠推导。
- **「不是 X，是 Y」否定并列是最隐蔽的 AI 句式**：收口前 `grep -n '不是\|而是\|并非'` 全篇扫；技术对比（两个具体选项挑一个）留着，修辞强调一律删。
- 结尾「总结」段：具体要点 + 一句前瞻，禁空谈升华（「标志着 / 意味着」）。范本：ST-RMQ / DFS序 / LCA / 虚树（要点在技能 SKILL.md「范本对齐」块，原文与去 AI 味清单在技能 `references/`）。
- 杂字扫描放过 `√ ² Σ → ≤` 与「」，但**能用文字写就别用符号**：`Σ(1/r)` → 「1/51 一直到 1/100 加起来」，`Anna Gál` → `Anna Gal`。

## 视觉与目录
- 纯静态无构建：`index.html`（总览）+ `catalog.html`（目录）+ `algorithms/*.html` + `assets/{css/shared.css, js/shared.js, data/algorithms.json}`。
- **入口两页**：`index.html` 只回答「有什么、怎么读」（Hero + 全站统计 + 三条路线 + 难度 SVG），**不放目录列表 / 分类卡 / 搜索**；`catalog.html` 是「按一级铺目录」页（`#homeNav` 三级目录 + 搜索 + `#levelChips` + 卡片）。两页都带 `body.page-index`（css 大量依赖）。两页数字与难度条由 shared.js 从 json 实时算，**不许写死**；加页后同步 meta description 与兜底值。
- **`#catNav` 按页面类型分流**：`.cat-tab` 的 href 一律 `catalog.html#cat-<一级名>`；有 `#index-list` → 原地换正文，有 `#sidebar` → 原地换侧栏，都没有（总览页）→ 走 href 跳转。**别用文件名判断页面类型**。
- 骨架固定（`.layout` + `aside#sidebar` + `main .algo-section` + `aside.toc-panel ul#toc`），侧栏 / 目录 / 翻页 / 复制 / 搜索 / 进度条全由 shared.js 注入，**页面不自带骨架**；独有样式留在各页 `<style>` / `<script>`。
- shared.css 只定义一份并各行复用：`.fig` / `.fig svg` / `.legend` / `.dot` / `.breakdown` / `pre.code .cm|.kw|.fn` / `table.complexity`；保留 `[hidden]{display:none!important}`。
- **顺序 = 目录页分区 = 侧栏 = 上下篇翻页**（categories → groups.cat → algorithms 原顺序铺平）；`file` 写 `algorithms/xxx.html`，侧栏高亮比 basename；`id` 可用下划线，磁盘文件名用连字符。
- 侧栏三级折叠：打开算法页只展开当前二级组、高亮项滚到可视区正中。
- 页底**一个** `nav.pager` 含两个 `.pager-item` 并排（单侧时单卡片靠右），别写成两个 nav。`catalog.html` 的 `#homeNav` 由 shared.js 渲染二级树（`#group-<i>` 锚点 + scrollspy），`homeNav + home-main` flex，900px 以下隐藏；`index.html` 没有 `#homeNav`。算法页顶部 sticky `#catNav`，`.sidebar` / `.toc-panel` 的 sticky top 用 `--navh` 让位（≤1180 时 98px）。
- 本地预览 `./run.sh`（8080）；`file://` 直开会被 CORS 拦。

## 编排
- **从小到大转（001 → 207）**；159~207 是倒序阶段产物**不回炉**，下一课号以 `prompt.md` 为准。
- **转一课不开子智能体**：主循环直接做；「连转 N 课」串行，一课收口干净（json + prompt.md 都更新）再开下一课。
- **导学课不产页**（PPT 无方法 / `src/classNNN/` 无代码）→ 记 prompt.md「跳过」写原因；**方法论课要产页**（先例：005 / 007 / 019 / 020 / 026 / 029 / 106 哈希四页 / 107 五页）。
- **跨会话抢锁防重**：开工第一步抢 `/tmp/algo-course-lock-<课号>`。**凭据是 lockdir 里的 `owner` 文件、不是目录**：`mkdir -p` 建容器，抢锁 = noclobber 下 `printf > owner`；抢不到且 owner ≤60 分钟就退出，死锁只看 `find owner -mmin +60`；收口只 `rm -f owner`，**别 rmdir**。
- **撞已有主题就合并**：开工先扫 json 的 `name` 与 `title`。新二级插在对应一级的末尾或主题相邻处（`cat` 必填），组内按课号升序；撞主题就并入已有组或给已有页补一节，不新建重复页。每课通常拆 2~4 页，按「技巧同源」合并。

## 转课流水线（技能 `course-site-page`）
- 站点根 `~/project/github/AIProject/algorithms-course`，课件根 `~/.../algorithm-journey`。**用户目录一律写 `~/`**（脚本默认 `SITE = (process.env.SITE_DIR || '~/project/...').replace(/^~(?=\/|$)/, os.homedir())`），**禁写 `/Users/<用户名>/...`**；node 用 `~/.workbuddy/binaries/node/versions/$(cat .../current)/bin/node`。
- 材料：`ppt/算法讲解0NN【难度】标题.pptx`（zipfile + ElementTree 遍历 `ppt/slides/slideN.xml` 拼 `a:t`）+ `src/classNNN/`。**低号课只有一份 Java**（文件名是题目名、无 `Code0X_`、无 C++ 版），页面要自己改写成 C++ 再编译对拍；**中高号偶数 java 里藏着 C++**：`awk '/^\/\/#include/{f=1} f&&/^\/\//{sub(/^\/\//,"");print;next} f{print}' X.java > X.cpp`。
- 拼页先 Write 带 `@@片段名@@` 的占位版，再用 `apply_placeholders.py` 替换（占位名 = 片段文件名）。省上下文：PPT 抽到 `/tmp/pptNNN.txt` 后按段取，**别把新页面 HTML 读回来**。

## 自检（全自动，别只靠截图）
- **「起 Chrome + 跑 node」写在同一条命令里**（Chrome 随 Bash 结束被回收）；端口读 `CDP_PORT`（默认 9333，换端口两处都要改），**并发会话别 `pkill` 别人的 Chrome**，用空闲端口 + 独立 `--user-data-dir`。
  ```bash
  "$CHROME" --headless=new --no-sandbox --disable-gpu --remote-debugging-port=9333 \
    --user-data-dir=/tmp/cdpX --allow-file-access-from-files about:blank >/dev/null 2>&1 &
  sleep 8
  $N $SKILL/verify_pages.js a.html b.html   # JS 错误 / fig==svg / nav·toc·viz / svg 越界 / pager 链
  $N $SKILL/verify_viz_steps.js a.html      # 逐帧点（上限 40 帧，跑满 ≠ 失败）
  $N $SKILL/verify_home.js                  # 入口两页
  ```
- 必查四项：① 无 JS 错误 ② `figure.fig` 数 == 其中 svg 数 ③ nav/toc/每个 `.viz` 有内容 ④ 每个 svg 用 `getBBox()` 比对 viewBox。`--allow-file-access-from-files` 必带，别加 `--window-size`（隐藏图标 svg 会被判空图）。
- 页列表从 json 生成（zsh 用 `${=PAGES}`，传裸文件名）；pager 链最灵敏（非末页 2、末页 1，标题与铺平后的 json 相邻项一致）。收口前跑杂字 / AI 味扫描 + `overlapNNN.js` 逐图比 `text` 的 `getBBox()`。
- 自写 CDP 脚本要 `Target.createTarget` + `attachToTarget({flatten:true})` 拿 `sessionId`（直接在浏览器端点发 `Runtime.enable` 报 `wasn't found`），之后每条 `send` 都带上它。

## 踩过的坑
- **自检 PASS ≠ 演示能走到底**：`verify_pages.js` 抓不到点击后抛的异常，末帧答案与按钮 disabled 要自写探针点到底；`verify_viz_steps.js` 的「下一步」按 `.viz-controls button` 兜底匹配，按钮语义特殊时会点错，**用 id 显式点击**。
- **探针设下拉用 `selectedIndex`**（`.value = String(下标)` 在 option value 非纯下标时赋成空串，多组组合实际跑成同一分支）；**要断言数值不只断言文本**（107 的 SpaceSaving 口径写反就是这么漏的）。
- **`verify_home.js` 有版本偏差**：仓库 `.agent/skills/course-site-page/scripts/` 那份是旧快照（报 `tabs` undefined），两入口页版在用户级 `~/.workbuddy/skills/course-site-page/scripts/`。
- **页面 JS 别拿单个大写字母当维度变量**：`var W=660,H=300` 的 `H` 撞哈希函数 `H()` 会抛 `TypeError`（图全空、`verify_pages` 照样 PASS）。画布高写 `HH`。
- **改格子间距必须同步框宽**（箭头画缝里时缝留 16~20px，画布高度重算）。对数轴刻度别用固定长度上标数组（渲染出字面量 `10undefined`）。
- **演示里的模运算用 BigInt**（`x * y % MOD` 且 MOD ≥ 1e6 时越过 2^53 静默丢精度）；**复现错法时赋值顺序要跟源码一致**（否则错法复现不出来）。
- **控制按钮容器统一 `class="viz-controls"`**（`shot_element.js` 按它索引点击）；纯 HTML 演示的标签溢出 / 重叠要逐图比 `getBBox()`。
- **课程代码 ≠ 题面直觉**：先随机小用例确认代码真实语义再写暴力，别改代码迁就直觉；强制在线题要交互编码（暴力输出用 `endl`）。
- 本机**没有 `bits/stdc++.h`**：自建 shim `/tmp/cppNNN/inc/bits/stdc++.h`，编译加 `-I inc -std=c++17`；对拍看 `grep -E 'OK|MISMATCH'`。
- 缩字母表做对数器时字母表常数要一起改；树状数组上界写成 `n` 的题随机数据要限 ≤ n，否则假 MISMATCH，**先怀疑数据再怀疑代码**；连续覆盖类题要合并区间看能否接上。
- **C++ 排序比较符必须是严格弱序**：Java 的 `<=` 比较器抄成 `std::sort` 在相等键时两个方向都返回 true，行为未定义（实测段错误）。一律换 `<` / `>`。
- **暴力要照顾「没被处理的元素」**：只检查主动处理的那些会漏掉「没排上号的东西自己走到坏状态」，答案算得偏大。
- **复用中断残留前先核对它自写的测量脚本**（计数器是否一一对应、`printf` 标签与函数语义是否一致）；**「看着像错其实等价」要专门量一遍，0.00% 也是结论**（要与真错法分开写）。
- **空间数字按题目上限算，不是按测试用的 n 算**（`char ss[MAXN<<1]` 配 `MAXN = 11000001` 是 22 MB）。
- **方向性结论必须实测钉死**（107）：SpaceSaving **偏大**（Misra-Gries 才偏小）、上界 N/k、最大高估量 = 表里最小计数器；「估计 / 真实」这种混合口径的列要么统一口径要么拆成召回 / 精度两列。`ts << 64` 是 UB；宏 `#define data gData` 会误伤 `buf.data()`。
- **统计量要给分布不只要给均值**（107）：标准差用二项式 √(np(1−p))，别用泊松近似。
- **开工先探残留**：`ls -dt /tmp/cppNNN /tmp/hlNNN` + `find algorithms -newermt '<半小时前>'`；写 json 前 `cp` 备份 + 重读确认。
- **「下一课」与 json 末条对不上 = 可能已有失联会话做完**：查该课 id 与页面 mtime；页面与 json 已落盘、只差 prompt.md 的 → 按死锁规则清 owner 重抢锁，**只补记进度、不重写页面**。
