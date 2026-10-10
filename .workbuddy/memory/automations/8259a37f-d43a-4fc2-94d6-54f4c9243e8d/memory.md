# 自动化「转换下一课」执行记录

## 2026-10-10 23:52 - 10-11 00:10（本次）
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
