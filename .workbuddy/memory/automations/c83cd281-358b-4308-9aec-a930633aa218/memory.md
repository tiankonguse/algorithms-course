# 自动化「转换下一课」执行记录

进度表在 `algorithms-course/prompt.md`，本文件只记每次执行的概要，方便下次接手。

## 2026-10-10（本次）

- 读了 prompt.md，下一课 = 081（状压dp-下，`src/class081/` 4 份低号课 Java）。抢锁 LOCK_OK，无残留半成品。
- 产出 3 页：`bitmask-dp-hats.html`（题 1 帽子 + 题 2 账单平衡）、`bitmask-dp-good-subsets.html`（题 3 好子集）、`bitmask-dp-subset-enum.html`（题 4 分配重复整数 + 子集枚举），并入「状压 dp」组、追加在 bitmask-dp-tsp 之后，json 304 → 307、六字段全齐。
- 四题 /tmp/cpp081 手写 C++ 编译 + 各 6000 组随机对拍 0 mismatch；正文数字全部实测（BK 提取比 for 枚举省 4~15 倍、账单 sum==0 时 break 省 59.11% 分支且答案不变、debt 不剔零 100% 偏大、好子集 status 正序遍历 0 差异、漏 1 的倍率 15.37%、贪心 4.08% 假阴性、子集枚举漏 j==status 48.20%、3^m = Σ C(m,k)·2^k、n=10 硬实例枚举 272746）。
- 自检：自写 CDP 探针（verify_pages.js 本环境对任何页报全 0）三页 PASS，6 演示 × 22 select 组合逐帧到末帧 ALL COMBO OK，pager 链 tsp→三新页→树状数组 正确，首页 307 卡片 PASS，杂字/AI 味扫描 CLEAN。修掉 4 处 svg 越界与 3 处「不是X而是Y」修辞。
- 收口：prompt.md 下一课改成 **082**、081 写入已完成；daily log（2026-10-10.md）追加细节；锁已释放（只删 owner）。

## 2026-10-06（本次）

- 读了 prompt.md，下一课 = 033（位运算实现加减乘除，`src/class033/BitOperationAddMinusMultiplyDivide.java`，低号课单 Java）。开工前探过 /tmp 与 algorithms/，无 033 残留。
- 产出 1 页：`algorithms/bit-arithmetic.html`，并入「入门」组、插在 `bitset.html` 之后（json 177 → 178）。6 图 + 3 演示（加法逐帧 6 分支 / 龟速乘逐帧 5 分支 / 除法逐帧 32 步含「跳到下次命中」）。
- 正文数字全部实测（/tmp/cpp033/src/bitmath.cpp、exp2.cpp、verify.cpp）：进位末尾 0 个数严格递增（50 万组 0 反例）→ 最多 32 轮；add 随机 200 万对平均 5.24 轮 / 最长 22 轮，构造的 add(2147483647,1) 与 add(-1,1) 走满 32 轮；除数左移版 20 万组随机正整数 100% 算错（y<<29 当 int 读已是 −536870912）；命中轮次里 y<<i 超界 0 次；INT_MIN 九组边界全对；龟速乘对 __int128 参考 5200836 组 0 MISMATCH。
- 踩坑：① `ri(INT_MIN, INT_MAX)` 里 `hi-lo+1` 有符号溢出触发 UB，clang -O2 在不同调用点算出不同的 a（表现为 Div 返回值与独立程序矛盾）——全 32 位随机要写 `(int)rng()`；② 页面代码内部统一用 unsigned，对拍的真值也要按 32 位回绕比；③ 本环境 `captureScreenshot` 带 captureBeyondViewport 会挂死，改成「固定视口 + scrollIntoView + 截视口 + sips 裁」；④ SVG helper 漏传 fill 会渲染成黑底，bbox 查不出。
- 自检：verify_pages（figs/svg 6/6、nav 178、toc 8、viz 3、code 5、pager「位图 ↔ 离散化」）、三个演示逐帧、自写 branches.js 15 分支末帧答案全对、verify_home 178 卡片 PASS，六字段计数全齐。
- 收口：prompt.md 下一课改成 **034**、033 写入已完成；daily log（2026-10-06.md）追加细节。

## 2026-10-05

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
