# 自动化运行记录：转换下一课

## 2026-10-03 00:00
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
