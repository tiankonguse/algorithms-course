# 自动化：转换下一课 — 执行记录

## 2026-10-03 23:00
- 取 prompt.md 的「下一课」= **005**（对数器），完成；「下一课」已推进到 **006**。
- 产出 1 页：`algorithms/validator.html`（标题「对数器」），并入已有「入门」组、插在 `sort-basic.html` 之后，json 149 → 150 条。
- 关键判断：005 是**方法论课**（PPT 仅 2 页、无算法只有 `Validator.java`），但「怎么在没有评测机时验证自己的代码」是后续每课都要用的手法，因此产页而非记为「跳过」——这条判断标准已补进技能第一节。
- 页面骨架数据全部实测（`/tmp/cpp005/bughunt.cpp`）：四种笔误注入方法 a 的抓取率（冒泡少跑一趟 200/200 中位 32 轮、插入到不了 0 位 200/200 中位 1 轮、选择初值 n-1 200/200 中位 1 轮、选择多写等号 **0/200 抓不到**），以及二分 `>=` 写反时值域 V 对命中轮数的影响（V=5→2 轮 … V=1000→148 轮）。
- 两个交互演示：对数器跑批（五个 bug 分支）、值域 V 与命中轮数的对数柱图。
- 自检：verify_pages 全站 150 页 PASS、verify_home 150 卡片 PASS、pager 链正确；自写 `/tmp/lg005.js` 跑遍五个 bug 分支与五档 V，无 JS 错误、无 svg 越界；三图两演示截图逐张看过（修掉图 1 箭头方向、图 2 标签重叠）。
- 顺手沉淀两条技能经验：演示随机数用 mulberry32（大乘数 LCG 在 JS 会丢精度）、演示含算出来的结果时用自写 CDP 脚本并读回 DOM 属性核对标色。
- 下一课：006（二分搜索）。

## 2026-10-04 23:00
- 取 prompt.md 的「下一课」= **015**（最小栈，LeetCode 155），完成；「下一课」已推进到 **016**。
- 产出 1 页：`algorithms/min-stack.html`，并入已有「入门」组、插在 `queue-stack-convert.html` 之后，json 159 → 160 条（六字段齐全）。
- 四个实现自己改写 C++ 并交叉对拍 8004 组序列 0 mismatch（`/tmp/cpp015/verify.cpp`）：同步 mn 栈 / 省空间版（mn+cnt）/ 差值法 / 定长数组版。
- 正文数字全部实测（`/tmp/cpp015/exp.cpp`、`exp2.cpp`）：全扫 vs min 栈（n=1e5 问 n 次 2500ms vs 0.025ms）、mn 栈平均长度 ≈ H(min(n,V))（实测 7.37 vs 理论 7.48）、省空间版不计数的错法出错率（V=5 → 60.22%）、同步版 pop 忘弹 mn（76.36%）、INT_MAX−INT_MIN 在 int 下溢出成 −1、vector 到 1e6 扩容 21 次、定长 vs vector 30.27/33.10ms。
- 重要更正：同步 min 栈里「`<=` 写成 `<`」结果零差异（20000 组）——等号的坑在省空间版，页面按此写。
- 5 图 2 演示；自写 `/tmp/br015.js`（WebSocket 版）跑 15 分支逐帧查标红，a×bug 帧 7 现形、c×bug 弹空、正确版 0 误报；CDP 全 PASS（figs 5/5、nav 160、toc 9、viz 2、code 4、pager 链 队列和栈→栈和队列相互实现→最小栈→离散化）、verify_home 160 卡片 PASS。
- 技术坑：`/json/runtime/evaluate` 不是 CDP HTTP 端点，自写脚本要照 shot_element.js 走 WebSocket + Target.attachToTarget(flatten)；shot_element 的点击参数传字符串 "0" 也是 truthy 会点第一个按钮。
- 下一课：016。

## 2026-10-05 23:00
- 取 prompt.md 的「下一课」= **031**（位运算的常见技巧），完成；「下一课」已推进到 **032**。
- 产出 1 页：`algorithms/bit-tricks.html`（level 2），并入已有「入门」组、插在 `xor-tricks.html` 之后（030 结尾「下一讲继续在位运算上做文章」接得住），json 175 → 176 条，六字段齐全。
- 六个问题：判 2 的幂、判 3 的幂、最小 2 的幂、区间全体按位与、逆序 32 位、数 1 的个数（+ 汉明距离）。低号课只有一份 Java，页面代码自己改写成 C++（统一 unsigned）。
- 核心实测结论（/tmp/cpp031）：clang 把 BK 的 `while(n){n&=n-1;}` 识别成 popcount 编成 NEON `cnt`，-O2 下 0.53ms 比手写 SWAR 3.16ms 快 6 倍；-O0 下 SWAR 恒 36.59ms、BK 32.15→235.08ms。全部实现与暴力对拍 0 mismatch。
- 6 SVG + 3 演示；自写 /tmp/br031/branches.js 跑 6+4+8 分支 ALL PASS，全站 verify_pages PASS、verify_home 176 卡片 PASS、pager 链正确。
- 技能补三条（course-site-page/SKILL.md）：演示 note 必须带「第 k / n 步」（逐帧脚本按文本变化 break，会提前退出）、图里框内两行文字要加高框与行距、性能数字要连 -O0 一起量并 `-S` grep 指令名确认编译期识别。

## 2026-10-06 23:5x
- 取 prompt.md 的「下一课」= **057**（并查集-下，4 题全 LeetCode），完成；「下一课」已推进到 **058**（洪水填充）。
- 产出 3 页，均并入已有「入门」组、插在 `union-find-islands.html` 之后 `discretize.html` 之前，json 234 → 237（level 2）：
  - `algorithms/union-find-extra.html`：LC947 移除最多同行同列石头（行列两张 map + sets，答案 n − sets；「只连第一个遇到的」4 条边 vs 全连 10 条边集合数一样）+ LC2092 找出知晓秘密的所有专家（代表节点挂 secret 标签、按时刻分批合并 + 不知秘密者退回单点）。
  - `algorithms/union-find-goodpath.html`：LC2421 好路径的数目（边按 max(vals[u],vals[v]) 升序加，代表节点取集合最大值，等值合并贡献 maxcnt[fx]*maxcnt[fy]）。
  - `algorithms/union-find-malware.html`：LC928 恶意软件传播 II（病毒点不进集合、三态 infect −1/≥0/−2、cnts 按源头累加）。
- 页面 C++ 全部自写（低号课无「C++ 塞注释」版），四题随机对拍 ALL OK；所有正文数字来自 `/tmp/cpp057/verify.cpp`、`exp.cpp`、`exp2.cpp`、`samples.cpp` 实测，含六种常见错法的错误率。
- 自检：CDP 9337 + /tmp/cdp057x，三页 verify_pages PASS（figs/svg 3/3、4/4、4/4，toc 8/8/10，viz 2/2/2，code 2/1/1）、演示逐帧 40 帧无 JS 错误无越界、pager 链核对正确、verify_home 237 卡片 PASS、json 六字段齐全且 diff 只增不删。
- 排坑（可沉淀到技能）：三页占位符外套的 `<pre class="code">` 与 apply_placeholders 产出的 `<pre class="code">` 再次双层嵌套（056 已踩同款），收口前务必 `grep -c 'pre class="code"'` 比对数。
- 下一课：058。

## 2026-10-07 23:5x
- 取 prompt.md 的「下一课」= **066**（从递归入手一维动态规划，8 题 LC509/983/91/639/264/32/467/940），完成；「下一课」已推进到 **067**。
- 产出 3 页，并入已有「入门」组、插在 `bellman-ford-spfa.html` 之后 `graph-build.html` 之前，json 259 → 262（level 2）：
  - `algorithms/dp-from-recursion.html`：斐波那契四步（暴力递归 → 记忆化 → 严格位置依赖 → 空间压缩）+ 最低票价（f(i) 定义、三种票枚举 O(1)、倒着填表）。
  - `algorithms/dp-decode.html`：解码方法（'0' 是死路、dp[n]=1 种子）+ 解码方法 II（'*' 五类倍数表：单转 9、'1*' 9、'2*' 6、'*小数字' 2、'**' 15）。
  - `algorithms/dp-1d-apps.html`：丑数 II 三指针归并、最长有效括号三段拼、环绕串按字符取最大延伸、不同子序列 II 按结尾字符计数去重。
- 页面 C++ 全自写（低号课无「C++ 塞注释」版），8 题 4 种子 × 3000 轮对拍 ALL OK；正文数字全部来自 `/tmp/cpp066/` 实测（fib n=40 调用 3.3 亿次 354ms vs dp 0.0008ms；记忆化 2n−1 / 票价 3n+1；n=2×10⁷ dp 数组 10.91ms/76MB vs 滚动 4.99ms；解码 II 全 '*' len=10 不取模 13483456911；错版抓取率 9 组：else-if 94.94%、漏 dp[p−1] 38.77%、不取 max 23.98%、不减 cnt[x] 90.42% 等）。
- 自检：CDP 三页 verify_pages PASS（figs/svg 5/5、5/5、4/4，toc 11/10/9，viz 2/2/3，code 6/6/4），自写 /tmp/br066.js 跑 7 个演示 × 全部 select 组合共 55 组合逐帧点到末帧 ALL COMBO OK（末帧答案与 C++ 实测逐项一致），全站 262 页 verify_pages PASS、verify_home 262 卡片 PASS、json Counter 六字段全齐、杂字/AI 味扫描 CLEAN。
- 排坑（可沉淀）：① 对拍暴力参考用 bitmask 枚「n−1 个缝」的切法会重复计数（相邻两位同取映射到同一 partition），解码题的暴力要递归枚「切 1 位还是 2 位」；② 图里文字与格子坐标要在截图时核对三处：figTicket 相邻日 i= 标注重叠（删行）、figDp 注释压格（右移）、figParen 曲线起点错位（改三条彩色下划线段 + 上方 5·i / 2·p 标注，比曲线清楚得多）；③ 演示里 dp 数组长度多开一格会让末帧下标错位，先算好 0..n 还是 1..n。
- 下一课：067。

## 2026-10-08 23:00
- 取 prompt.md 的「下一课」= **070**（子数组最大累加和与扩展-上，6 题），完成；「下一课」已推进到 **071**。
- 产出 3 页，并入已有「入门」组、插在 `dp3d-scramble.html` 之后 `graph-build.html` 之前，json 270 → 273（level 2/3/2）：
  - `algorithms/max-subarray-basic.html`：LC53 一维最大子段和（dp 定义 / 滚动变量 / 换开头写法带 left/right/sum），四种错法错误率实测（ans 初值 0 全负数组 100%、不换开头 44.14%、忘 l=r 44.14%、pre>0 仅区间不同 5.40%）。
  - `algorithms/max-subarray-circular.html`：LC918 环形（all − minsum 与 all==minsum 退化，缺判断 7.77%、只算 all−minsum 37.09%，绕过去占比随 n 升至 74.52%）+ LCCI 子矩阵（压行 O(n²m)，忘清 nums 58.33%、顺序反 67.60%，100²→0.40ms / 300²→11.71ms）。
  - `algorithms/house-robber.html`：LC198 线性 + LC213 环形（只算一半 37.76% / 右界 n−1 37.48%）+ LC2560 打家劫舍 IV（二分 + 贪心计数，>=k 写 >k 错 86.67%，n=1e5 二分 0.86ms 30 轮）。
- 六题 4 种子 × 3000 轮对拍 0 mismatch（Q5 暴力参考误用环形判据，修成线性后全绿）；7 个演示 26 个 select 组合逐帧点到底，末帧答案与 C++ 实测一致；全站 273 页 verify_pages + verify_home PASS、pager 链正确、杂字/AI 味 CLEAN。
- 下一课：071。
