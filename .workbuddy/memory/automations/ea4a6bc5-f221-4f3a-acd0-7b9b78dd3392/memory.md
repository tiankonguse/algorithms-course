# 「转换下一课」自动化执行记录

## 2026-10-14 21:27 — 120 课（树的重心）

- 进度指针读自 `prompt.md`（当时「下一课：120」），抢 `/tmp/algo-course-lock-120/owner` 后开工，收口已删 owner。
- 产出 3 页，新开「树的重心」组（图论下，lca 之后）：`tree-centroid` / `tree-centroid-weight` / `tree-centroid-unique`。四题（POJ1655、POJ3107、P2986、CF1406C）自改 C++ 并随机对拍 `bad=0`。
- 自检全过：verify_pages 三页 PASS、verify_viz_steps、verify_home（427 / 9 / 80 / 258）；自写探针 38 项断言、重叠检查 99 帧 CLEAN。
- 收口动作：nav 接入 + 重新生成 json + 入口页兜底数字同步 + prompt.md 进度推到 121 + 两笔 git 提交 + 清掉本次起的 headless Chrome。
- 下一课是 **121**（树的直径）。流程照技能 `.agent/skills/course-site-page` 走；开工第一步先抢 `/tmp/algo-course-lock-121/owner`，抢不到就退出。

## 2026-10-15 13:12 — 125 课（轮廓线 dp）

- 进度指针读自 `prompt.md`（当时「下一课：125」），抢 `/tmp/algo-course-lock-125/owner` 后开工，收口已删 owner（121~124 由别的会话完成，本轮只做 125）。
- 产出 3 页，**新开「轮廓线 dp」组**挂在 `cat: "动态规划"` 下（插在 `dp-bitmask` 之后、`dp-tree` 之前）：`contour-dp`（原理 + P1879 种草）、`contour-dp-tile`（POJ2411 贴瓷砖 + P2435 染色）、`contour-dp-kings`（P1896 摆放国王），难度 3/4/5。教学序就是组内序。
- 材料：`ppt/算法讲解125【扩展】轮廓线dp.pptx`（7 页）+ `src/class125/` 9 份 Java，**全无「C++ 塞注释」版**，页面代码全部自改 C++。
- 4 题全部编译（`g++ -O2 -std=c++17` + 自建 bits shim）+ 独立暴力随机对拍 `bad=0`；正文数字全部自己量（`/tmp/cpp125/exp.cpp`）。
- 自检全过：verify_pages 三页 PASS（pager 链双向正确）、verify_viz_steps、verify_home（444 / 9 / 84 / 273）；自写 `/tmp/js125/overlap.js` 全预设全帧 ALL CLEAN，`/tmp/js125/probe.js` 12 项断言与 node 侧独立实现（空间压缩迭代版）逐项一致。
- 收口动作：nav 父层与组 json 落盘 + 重新生成 json + 入口页兜底数字同步（438→444 / 83→84 / 268→273）+ prompt.md 进度推到 126 + 两笔 git 提交 + 清掉本次起的 headless Chrome。
- 下一课是 **126**。开工第一步先抢 `/tmp/algo-course-lock-126/owner`，抢不到就退出。

## 2026-10-16 03:00 — 130 课（单调队列与单调栈优化）

- 进度指针读自 `prompt.md`（当时「下一课：130」），抢 `/tmp/algo-course-lock-130/owner` 得 `LOCK_OK auto-1792090900-79226`，收口已删 owner（126~129 由别的会话完成，本轮只做 130）。
- 产出 **4 页，新开「单调队列与单调栈优化」组**（`nav/dp/dp-monoq/`，插在 `dp-optimize` 之后、`dp-restore` 之前）：`dp-monoq-basic`（P1725 + P3800）、`dp-monoq-formula`（P2627 + POJ1821 + LC2463）、`dp-monoq-stack`（LC2281 + 变序和）、`dp-monoq-boxes`（LC1687），难度 3/4/4/4。教学序就是组内序。
- 材料：`ppt/算法讲解130【扩展】动态规划优化专题4-单调队列和单调栈优化.pptx`（9 页）+ `src/class130/` 8 份 Java，**全无「C++ 塞注释」版**，页面代码全部自改 C++。
- 8 题全部编译（`g++ -O2 -std=c++17` + 自建 bits shim 到 `/tmp/cpp130/inc/bits/`）+ 独立暴力随机对拍 `bad=0`；错法比例与例子数字全部自己量（`exp.cpp` / `exp2.cpp` / `exp3.cpp`）。详细记录按课号写入 `progress/130.md`。
- 自检全过：verify_pages 四页 PASS（pager 链双向正确）、verify_viz_steps 逐帧无空帧、verify_home（**463 / 9 / 87 / 289**）；自写 `/tmp/js130/probe.js` 20 项末帧答案与页面内独立暴力实现逐项一致、`/tmp/js130/overlap.js` 15 项逐图逐帧文字 bbox ALL CLEAN。
- 收口动作：nav 父层与组 json 落盘 + 重新生成 json（459→463 页、86→87 组）+ 入口页兜底数字同步（444→463 / 84→87 / 273→289）+ prompt.md 进度推到 **131** + 两笔 git 提交 + 清掉本次起的 headless Chrome。
- 下一课是 **131**（树状数组和线段树优化）。开工第一步先抢 `/tmp/algo-course-lock-131/owner`，抢不到就退出。
