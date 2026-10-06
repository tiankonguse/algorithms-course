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
