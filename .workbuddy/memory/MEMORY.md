# 项目约定（algorithms-course）

> 只放**规则**。进度 / 下一课看 `prompt.md`，目录与页清单看 `nav/`（生成物 `assets/data/algorithms.json` 只给浏览器读）。更细的坑清单在技能 `course-site-page` 里。

## 导航：nav/ 目录树 + 启动生成
- **真值是 `nav/` 三层目录树，每层只管自己这层**：`nav/index.json` 一级 → `nav/<cat>/index.json` 二级 → `nav/<cat>/<group>/index.json` 页面（有序 `[{file,title,description,level}]`）。目录名英文 slug，中文名与备注写在**父层** json，不存两份。`file` 只写文件名，生成器拼成 `algorithms/<file>`。
- 一级 11 个：`lang/basics/search/dp/string/math/ds/graph/geometry/misc/topics`（无页面的不上顶栏）。二级名与 slug 都**全局唯一**。
- **加页** = 往所属组 `index.json` 插一条四字段（level **数字** 1~5）+ `python3 tools/build_nav.py` 重新生成 + 生成物一起提交。**开新组先写父层 `index.json` 再建目录**。忘了写 nav 不会静默丢——下次生成报「孤儿页」。
- `assets/data/algorithms.json` 是**生成物，禁止手改**。参数 `--check` / `--strict` / `--stats` / `--verify-against <旧 json>`。13 条硬错（缺多字段、slug 不合规、`file` 不存在、**大小写不一致**、重复收录、level 非 1~5）+ 孤儿页 / 野目录告警。
- 生成物**已无 `id`**（shared.js 翻页用 `file`）。按组输出 → `algorithms` 数组**必然重排**，**定位按 `(group, file)` 别按下标**；`flat` 铺平序与组内相对序一字不差。
- 迁移脚本 `tools/migrate_json_to_nav.py` 是从旧格式恢复的唯一路径，别删。
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
- 结尾「总结」段：具体要点 + 一句前瞻，禁空谈升华。范本：ST-RMQ / DFS序 / LCA / 虚树（要点在技能 SKILL.md「范本对齐」块）。
- 杂字扫描放过 `√ ² Σ → ≤` 与「」，但**能用文字写就别用符号**。

## 视觉与目录
- 纯静态（只有一步 `tools/build_nav.py`）：`index.html` + `catalog.html` + `algorithms/*.html`（平铺）+ `nav/` + `assets/{css/shared.css, js/shared.js, data/algorithms.json}`。本地预览 `./run.sh`（8080）；`file://` 直开会被 CORS 拦。
- **部署不在仓库里**，remote 只是 GitHub 备份。上线走 WorkBuddy「发布为应用」，**打包本地工作目录**发到 `https://algorithms-course.app.workbuddy.host/`。**发布前必须先跑生成器**；发布后冒烟：curl 线上 json 比对本地，再抓一次线上首页 DOM 取 `statTotal/statL1/statL2` 与 `undefined` 计数。发布前移走 `assets/data/` 下的 *.bak*。轻量云 82.156.9.205 那条路因 CAM 缺 `tat:RunCommand`/`DescribeFirewallRules` 没通。
- **入口两页**：`index.html` 只回答「有什么、怎么读」（不放目录列表 / 分类卡 / 搜索）；`catalog.html` 按一级铺目录。两页都带 `body.page-index`；数字与难度条由 shared.js 实时算，**不许写死**，加页后同步 meta description 与兜底值。**同一批兜底值在 index.html 里出现两处**（hero 的 `#statCount/#statL1/#statL2` 与 `.badge-pill` 的 `#statTotal/#statGroups/#statGroupCount`），改一个漏一个就两边不一致；`levelChartNote` 的「高阶及以上」是 `level >= 3` 的条数，不是 4 以上。
- **`#catNav` 按页面类型分流**：`.cat-tab` href 一律 `catalog.html#cat-<一级名>`；有 `#index-list` → 换正文，有 `#sidebar` → 换侧栏，都没有 → 跳转。**别用文件名判断页面类型**。
- 骨架固定（`.layout` + `aside#sidebar` + `main .algo-section` + `aside.toc-panel ul#toc`），侧栏 / 目录 / 翻页 / 复制 / 搜索 / 进度条全由 shared.js 注入，**页面不自带骨架**。shared.css 只定义一份 `.fig` / `.legend` / `.dot` / `.breakdown` / `pre.code .cm|.kw|.fn` / `table.complexity`；保留 `[hidden]{display:none!important}`。
- **顺序 = 目录页分区 = 侧栏 = 上下篇翻页**（categories → groups.cat → 组内原序铺平）；侧栏高亮比 basename；磁盘文件名用连字符。页底**一个** `nav.pager` 含两个 `.pager-item`（单侧时单卡片靠右）。顶部 sticky `#catNav`，`.sidebar`/`.toc-panel` 的 sticky top 用 `--navh` 让位（≤1180 时 98px）。

## 编排
- **从小到大转（001 → 207）**；159~207 是倒序阶段产物**不回炉**，下一课号以 `prompt.md` 为准。
- **prompt.md 只放进度指针**（下一课 / 跳过 / 已完成）；每课详细记录（拆页、对拍、踩坑）按课号存 `progress/NNN.md`，参考先例按课号读。
- **转一课不开子智能体**；连转 N 课串行，一课收口干净（nav 条目 + 生成物 + prompt.md）再开下一课。
- **导学课不产页**（PPT 无方法 / `src/classNNN/` 无代码）→ 记 prompt.md「跳过」写原因；**方法论课要产页**（先例 005/007/019/020/026/029/106/107）。
- **跨会话抢锁**：开工第一步抢 `/tmp/algo-course-lock-<课号>`。**凭据是 lockdir 里的 `owner` 文件、不是目录**：`mkdir -p` 建容器，抢锁 = noclobber 下 `printf > owner`；抢不到且 owner ≤60 分钟就退出，死锁只看 `find owner -mmin +60`；收口只 `rm -f owner`，**别 rmdir**。
- **「下一课」与导航末条对不上 = 可能有失联会话已做完**：`grep -r '<课页面名>.html' nav/*/*/index.json` + 看页面 mtime；页面与 nav 已落盘只差 prompt.md → 清 owner 重抢锁，**只补记进度、不重写页面**。
- **撞已有主题就合并**：开工先扫 `nav/*/index.json` 的 `name` 与各组 `title`。新二级 append 进对应一级，组内按课号升序。每课通常拆 2~4 页，按「技巧同源」合并。

## 转课流水线（技能 `course-site-page`）
- 站点根 `~/project/github/AIProject/algorithms-course`，课件根 `~/.../algorithm-journey`。**用户目录一律写 `~/`**，禁写 `/Users/<用户名>/...`；node 用 `~/.workbuddy/binaries/node/versions/$(cat .../current)/bin/node`。
- 材料：`ppt/算法讲解0NN【难度】标题.pptx`（zipfile + ElementTree 遍历 `ppt/slides/slideN.xml` 拼 `a:t`）+ `src/classNNN/`。**低号课只有一份 Java**（无 `Code0X_`、无 C++ 版），要自己改写成 C++ 再对拍；**中高号偶数 java 里藏着 C++**：`awk '/^\/\/#include/{f=1} f&&/^\/\//{sub(/^\/\//,"");print;next} f{print}' X.java > X.cpp`。
- 拼页先 Write 带 `@@片段名@@` 的占位版，再用 `apply_placeholders.py` 替换（占位名 = 片段文件名，只含 `[A-Za-z0-9_]`，**占位符别套 `<pre class="code">`**，要单独一行）。省上下文：PPT 抽到 `/tmp/pptNNN.txt` 后按段取，**别把新页面 HTML 读回来**。
- **技能只有一份且自包含（2026-10-14 收敛）**：唯一副本是仓库 `.agent/skills/course-site-page/`，**不依赖项目外的任何技能**：截图/越界用自带 `scripts/shot_*.js` + `verify_pages.js`（不再调 `ui-verify-cdp`），去 AI 味规则已内联为 `references/humanize.md`（不再调 `去AI味`/`humanizer`）；脚本只用 node 内置模块 + 同目录 `cdp_page.js`。**改完不用 cp 同步**（旧规矩作废：`~/.workbuddy/skills/` 那份已不存在，`~/.agents/skills/course-site-page/` 已删除，备份 `/tmp/course-site-page-home-backup-20261014.tar.gz`）。`grep -c 'tools/build_nav.py' SKILL.md` 得 8 处 = 当前版。CDP 类脚本依赖同目录 `cdp_page.js`，别单搬一个文件用。

## 材料归档（每课一目录 + 页面入口，2026-10-19 用户定）
- **中间产物归 `artifacts/<课号>/`**：PPT 抽取文本（ppt.txt）、Java→C++ 转换与对拍器（code/，白名单 `*.cpp/*.cc/*.c/*.h/*.hpp/*.py` + `paipai*.out|log`；**随机测试数据 .in/.want/io*.txt、.bin 与 ELF/Mach-O 一律不入库**）。收口必跑 `python3 tools/archive_course.py <课号> <页名...>`，产物 `artifacts/<课号>/{index.json,index.html}` + 生成物 `assets/data/artifacts.json` 随提交入库。
- **页面入口零标记**：shared.js 按 `assets/data/artifacts.json` 命中当前页，在 pager 前注入「本课材料」链接；无归档静默。改 shared.js 后跑 verify_pages 确认无 JS 错。`run.sh` 启动先 build_nav.py 再 build_artifacts.py。
- **回填**：`tools/archive_course.py --backfill` 从 git 提交反查页名（兼容「转换第 N 课 / 转换N课 / N课 / N 标题」四种消息），跳过持锁课。已回填 131~170 共 40 课 126 页；003~130 的 /tmp 产物已清无法回填。

## 自检（全自动，别只靠截图）
- **「起 Chrome + 跑 node」写在同一条命令里**（Chrome 随 Bash 结束被回收）；端口读 `CDP_PORT`（默认 9333）；**并发会话别 `pkill` 别人的 Chrome**，用空闲端口 + 独立 `--user-data-dir`。`--allow-file-access-from-files` 必带，别加 `--window-size`。
  ```bash
  "$CHROME" --headless=new --no-sandbox --disable-gpu --remote-debugging-port=9333 \
    --user-data-dir=/tmp/cdpX --allow-file-access-from-files about:blank >/dev/null 2>&1 &
  sleep 8
  $N $SKILL/scripts/verify_pages.js a.html b.html      # JS 错 / fig==svg / nav·toc·viz / svg 越界 / pager 链
  $N $SKILL/scripts/verify_viz_steps.js a.html b.html  # 逐帧点到按钮 disabled（上限 40 帧，跑满 ≠ 失败）
  $N $SKILL/scripts/verify_home.js                     # 入口两页（index + catalog）
  ```
- 必查四项：① 无 JS 错误 ② `figure.fig` 数 == 其中 svg 数 ③ nav/toc/每个 `.viz` 有内容 ④ 每个 svg 用 `getBBox()` 比对 viewBox。pager 链最灵敏（非末页 2、末页 1）。收口前跑杂字 / AI 味扫描 + 逐图比 `text` 的 `getBBox()`。**两扫描已抽成脚本**（2026-10-14），别再手敲内联 python：`python3 $SKILL/scripts/scan_typos.py [页...]`（杂字 / 占位符残留 / 代码块真嵌套）、`python3 $SKILL/scripts/scan_ai_style.py [页...]`（套话 / 否定并列 / 破折号 / strong / emoji），不带参数扫全站。
- 页列表从 **nav** 生成（zsh 用 `${=PAGES}`）：`PAGES=$(python3 -c "import json,glob;print(' '.join(i['file'] for p in sorted(glob.glob('nav/*/*/index.json')) for i in json.load(open(p, encoding='utf-8'))))")`
- **三个脚本的参数都只传文件名**，它们自己拼 `algorithms/`。
- **「全 0 / no viz」几乎都是路径拼错**（2026-10-14 定论）：传 `algorithms/x.html` 会拼成 `algorithms/algorithms/x.html`，Chrome 开的是自己的「无法访问您的文件」报错页——正文 41KB、0 个 `figure`、0 个 `.viz`、还不抛 JS 错，于是每项检查都静默通过只剩全 0。两个脚本已加参数归一化 + `chrome-error` 显式报 FAIL；再看到 0/0，先读那行报错，别去怀疑脚本。
- **CDP 开页在 Chrome 155 上两条路都通**：① 浏览器级 WS 的 `Target.createTarget` + `attachToTarget({flatten:true})`；② 页面级 WS（`PUT /json/new?<url>` 再连页面自己的 WS，技能里封装成 `cdp_page.js`）。报 `'Xxx.enable' wasn't found` 先查自己**漏没漏传 sessionId**，别急着判 Chrome 不支持。
- **等导航落地要等「location 真的换掉」**：about:blank 一上来 readyState 就是 complete，只看 complete 会在导航提交前退出、把 CHECK 打在空白页（两个脚本已改成 readyState + href 双条件）。
- **演示的答案断言要「独立算」**，别拿页面自己的数据表当期望：118 演示一同时写了 `PAR`（给跳表用）和 `EDG`（给画图用），两处对不上而页内自洽——探针从 `EDG` 反推父亲，143 组合立刻全部现形。
- **自检 PASS ≠ 演示能走到底**：末帧答案与按钮 disabled 要自写探针点到底。
- **耗时数字不能在 load 高时重测**：多个 headless Chrome 留在后台时 load 能到 65，同一份代码连跑三次读数差 3~10 倍。判定改用「正文与图互相自洽 + 定性结论稳定」，别拿噪声覆盖站上已有的数字。
- **收工时顺手清掉自己起的 Chrome**：实测泄漏过 213 个主进程 + `/tmp/cdp*` 34 GB（2026-10-14 清过一次）。`ps` 在沙箱里被禁（`operation not permitted`），**用 pgrep**：主进程取 `pgrep -fl 'MacOS/Google Chrome' | grep -v Helper`（Helper 路径里也含 `MacOS/Google Chrome`，不滤会多算上千个），日常浏览器是那个不带 `--headless` 的，**只杀带 `--headless` 的**。删 `/tmp/cdp*` 时：zsh 不对变量分词（`for d in $LIST` 会整体传给 `rm` 报 File name too long，用 `while IFS= read -r`）；里面混着 `cdp_page.js` / `cdpqa.js` / `*.log`，按扩展名跳过，只删含 `Local State` / `Default` / `SingletonLock` / `Crashpad` 的目录。`/tmp/cpp*` 与 `/tmp/hl*` 是对拍与高亮产物，按约定可复用，别清。

## 踩过的坑（技能 `references/` 下按节拆的坑录，**按需读单节别整份读**：`pitfalls-build.md` §5 拼页演示 / `pitfalls-selfcheck.md` §8 自检截图 / `pitfalls-verify.md` §11 对拍 / `pitfalls-rewrite.md` §12 页面改写，`pitfalls.md` 是 1KB 索引）

**演示与页面 JS**：演示按钮**用 id 显式点击**并同步 `disabled`，漏加会一直点到 40 帧上限；**`var el` 循环挂 handler 会闭包串味**（`el.value` 恒空串 → 演示整块空白），统一 `bind(id, ev, fn)` + `this.value`，改完用 CDP 抓 `Runtime.exceptionThrown`；**别用单个大写字母当维度变量**（`var W=660,H=300` 的 `H` 撞哈希函数，图全空而自检照样 PASS，画布高写 `HH`）；探针设下拉用 `selectedIndex`，**别缓存会被 `innerHTML` 换掉的节点**；MOD ≥ 1e6 的乘法取模用 BigInt（越过 2^53 静默丢精度）；演示随机数一律 mulberry32；`rebuild()` 里别把 step 设成末帧。

**SVG 画图**：逐图检查必须覆盖每个预设（长度可变时最后一格冲出 viewBox）；**helper 只返回自己片段，不能吃进累积串**（`g += axis(g,…)` 会让每个 `text` 多一个孪生兄弟，只有逐图比 `getBBox()` 才看得出）；标题与紧邻标签行距按 bbox 量；**0 起数组喂给 1 起的 helper 是下标最容易漏的错**；改格子间距必须同步框宽。

**代码与对拍**：**课程代码 ≠ 题面直觉**，先用随机小用例确认代码真实语义再写暴力；本机**没有 `bits/stdc++.h`**，自建 shim 到 `/tmp/cppNNN/inc/bits/` 加 `-I inc -std=c++17`；**先怀疑数据，再怀疑代码**（上界写成 `n` 的题数据限 ≤ n、缩字母表时字母表常数一起改、DAG 最长路暴力必须按拓扑序扫）；**C++ 排序比较符必须严格弱序**（Java 的 `<=` 直接抄成 `std::sort` 会段错误）；**「看着像错其实等价」要专门量一遍，0.00% 也是结论**，与真错法分开写。

**流程与文件**：就地改写页面 HTML **别用 `s.replace(s[idx(a):idx(b)], new)`**（b 在 a 前切出空串，`str.replace('', new)` 把新块插进每两个字符之间，59KB 变 80MB），写 `i = s.index(a); j = s.index(b, i); assert i < j` 再 `wc -c` 查体积；**别用 `git diff HEAD` 核生成物改动**（整份重写会报上千行假改动），用 `diff` 对比写前备份；大量条目变更时**生成物单独一笔提交**。
