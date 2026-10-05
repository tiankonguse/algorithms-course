# 自动化「转换下一课」执行记录

进度表在 `algorithms-course/prompt.md`，本文件只记每次执行的概要，方便下次接手。

## 2026-10-05（本次）

- 读了 prompt.md，下一课 = 017（二叉树及其三种序的递归实现，`src/class017/BinaryTreeTraversalRecursion.java`，低号课单 Java）。
- 产出 1 页：`algorithms/binary-tree-traversal.html`，并入「入门」组、插在 `deque.html` 之后（json 161 → 162 条）。站点第一个二叉树页面。
- 正文数字全部实测（/tmp/cpp017/exp.cpp、exp2.cpp、verify.cpp）：三序 n=10⁶ 约 2ms 且线性；栈深峰值=h（完全 20/随机 BST 53/链 10⁴）；链递归爆栈二分实测 260937 层过、261000 层段错误（约 32B/帧）。对拍 4500 组四种树形 0 mismatch。
- 5 张 SVG + 2 个演示（递归序逐帧 4 树×3 位置、递归栈逐帧 4 树）。自写 /tmp/br017.js（Node22 全局 WebSocket）把 12+4 分支跑完与手算一致。
- 踩坑：drawT7 忘接返回值整树没画；SVG text 空格折叠（esc 里换 \u00a0）；固定格宽溢出卡片；演示说明行压树底。均已修，截图逐张看过。
- 自检：verify_pages / 两演示逐帧 / verify_home 全 PASS，pager「双端队列 ↔ 二叉树三序 ↔ 离散化」，json 六字段计数全齐。
- 收口：prompt.md 下一课改成 **018**、017 写入已完成；daily log（2026-10-05.md）追加细节。

## 2026-10-04

- 读了 prompt.md，下一课 = 007（时间复杂度和空间复杂度，入门方法论课，`src/class007/Complexity.java`）。
- 产出 1 页：`algorithms/complexity.html`，并入「入门」组、插在 `binary-search.html` 之后（json 151 → 152 条）。
- 正文数字全部实测（/tmp/cpp007/exp.cpp、const.cpp、verify.cpp）；6 段页面代码自己从 Java 改写 C++ 并编译验证（冒泡/插入/DynArray/harmonic/quadratic/mergeSort 全 0 错）。
- 6 张 SVG + 2 个演示（单 while 冒泡逐帧、动态数组逐次 push）。
- 自检：verify_pages / 两个演示逐帧 / verify_home 全 PASS，pager「二分搜索 ↔ 复杂度 ↔ 离散化」。
- 踩坑：脚本里 24 处 `text(...)`/`mono(...)` 忘了 `g +=`，svg 只剩标题；bbox 检查查不出「没画」，靠截图抓出。已修。
- 收口：prompt.md 下一课改成 **008**（class008 空目录，预计记「跳过」）、007 写入已完成；daily log 追加细节。

## 2026-10-03

- 读了 prompt.md，当时下一课 = 163（树上启发式合并，倒序阶段）。
- 产出 4 页新页面：`dsot.html`、`dsot-color.html`、`dsot-depth.html`、`dsot-path.html`，新开「树上启发式合并」组放在全站最末（接在「Kruskal 重构树」组之后）。
- 7 道题全部编译 + 各 300 组随机对拍通过（题 7 强制在线，走交互编码）。
- 自检：4 页 verify_pages / verify_viz_steps / verify_home 全 PASS，全站 132 页 23 组。
- 收口：prompt.md 的 163 移入「已完成」、下一课改成 162；daily log 追加细节。
- 细节（对拍坑、SVG 越界、图例配色）见 `.workbuddy/memory/2026-10-03.md`。
