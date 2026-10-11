# 项目约定（algorithms-course）

> 本文件只放**规则**。状态在别处：进度 / 下一课 / 已完成看 `prompt.md`，组顺序与页清单看 `assets/data/algorithms.json`（`groups` 有序，每条算法带 `group`）。

## 内容
- 每页必含「复杂度分析」：一张 `table.complexity` + 空间说明，明确 O(...)。
- 每页「总结」之后固定两节：「相关题目」（平台 / 题目 / 链接，取自 PPT 与 Java 头注释）、「参考材料」（来源 / 名字 / 链接；主参考固定「左程云 - 算法讲解NNN【难度】标题」→ https://github.com/algorithmzuo/algorithm-journey/ ，其他有具体页给具体页）。
- **页面完全独立**（2026-10-06 用户定）：正文不写相邻算法的链接 / 指代，页尾不插翻页标记；翻页由 shared.js 按 json 顺序自动生成。
- 代码统一 C++（`#include <bits/stdc++.h>` + `using namespace std`），只贴关键函数不贴 main。正文不提 PPT / 课件，代码不提 Java；导航 / 标题不出现「模块」二字。
- 每页 ≥2 张图、≥1 个交互演示，一句话讲不清的结论 / 结构 / 过程都配图。默认内联 SVG（JS 生成后塞进 `<figure class="fig">` + 一句 `<figcaption>`），数组 / 表格 / 树可用 HTML+CSS 网格，不引位图。配色：主蓝 #2563eb（底 #dbeafe/#eef4ff）、次橙 #ea580c（底 #ffedd5）、灰 #9aa0a6、底 #f6f7f9、边框 #e6e8eb；浅底深字。SVG `<text>` 字号 ≥10，中文按 12~13px/字估宽。画图时顺带核对正文结论，冲突改正文。

## 写作风格（去 AI 味，对齐 tiankonguse.github.io 的 OI 文章）
- 段落一到两句，用「可以发现 / 因此 / 具体来说 / 这里有个细节」推进；抛反问后先接「确实…」再转折。章节：零、背景 → 一、问题 → 二、朴素做法 → … → 复杂度分析 → 总结 → 最后。
- 引号用「」，数值范围写「0 到 10⁹」；少用 `<strong>`，不做总结式清单。禁用 AI 模板句（「矛盾在哪？」「想一想：」「关键提醒」「核心思想」）、emoji、破折号滥用、通篇加粗，强调靠推导。
- **「不是 X，是 Y」否定并列是最隐蔽的 AI 句式**：收口前 `grep -n '不是\|而是\|并非'` 全篇扫一遍；技术对比（两个具体选项挑一个）留着，当修辞强调的一律删。别用「手滑」这类网络词，直接写「写错」。
- 结尾「总结」段（2026-10-04 用户定）：具体要点 + 一句前瞻，禁空谈升华（「标志着 / 意味着」）。范本 tiankonguse.github.io 的 ST-RMQ / DFS序 / LCA / 虚树（要点已提炼进 SKILL.md「范本对齐」块；四篇原文与去 AI 味清单 `humanize.md` 在技能 `references/`，动笔和收口前各过一遍）。

## 视觉与目录
- 纯静态无构建：`index.html` + `algorithms/*.html`；`assets/css/shared.css`、`assets/js/shared.js`、`assets/data/algorithms.json`。
- 页面骨架固定（`.layout` + `aside#sidebar` + `main .algo-section` + `aside.toc-panel ul#toc`），侧栏 / 目录 / 翻页 / 复制 / 搜索 / 进度条全由 shared.js 注入，**页面不自带骨架**；独有样式与脚本留在各页 `<style>` / `<script>`。
- shared.css 只定义一份、各行复用：`.fig` / `.fig svg` / `.legend` / `.dot` / `.breakdown` / `pre.code .cm|.kw|.fn` / `table.complexity`；全局保留 `[hidden]{display:none!important}`。
- json 的 `file` 写 `algorithms/xxx.html`，侧栏高亮比 basename。**顺序 = 首页卡片 = 侧栏 = 上下篇翻页**（看 `groups` 数组顺序，不改 `algorithms` 的绝对位置）。
- 侧栏分组折叠（2026-10-06）：打开算法页只展开当前所在组、把高亮项滚到可视区正中，其余收起；手动展开的组记 `localStorage['algoNavOpenGroups']` 跨页保持；搜索命中组临时全展开、清空恢复。
- 页底一个 `nav.pager` 含两个 `.pager-item` 并排（单侧时单卡片靠右），别写成两个 nav（会叠两行）。首页 `#homeNav` 由 shared.js 按 json groups 渲染（`#group-<i>` 锚点 + 序号 / 组名 / 篇数 + scrollspy），保持 `homeNav + home-main` flex 布局，900px 以下隐藏。
- 本地预览 `./run.sh`（端口 8080）；`file://` 直开会被 CORS 拦。

## 编排
- **从小到大转（001 → 207）**（2026-10-03）：倒着转会让背景先讲高阶算法，教学序反了。159~207 是倒序阶段产物**不回炉**，下一课号以 `prompt.md` 为准。
- **转一课不开子智能体**：主循环直接做；「连转 N 课」串行，一课收口干净（json + prompt.md 都更新）再开下一课。
- **导学课不产页**（PPT 无方法 / 算法，或 `src/classNNN/` 无代码）→ 记 prompt.md「跳过」写原因；**方法论课要产页**（005 对数器 / 007 复杂度 / 019 IO / 020 递归 / 026 容器 / 029 排序总结先例）。
- **跨会话抢锁防重**（2026-10-07）：开工第一步按 prompt.md 头部脚本抢 `/tmp/algo-course-lock-<课号>`。**凭据是 lockdir 里的 `owner` 文件、不是目录**：`mkdir -p` 建容器，抢锁 = noclobber 下 `printf > owner`（等价 O_EXCL）；抢不到且 owner ≤60 分钟就退出；死锁只看 `find owner -mmin +60`；收口只 `rm -f owner`，**别 rmdir、别删锁目录**。
- **新组插在「入门」之后**（`splice(1,0,新组)`，同阶段按课号升序），组内也按课号升序、同主题插对应位置不追加组尾；判据 = 主题承接 + 教学序（SCC 组紧贴「双连通分量与圆方树」之前的先例）。老组「高阶数据结构」是历史降序，不重排。组名撞车要避开（`kdtree-of-tree` 已占「树套树」，160 课的新组叫「二维树套树」）。
- **低号课可能撞站内已有页**：先扫 json 的 `title`，撞主题就并入已有组或给已有页补一节，不新建重复页。每课通常拆 2~4 页，按「技巧同源」合并（如 167：背包类 / 线性基类 / Trie 类）。

## 转课流水线（技能 `.agent/skills/course-site-page`）
- 站点根 `~/project/github/AIProject/algorithms-course`，课件根 `~/project/github/AIProject/algorithm-journey`。**用户目录路径一律写 `~/`**（多机运行、项目相对 home 位置固定）：脚本默认 SITE = `(process.env.SITE_DIR || '~/project/github/AIProject/algorithms-course').replace(/^~(?=\/|$)/, os.homedir())`；node 用 `~/.workbuddy/binaries/node/versions/$(cat .../current)/bin/node`。**禁写 `/Users/<用户名>/...`。**
- 材料：`ppt/算法讲解0NN【难度】标题.pptx`（文本用 zipfile + ElementTree 遍历 `ppt/slides/slideN.xml` 的 `p:sp` 拼 `a:t`）+ `src/classNNN/CodeXX_*.java`。**低号课（001~）只有一份 Java**（文件名是题目名、无 `Code0X_` 前缀、无「C++ 塞注释」版，有的空目录如 008），页面统一 C++ 需自己改写再编译对拍；**中高号偶数 java 里藏着 C++**：`awk '/^\/\/#include/{f=1} f&&/^\/\//{sub(/^\/\//,"");print;next} f{print}' X.java > X.cpp`。
- 拼页先 Write 带 `@@片段名@@` 的占位版，再用 `apply_placeholders.py` 替换（占位名必须等于片段文件名）。省上下文：PPT 抽到 `/tmp/pptNNN.txt` 后 grep/sed 按段取，源码只取要贴的一段，核验只用 grep/ls/find，**别把新页面 HTML 读回来**。

## 自检（全自动，别只靠截图）
- **「起 Chrome + 跑 node」必须写在同一条命令里**（Chrome 随 Bash 命令结束被回收）。端口默认 9333、读环境变量 `CDP_PORT`（换端口两处都要改，否则误判「Chrome 没起来」）；**并发会话别 `pkill` 别人的 Chrome**，用空闲端口 + 独立 `--user-data-dir`。
  ```bash
  rm -rf /tmp/cdpX; "$CHROME" --headless=new --no-sandbox --disable-gpu --remote-debugging-port=9333 \
    --user-data-dir=/tmp/cdpX --allow-file-access-from-files about:blank >/dev/null 2>&1 &
  sleep 8
  $N $SKILL/verify_pages.js a.html b.html   # JS 错误 / fig==svg 数 / nav·toc·viz / svg 越界 / pager 链
  $N $SKILL/verify_viz_steps.js a.html      # 逐帧点；只查第一个 .viz，第二个用 sed 换 id 再跑
  $N $SKILL/verify_home.js
  ```
- 必查四项：① 无 JS 错误 ② `figure.fig` 数 == 其中 svg 数 ③ nav/toc/每个 `.viz` 有内容 ④ 每个 svg 用 `getBBox()` 比对 viewBox（bbox = left/top/right/bottom）。`--allow-file-access-from-files` 必带，别加 `--window-size`（隐藏图标 svg 会被判空图）。
- 全站页列表从 json 生成（zsh 里展开用 `${=PAGES}`）；首页单独 verify_home.js（**无侧栏，`navGroups==0` 正常**）。跑到第 49 页左右偶发超时，脚本已重试，真失败换 `--user-data-dir` 补跑。pager 链最灵敏：非末页 2、末页 1，标题与 json 相邻项一致。拼完先跑杂字扫描与 AI 味 `grep`。

## 踩过的坑（通用教训）
- **`verify_pages.js` PASS ≠ 演示能走到底**：它只抓加载期 JS 错误、**抓不到点击后抛的异常**（067 末帧抛 TypeError → 答案行不渲染、按钮永不 disabled，自检照样 PASS）。末帧答案行与按钮 disabled 一律用 `verify_viz_steps.js` 或自写探针点到底确认（`/tmp/probe2.js` 可先设 `<select>.selectedIndex` 再点到底，穷举下拉组合）；`verify_viz_steps.js` 上限 40 帧，**「跑满 40 帧」不等于失败**（067 二叉树 44 步），要单看 disabled 与答案。
- **改格子间距必须同步框宽**（间距与框宽是两个独立参数，箭头画缝里时缝至少留 16~20px，画布高度跟着重算）。对数轴刻度标签别用固定长度上标数组（循环到 `t=6` 就要有第 6 项，否则渲染出字面量 `10undefined`）。
- **课程代码 ≠ 题面直觉**：先用随机小用例做假设检验确认代码真实语义，再据此写暴力，别改代码迁就直觉（BZOJ2870 会把单点链算进答案）。强制在线题要**交互编码**：逐条喂解码后的操作、读暴力输出当 lastAns（暴力输出用 `endl` 不用 `"\n"`）。
- 本机**没有 `bits/stdc++.h`**：自建 shim `/tmp/cppNNN/inc/bits/stdc++.h`，编译加 `-I inc -std=c++17`；对拍结果 `grep -E 'OK|MISMATCH'`。
- 树状数组上界写成 `n` 的题（如 P4093）：随机数据要把值限制在 ≤ n，否则静默不写、假 MISMATCH，**先怀疑数据再怀疑代码**。连续覆盖类题要合并区间看能否接上（只查整数点会把 `[0,4]`+`[5,7]` 判成盖满，正确 −1）；缩字母表做对数器时代码里的字母表常数要一起改（26 → 4）。
- **演示的控制按钮容器统一写 `class="viz-controls"`**（`shot_element.js` 按它索引点击，自造 `.btnrow` 会点空、截图停在初始帧）。纯 HTML 演示（无 svg）：`verify_viz_steps.js` 只查「有没有内容」，标签溢出 / 文字重叠要逐图比 `text` 的 `getBBox()`（`/tmp/overlapNNN.js`）或 `shot_steps.js` 点 N 帧截图看。
- gen_code.py：`#include` 必须先换哨兵再跑关键词正则（否则 `class` 被二次包裹），fn 只高亮定义处。
- **开工先探残留**：`ls -dt /tmp/cppNNN /tmp/hlNNN` + `ls -lT algorithms/` + `find algorithms -newermt '<半小时前>'`。中断残留常很完整（抽好的 C++、已编译 bin、brute/stress、高亮片段、半成品页），先跑 stress 看语义再补拼页。写 json 前 `cp` 备份 + 重读确认。
- **「下一课」编号与 json 末条对不上 = 可能已有失联会话做完**：查 json 有无该课 id、页面 mtime；页面与 json 已落盘、只差 prompt.md 与解锁的 → 按死锁规则清 owner 重抢锁，核对页面后**只补记进度、不重写页面**。
- **C++ 排序比较符必须是严格弱序**：把 Java 里常见的 `<=` 比较器抄成 C++ 的 `std::sort` 比较函数，相等键时两个方向都返回 true，行为未定义——094 实测 n=50、m=10⁵ 直接段错误（`<=` 崩、`<` 正常）。改写 Java → C++ 时先看比较器有没有 `<=`/`>=`，一律换成严格 `<`/`>`，相等时顺序无所谓。
- **对数器的暴力要照顾「没被处理的元素」**：消灭怪物只检查「被杀的怪来不来得及」会漏掉「没排上号的怪已经到城」，答案算得比正解大（094 第一版暴力返回 3、正解 1）。写暴力前先问一句：除了我主动处理的那些，剩下的东西会不会自己走到坏状态。

- **演示里的模运算一律用 BigInt，别用 Number**（099）：mod 到 1e9 时 `acc * base` 是 1e18，越过 2^53 就悄悄丢精度——快速幂求逆元的演示在小模数（41）上全对，换到 1e9+7 就给出 834560781（应为 1000000）。凡写了 `x * y % MOD` 且 MOD ≥ 1e6 的演示，直接改 BigInt。
- **自写逐帧探针要按 `selectedIndex` 设下拉，别按 option 的 value**（099）：`e.value = String(下标)` 在 option value 是「13/41/101」或「ok/noinc/self」时会赋成空串，select 静默退回空值，9 组组合实际全跑成同一分支、错误版本一次都没跑到。用 `e.selectedIndex = i` 才稳。
- **演示复现代码里的错法时，赋值顺序要跟源码一致**（099）：C++ 是「先 `dp[i] = base` 再减」，错法版减到 j=i 时减掉的是刚写进去的 base，整表归零；JS 里若把 base 放局部变量、最后才写表，j=i 读到的是初始 0，错法复现不出来。
