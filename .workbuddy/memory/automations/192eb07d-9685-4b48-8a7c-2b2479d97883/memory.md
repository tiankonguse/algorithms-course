# 自动化运行记录：转换下一课

## 2026-10-10 00:00
- 任务：转换下一课 → prompt.md「下一课 073（背包dp-01背包、有依赖的背包）」，抢锁成功，主循环直做。
- 执行：产出 4 页并入「入门」组（lis-ext 之后、graph-build 之前）：knapsack-01 / knapsack-subset / knapsack-dependent / knapsack-topk。
  - 七题 C++ 手改（低号课无「C++ 塞注释」版），4 种子 × 3000 轮对拍全绿；全部正文数字在 /tmp/cpp073 实测（耗时、内存、错误率、具体反例）。
  - 8 个演示 × 40 个 select 组合逐帧点到末帧，答案与 C++ 逐项一致（自写 /tmp/br073.js）。
  - 自检：四页 verify_pages PASS、verify_home 283 卡片 PASS、杂字/AI 味扫描 CLEAN（修掉两处修辞否定、两处破折号）。
  - json 279 → 283（六字段全齐），锁已释放。
- 环境备忘：本环境 node 路径是 `~/.workbuddy/binaries/node/versions/22.22.2-6/bin/node`（不是 22.22.2）；verify_pages.js 这次正常工作（不像 071 那次全 0）。
- 无遗留问题。下一课 074。

## 2026-10-09 00:08
- 任务：转换下一课 → prompt.md「下一课 071（子数组最大累加和与扩展-下）」，抢锁成功，主循环直做。
- 执行：产出 3 页并入「入门」组（house-robber 之后）：subarray-product-mod7 / subarray-pre-suffix / subarray-change-once。
  - 六题 C++ 手改 + 4 种子 × 3000 轮对拍 0 mismatch；全部正文数字实测（错误率、性能、具体反例）。
  - 环境坑：verify_pages.js 在 Chrome 155 下对任何页全 0（已知好页也 0），自写 /tmp/probe071.js（/json/new + 页面级 WS）等价覆盖；verify_home 正常。
  - 自写 br071.js 把 6 演示 55 个 select 组合逐帧点到末帧，答案与 C++ 逐项一致；修掉 LC689 演示 prefix 索引错、大数显示、figWin 文本越界三处。
  - json 273 → 276（只增不删、六字段全齐），首页 276 卡片 PASS，杂字/AI 味扫描 CLEAN，锁已释放。
- 无遗留问题。下一课 072。

- 任务：转换下一课 → 读取 prompt.md 得到「下一课 164（Kruskal 重构树）」。
- 执行：按技能 course-site-page 主循环直做模式完成全部流程。
  - 抽取 PPT 文本（/tmp/ppt164.txt）与 7 份 C++（/tmp/cpp164），全部编译通过（题 4 需 -std=c++14，因 `visit` 与 std::visit 冲突）。
  - 7 题各写 Python 暴力 + 随机对拍，3 个种子（7/21/42）全部 OK。修了两个对拍脚本坑：随机图 m 无上界导致死循环；强制在线题编码器要用负向编码（raw = 真值 − k·lastAns / 真值 ^ lastAns）。
  - 生成 13 个代码高亮片段（/tmp/hl164/out），写 4 个页面：kruskal-tree / kruskal-subtree / kruskal-combine / kruskal-ds，每页 2~3 张 SVG 图 + 2 个交互演示。
  - json 新开「Kruskal 重构树」组（4 条）追加到 groups 末尾，algorithms 数组 124→128。
  - 自检：verify_pages 4 页 PASS（pager 链正确、末页 1 个 pager）、8 个演示逐帧无越界（第二个演示用 `.closest('.viz')` 变体脚本）、verify_home PASS（128 卡片）。
  - 收口：prompt.md 进度（164 已完成、下一课 163）、memory 日志 2026-10-03.md。
- 无遗留问题。

## 2026-10-04 00:00
- 任务：转换下一课 → 读取 prompt.md 得到「下一课 006（二分搜索）」。
- 执行：主循环直做，产出 1 页并入已有「入门」组。
  - 材料：PPT 仅 2 页 493 字（提纲），`src/class006/` 四个 Java（FindNumber / FindLeft / FindRight / FindPeakElement），无 C++ 版，页面代码自己改写成 C++。
  - `/tmp/cpp006`：自建 bits shim + `-I inc -std=c++17`，`main.cpp` 编译通过；`stress.py` 三种子各 4000 组对拍 exist/findLeft/findRight 与峰值，全 OK。
  - 正文数字全部用 `/tmp/cpp006/exp.cpp` 实测（线性平均 ≈ n/2+1、二分最坏 ⌈log₂(n+1)⌉、中值溢出的两组具体数值）。
  - 新页 `binary-search.html`：5 张 SVG + 2 个交互演示 + 6 个代码片段，插在 validator 之后、discretize 之前（离散化正文用到二分），json 150 → 151。
  - 自检：verify_pages / verify_home PASS，pager 链「对数器 | 二分搜索 | 离散化」三段对得上；自写 `/tmp/br006.js` 跑遍演示1 的 15 个分支（3 问法 × 5 个 num），答案与手算逐项一致、无越界。
  - 修了两处：区间收敛图的示例数据换成能走满 5 轮的 30（原 21 只走 2 行，与 figcaption 不符）；演示末帧改为沿用上一步的区间并把命中格标橙（原来会全灰）。
  - 收口：prompt.md（006 已完成、下一课 007）、memory 日志 2026-10-04.md、技能补了两条经验（演示末帧设计、图内结论文字要与渲染一致）。
- 无遗留问题。

## 2026-10-05 00:00
- 任务：转换下一课 → 读取 prompt.md 得到「下一课 016（双端队列-双链表和固定数组实现）」。
- 执行：主循环直做，产出 1 页 `algorithms/deque.html`，并入「入门」组、插在 min-stack 之后（离散化之前）。
  - 材料：PPT 仅 1 页提纲；`src/class016/CircularDeque.java` 一份，无 C++ 版，页面四份实现自己改写 C++（哨兵双链表 / 环形数组有特判 / 环形数组不变式 / 静态数组双链表）。
  - `/tmp/cpp016`：自建 bits shim + `-I inc -std=c++17`；`verify.cpp` 4000 组种子对拍 std::deque 全 OK；`perf.cpp` 量性能（滑窗 100 万轮：窗口 8 → 2.29/3.58/7.54/27.10 ms）；`bughunt.cpp` 量六种笔误（25.11% / 100% / 80% / 100% / 80%(越界) / 99.89%）。
  - 5 张 SVG + 2 个演示（环形数组操作台 3 序列 × 4 版本；双链表操作台 2 × 2）；自写 `/tmp/br016.js` 把 12 + 4 个分支全跑完，无 JS 错误、无 bbox 越界。
  - 自检：verify_pages PASS（figs/svg 5/5、nav 161、viz 2、code 4、pager「最小栈 | 离散化」）、verify_home PASS（161 卡片）、AI 味与杂字扫描干净。json 160 → 161。
  - 收口：prompt.md（016 已完成、下一课 017）、memory 日志 2026-10-05.md、技能第十二节补了四条经验（切片替换炸文件、rebuild 停末帧、模拟器要防断链、随机序列要加权）。
- 无遗留问题。

## 2026-10-07 00:00
- 任务：转换下一课 → prompt.md「下一课 058（洪水填充）」，抢锁成功后按技能 course-site-page 主循环直做。
- 执行：产出 3 页并入「入门」组（union-find-malware 之后、discretize 之前）：flood-fill-basic（原理 + 岛屿数量 + 被围绕的区域）、flood-fill-island（最大人工岛：编号 + 面积表 + visited 去重）、flood-fill-bricks（打砖块：反向补砖 + 感染计数）。
  - 4 题各 6000 组对拍全绿（打砖块暴力生成器要先滤出「开局稳定」的砖）；全部正文数字实测（调用次数 4n²+1、272×272 能过 274×274 段错误、被围绕顺向 vs 反向 2126.76/0.44ms、不去重错误率 58.31%、三坑 14.82%/69.46%/0%、正向模拟 2596.87ms vs 反向 0.72ms）。
  - 13 张 SVG + 6 个演示；`/tmp/br058.js` 走 CDP 把 24 组预设逐帧点到末帧，答案与 C++ 实测全一致（两次 FAIL 均为验证脚本期望串写错）。
  - 自检：三页 verify_pages PASS、相邻页 pager 链双向正确、verify_home 240 卡片 PASS、全站 240 页 verify_pages 全 PASS、杂字/AI 味扫描 CLEAN（修掉三处破折号、一处修辞否定、三处图内重叠）。
  - 收口：json 237 → 240（六字段全齐）、prompt.md（058 完成、下一课 059）、memory 日志 2026-10-07.md、锁释放。
- 无遗留问题。

## 2026-10-06 00:00
- 任务：转换下一课 → 读取 prompt.md 得到「下一课 032（位图）」。
- 执行：主循环直做，产出 1 页 `algorithms/bitset.html`，并入「入门」组、插在 bit-tricks 之后（discretize 之前）。
  - 材料：PPT 2 页（位图原理 + LeetCode 2166 Design Bitset）；`src/class032/` 两份低号课风格 Java（Code01_Bitset、Code02_DesignBitsetTest），无 C++ 版，页面代码自己改写 C++（数组用 `unsigned`，掩码 `1u << bit`）。
  - `/tmp/cpp032`：自建 bits shim（补了缺的 `<unordered_set>`）；`verify.cpp` 对数器跑通（基础位图 vs unordered_set、设计位图 vs 真翻的朴素实现，0 不一致）；`exp*.cpp` 量出全部正文数字。
  - 关键实测：判重四做法（位图 23.12ms / char 57.75ms / 排序 501.47ms / 哈希集合 979.17ms）；常驻内存（1e8 个位：位图 12MB vs char 96MB；哈希集合存 1e7 个 int 512MB）；求交 855 倍；flip 懒标记 3600 倍；三个坑（`1<<31` 实测四操作仍全对、`(n+31)/32` 溢出 vector 抛异常、n=70 多出 26 位）。
  - 踩坑：macOS 沙箱里 calloc 惰性零页把 `vector<char>(n,0)` 的 RSS 量成 0，要强制触页才量得准（详见当日 memory 日志）。
  - 6 张 SVG + 2 个演示；自写 `/tmp/br032/branches.js` 把 preset+rand 共 48 步跑到末帧，set 值与手算逐项一致。修了三处截图问题（fig-layout 标注重叠、fig-ops 高亮错位与「→ true」丢失、演示 1 的位面板按 uint32 分行）。
  - 自检：verify_pages PASS（figs/svg 6/6、nav 177、toc 9、viz 2、code 4、pager「位运算的常见技巧 | 离散化」）、verify_home PASS（177 卡片）、json 176 → 177 六字段全齐、AI 味与杂字扫描干净。
  - 收口：prompt.md（032 已完成、下一课 033）、memory 日志 2026-10-06.md。
- 无遗留问题。

## 2026-10-11 00:09
- 任务：转换下一课 → prompt.md「下一课 085（数位dp-下）」，抢锁成功，主循环直做。
- 执行：产出 3 页并入已有「数位 dp」组（追加在 digit-dp-mask 之后）：digit-dp-windy / digit-dp-palindrome / digit-count-in-range。
  - 7 份 Java 手改 C++（低号课无「C++ 塞注释」版），4 种子 × 3000~4000 组对拍全绿；全部正文数字在 /tmp/cpp085 实测（实填格数、裸递归调用、四种错法抓取率、引理穷举、cnt 的 off-by-one、性能）。
  - 15 张 SVG + 6 个演示；自写 /tmp/br085.js 把 6 演示全部 select 组合逐帧点到末帧，62 项断言与 C++ 逐项一致。
  - 自检：三页 probe085 PASS、verify_home 319 卡片 PASS、杂字/AI 味扫描 CLEAN（5 处破折号、2 处修辞否定已改）。json 316 → 319（六字段全齐），锁已释放。
- 环境备忘：verify_pages.js 在本环境对任何页仍全 0（同 071/083 的怪癖），用自写 CDP 脚本等价覆盖；新增 /tmp/ovl085.js 做 svg 文字两两重叠/出界几何检查，替代逐张看图，效果不错。node 路径是 `~/.workbuddy/binaries/node/versions/22.22.2-6/bin/node`。
- 无遗留问题。下一课 086。
