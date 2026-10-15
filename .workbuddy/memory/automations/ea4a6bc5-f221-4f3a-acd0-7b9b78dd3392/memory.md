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
