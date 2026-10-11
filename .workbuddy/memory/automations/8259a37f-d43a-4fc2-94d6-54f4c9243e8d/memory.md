# 自动化「转换下一课」执行记录

## 2026-10-11 12:33 起（本次）
- 读 prompt.md 时「下一课」是 **088**，但 json 里已有 `dp-summary` 条目、`algorithms/dp-summary.html` mtime 03:19、锁 `/tmp/algo-course-lock-088/owner` 停在 03:09（超 60 分钟）→ 判定上一会话写完页面与 json 后失联，**只剩 prompt.md 与解锁没做**。
- 按死锁规则清 owner 重抢锁（LOCK_OK），本轮**只核对不重写**：verify_pages PASS（figs/svg 6/6、nav 326、toc 11、viz 2、code 6、pager 链正确）、verify_home PASS（326 卡片、含新组「动态规划总结」）、复跑 /tmp/cpp088 全部实验逐项复现正文数字、复跑它自写的 /tmp/probe088.js（调用树 n=4/5/6 末帧答案与 disabled 全对、估算台 20 组合与独立复算一致、6 图 bbox 不越界、无 JS 错误）。
- 顺手清理：估算台 `MODELS` 里一条没有对应 `<option>`、永远选不到的「数位 dp」死数据删掉，probe 复跑确认行为不变。
- 进度回写：prompt.md 新增 088 条目、「下一课 088 → 089」；本地日志 `.workbuddy/memory/2026-10-11.md` 追加一段；MEMORY.md 踩坑节加一条「下一课编号与 json 末条目对不上 = 很可能已有失联会话做完，先查条目与 mtime 再决定」。
- 088 产出：1 页 `dp-summary.html`（新组「动态规划总结」插在「按数据量猜解法的 dp」之后），PPT 088「动态规划专题总结与预告」，`src/class088/` 无目录、页面无题目，代码片段自写自测。json 325 → 326。
- 下一课：**089**（贪心经典题目专题 1）。

## 2026-10-10 23:52 - 10-11 00:10（更早）
- 开工读 prompt.md 时「下一课」已是 **084**（083 由上一轮 23:3x 完成，本次的自动化 memory 还停在 083 的旧快照，以 prompt.md 为准）。
- 抢锁 `/tmp/algo-course-lock-084/owner` 成功（noclobber 独占写），收口 `rm -f owner` 释放（目录保留）。
- 转换第 **084** 课（数位dp-上）→ 3 个页面，**新开「数位 dp」组**插在「优化枚举的 dp」之后（入门段 DP 专题按课号升序：树型 dp 078 → 状压 dp 080 → 优化枚举的 dp 082 → 数位 dp 084）：
  - `digit-dp-basic.html`：数位 dp 骨架（补前导 0 的定长串 + free/fix），LC357 闭式组合、LC902 两版实现（free/fix 递归 vs cnt 组合表）。
  - `digit-dp-range.html`：LC2719，区间拆两次上界相减 + 数位和进状态；两条剪枝（含「下界剪枝删不得」这条结论）。
  - `digit-dp-mask.html`：LC2376 / LC1012，用过的数字塞 10 位掩码 + cnt 排列表 + 互补关系。
- 流程：抽 PPT（5 页）→ 读 5 份 Java → 自改 C++ → `/tmp/cpp084/verify.cpp` 四题对拍全 0 mismatch → `exp.cpp` / `exp2.cpp` / `extra.cpp` 量性能与 11 组错法抓取率 → 写 3 页（带占位符）→ 套 pre 嵌套修回 → `apply_placeholders.py` → json 313→316 → CDP 自检（verify_pages / verify_home PASS）+ 自写 `/tmp/probe084.js` 穷举 16 组 select 组合点到末帧（答案与暴力一致、按钮 disabled）+ 杂字/AI 味扫描 CLEAN。
- 本轮修掉：1 处 SVG 越界（页面 2 性能图对数纵轴范围不够）、1 处真嵌套 `</pre></pre>`（三页各 2~3 处）、3 处否定并列、演示首位候选漏跳过 0。
- 进度回写：`prompt.md` 新增 084 条目、「下一课 084 → 085」；本地日志 `.workbuddy/memory/2026-10-10.md` 追加一段；MEMORY.md 组顺序小节更新。
- 下一课：**085**（数位dp-下）。

## 2026-10-10 22:39-22:55（上一轮）
- 082 课 3 页（优化枚举的 dp-上）；详见 prompt.md 的 082 条目。083 由后续会话完成（prompt.md 有记录）。
