# 自动化执行记录：转换下一课

## 2026-10-03 02:00（首次执行）

- 读取 `prompt.md` 进度，本课编号 **162**（树链剖分-下）。
- 开工前探测：`/tmp` 下只有 163~172 的旧目录，无 162 残留；`algorithms/` 最近改动为 163 课的四页（10-02 23:37~23:41），无半成品。课 161（树链剖分-上）尚未转换，**下一课是 161**，需插到新课组「树链剖分与长链剖分」组首。
- 产出 4 页并接入索引，新开组「树链剖分与长链剖分」放全站最末：`hld-edge` / `hld-pathops` / `lcd-ancestor` / `lcd-dp`。站点从 132 页 23 组变为 **136 页 24 组**。
- 7 个题全部编译 + 各 150 组随机对拍 0 MISMATCH；额外对题 6、题 7 做 n = 15~30 抽查。
- 自检：4 页 verify_pages PASS、verify_viz_steps 各 40 帧无越界、verify_home PASS、全站 136 页 verify_pages 全 PASS。
- 已更新 `prompt.md` 进度（已完成加 162，下一课改 161）与 `.workbuddy/memory/2026-10-03.md`。
- 本次踩坑已写入当日 memory：0-based 节点编号的对拍生成器、子树 DFS 起点父亲要显式给、题 7 g 表定义方向、`.dot.skip` 未定义需自补 `.dot.gray`、竖直链的 dfn 标注要放右侧。

## 2026-10-04 02:00（第三次执行）

- 读取 `prompt.md`，从小到大阶段下一课 **008**。class008 的 src 是空目录，但 PPT 2 页有可讲的方法（「任何数据结构 = 连续结构 + 跳转结构」总纲 + 硬计算/软计算分类），按技能「方法论课要产页」判据**产页而非跳过**，并改掉了 prompt.md 里原先「预计按导学课跳过」的预判。
- 产出 1 页 `algorithms/structure-two.html`（连续结构与跳转结构），并入「入门」组、插在 complexity 之后（json 152 → 153 条，pager 链「复杂度 ↔ 连续结构与跳转结构 ↔ 离散化」核对无误）。
- 正文数字全部实测（/tmp/cpp008/exp.cpp）；页面代码自己写 C++ 五个片段，verify.cpp 各 400 种子对拍全 OK。4 张 SVG + 2 个演示（演示一 34 分支全逐帧核过）。
- 自检：verify_pages / verify_viz_steps（两个 viz）/ verify_home 全 PASS，全站 153 页无 FAIL。
- 已更新 `prompt.md`（下一课 009）与 `.workbuddy/memory/2026-10-04.md`。
