# 坑录 §8 自检与截图

> 从 `pitfalls.md` 拆出的单节副本（2026-10-14，为按需读降 token）。总索引见 `pitfalls.md`。

## §8 自检与截图（完整版，最长）

### check_branch.js 的死点

带滑块/单选的演示：verify_viz_steps 只点默认状态，另一条分支（如 x>B 的暴力分支）覆盖不到，需要「先改控件再逐帧点」时用脚本目录里的 check_branch.js（2026-10-02 起已有）。

⚠️ 但 check_branch.js 是死点 `.viz-controls button:nth-child(3)` 和 `:nth-child(2)` 的：控件区里只要有 `div.ctl`（select/滑块的外层容器）排在按钮前面，这两个下标就落到 div 上，点了等于没点、帧帧相同，看起来像「演示没生效」。**控制区含 select/slider 的页面别用它**，自写一个循环脚本更稳（2026-10-03，004 课）：切 select 的值 + dispatchEvent('change')，然后每帧「按文字 idxOf('下一步') 找按钮 → click」直到步数不再增加，顺便把 stats/bbox 一起读回来比对。参考实现：scripts/example_controls_check.js（004 课写的，改掉选择器/期望值即可复用；它同时读回 breakdown 里的计数，和 python 手算逐项比对）。

### 越界报的是 (left, top, right, bottom)

`bbox=(-21.8,17.0,678.4,268.8) vb=(0,0,660,272)` 意思是左边和右边都出去了。中文标注按 12~13px/字估宽，`text-anchor="middle"` 的长句最容易左边溢出，改成 `font-size` 小一点或锚点改 start 就好。

### verify_viz_steps 的按钮与 HTML 演示

`verify_viz_steps.js` 现在按**按钮文字**找「下一步 / 重置」，找不到再退回老页面的 `.viz-controls button:nth-child(2|3)`；演示是纯 HTML+CSS 网格（没有 svg）时只打印 `(无 svg，跳过 bbox)`，不算失败——**但这也意味着演示的排版完全没被检查**。演示是 HTML 卡片（没有 svg）时，自己用 `shot_steps.js` 点到最后一帧截图看一眼（171 就是这样发现「窗口内机器人」显示成了格子号的）。最多点 40 帧。

### 第二个演示要复制脚本改选择器

**它只查 `document.querySelector('.viz')`，也就是页面里的第一个演示**。一页有两个演示时（cdq-count、cdq-plane 都是），第二个演示不会被点到：复制一份脚本，把选择器改成那个演示的 id 再跑一次即可：

```bash
sed "s/document.querySelector('.viz')/document.getElementById('ccMerge')/g" \
  $SKILL/verify_viz_steps.js > /tmp/viz2.js
CDP_PORT=9333 $N /tmp/viz2.js cdq-count.html
```

注意它**不报**「跳过」以外的信息，别把第一个演示的 PASS 当成整页通过。

**但 `shot_steps.js` 不需要 sed**（159 课核实）：它的第二个参数就是 viz 序号（脚本内部拼 `document.querySelectorAll('.viz')[idx]`），看第二个演示直接 `shot_steps.js a.html 1 5 /tmp/s.png`。只有 `verify_viz_steps.js` 要复制改选择器。

### 图例的四个档位

`shared.css` 里 `.legend .dot.skip` **已经定义**在 138 行（灰底 `var(--bg)` + `#c9ced4` 边框）。可用的是 `.dot`（浅蓝底蓝框）、`.dot.full`（实心蓝）、`.dot.partial`（浅橙底橙框）、`.dot.skip`（灰）。**先定档位、再写图的填色**：浅蓝 `#dbeafe`/`#2563eb` = 本次操作改到的位置，浅橙 `#ffedd5`/`#ea580c` = 重链/长链上的点，灰 `#f6f7f9`/`#c9ced4` = 不参与。需要额外的实心橙档（例如「减树 / 橙色竖线」）时，在**该页自己的 `<style>`** 里补一条：

```css
.legend .dot.minus{background:var(--partial);border-color:var(--partial);}
```

### 标注压线（bbox 抓不到）

`verify_viz_steps.js` 与 bbox 检查都抓不到「标注压线」（标注仍在 viewBox 内，只是压在圆圈或标题上）。两个高发位置：① **竖直排布的重链/长链**，节点正下方 `py + 28` 的 `dfn x` 标注会落进下一个节点圆圈，要改成放右侧 `px + 22` + `anchor="start"`；② 改完标注后下方的 dfn 行/标题要跟着下移（约 16px）并**同步加高 `viewBox`**。收尾一律用 `shot_steps.js` 点到最后一帧看一眼，别只信 PASS。

### 演示的「最后一帧」要专门看（006）

逐帧脚本只保证每帧不越界、有内容，**不管最后一帧好不好看**。典型翻车是「结束」那一步把 `l/r/m` 直接置空，末帧变成整排灰格子，像演示坏了。做法是让结束步**沿用上一步的 `l/r/m`**（区间框、指针位置都留着），只把结论标出来（命中格标橙 + 一行文字），再点满步数截图确认。同理，`m` 和 `ans` 落在同一格时两个标签会在同一个 y 上叠字，要合成一个（如 `m=ans`）并定好谁的颜色优先。

### 图里「结论性的文字」也要核对（006）

区间收敛图原本用 `num = 21`，实际只生成 2 行，而 figcaption 写着「第 3 轮命中」、正文写着「16 → 8 → 4 → 2 → 1」——数字和句子都不是画出来的。**挑示例数据时先心算一遍会产生几行/几步**，想展示「每轮减半」就选一个走满 ⌈log₂(n+1)⌉ 轮的数（006 里把 `num` 从 21 换成 30，正好走满 5 行、区间长度 16→8→4→2→1）。轴上刻度的格式化也别偷懒：`(1000/1000)+'k'` 会打出 `n=1000k`、`500.001k` 这种难看的标签，直接打整数。

### 数值对但语义错（160）

tot-apps 的演示把交换后的数组渲染成了 `[3,6,1,5,6,4]`——位置下标写错，数字本身都在。bbox、逐帧、图数检查全 PASS。**但 `shot_steps.js` 只截一张图，要看到最后一帧必须把「点几次下一步」给够**（步数是 `steps.length - 1`，不是 0）。凡是演示里出现「算出来的结果」（数组、计数、答案），都要点满步数截图，再和手算结果比一遍。

### 文案重复（028）

demo2 结论文案写成 `+rounds+' 轮（'+roundsHint+' 轮级别）`，读回 DOM 发现渲染成「10 轮（10 轮级别）」——roundsHint 与 rounds 本来就是同一语义。凡是「变量名直接拼进结果串」的文案，截图读回文本看一遍再收口。

### 演示的 note 里要带「第 k / n 步」（031）

自写逐帧脚本通常按「info 文本不再变化」判断跑完并 break，可 note 文案是逐步相同的（n=1 涂开时五步都被判「已涂满」、n=0 只有一步），脚本会在中途退出，看起来像「演示没跑完」。做法：`render()` 里统一拼一句 `第 X / Y 步`（前端也顺带多一个进度指示），脚本就读得到推进。同理，**末帧的结论不要只挂在 note 上**，答案那一行也拼进去（`答案：128`），脚本才能从末帧文本里取值和手算比。

### 框内两行文字（031）

右栏做 `判据 + 结论` 两行时，只把第二行画在框外 `yy + 34` 而下一框从 `yy + 40` 开始，第二行会被下一个框盖住（bbox 全 PASS，只有截图看得见）。稳妥参数：框高 42~46、行距 48~58、两行分别落在 `+18 / +35`，`viewBox` 同步加高，框外还有说明行的再加 20px。

### 区间条下界 Math.max(1, lo)（159）

双链表/哨兵场景里常见的 `L1 = 0` 表示「左边没有更大的位置」，直接拿 0 去换算坐标（`cellX(0) = 起始 - 一格`）会把矩形推到 x 为负，bbox 报 `left = -76`。**别用 `st.L !== null` 判断字段是否存在**——新建的 step 对象漏写该字段时是 `undefined`，`undefined !== null` 为真，会带着 `NaN` 坐标走完整段绘制逻辑（验证脚本只报一个含糊的负 bbox）。要么补全字段，要么用 `!= null` / `=== undefined` 判断。

### svg 内说明文字按句换行（159）

按 30 字硬切会把「答案 = v ^ b = 3 ^ 5」这种短语劈成两行、尾行还会断在半句话上。写一个 `wrapText(text)`：先用 `text.split('。')` 保证句子完整，单句超长时才硬切；渲染时最多画 N 行，超出在最后一行末尾加 `…`。完整说明仍然放 `.breakdown` 里，svg 里只放短摘要。

### Chrome 掉链子与启动参数

- 页数多时：跑到第 49 页左右可能偶发 `Target.createTarget` / `Runtime.evaluate` 超时。脚本已加逐页 try/catch + 重试，一页失败不会再掐死整轮；真失败了就换新的 `--user-data-dir` 只补跑剩下那几页。
- 别把 `--window-size` 加进启动参数：隐藏的顶栏图标 svg 会被判成「空 svg」。脚本已跳过 `button/a` 里的图标，但保持默认视口最省事。

### 元素截图一片白 = 元素在视口外（028）

headless 默认视口只有 ~756×469，`Page.captureScreenshot` 的 `clip` 用的是视口坐标，元素 `getBoundingClientRect().y` 超过视口高度时截出来是纯白图，而 `elText` 一切正常，容易误判「图没画」。做法：先 `Emulation.setDeviceMetricsOverride {width:1280, height:9000, deviceScaleFactor:2, mobile:false}` 撑大视口，再 `el.scrollIntoView({block:'start'})`，最后带 `clip` + `captureBeyondViewport:true` 按新 rect 截图。

### clip 在某些环境恒截空白（029）

`Page.captureScreenshot` 的 `clip` 在某些环境恒截空白，而 full shot 正常（macOS 桌面端豆包 + headless Chrome）：`clip` 底边一超出视口就报 `Invalid parameters`（`y + h > 视口高` 即炸，视口默认 ~756×469 时元素高 400 的图在 `y=400` 处就超界）；把视口撑到 2600 让 clip 完全落在视口内、滚动也生效后，clip 截图**仍然纯白**，而同一状态的 full shot 内容完全正常。判定这就是环境怪癖，别在 clip 上继续耗。**稳的通道是全页截图 + 客户端裁剪**：先 `Emulation.setDeviceMetricsOverride` 把视口高度设成 `document.documentElement.scrollHeight`，full shot 拿整页，再用 `sips -c <H> <W> --cropOffset <Y> <X>` 按元素的文档坐标（`getBoundingClientRect().left/top + scrollY`）裁出单图。

### scrollTo 是异步的（029）

`window.scrollTo` 是异步的，立即读 `getBoundingClientRect` 拿到的是滚动前的位置：实测 `scrollTo(0, 2082)` 之后立刻读 `scrollY` 还是 0，约 1 秒后才生效。凡是「先滚动再按新坐标截图」的脚本，顺序必须是 `scrollTo → sleep ≥1.5s → 再读 rect → 截图`；按滚动前的旧坐标截，元素可能落在视口外，截出来就是白图。

### 自写 CDP：onmessage 吞错误（029）

`onmessage` 里 `resolve(msg.result)` 会把 CDP 错误吞成 `undefined`：CDP 错误响应的结构是 `{id, error:{code,message}}`，没有 `result` 字段，直接 resolve 拿到的 `undefined` 会被当成「截图返回空」，实际是参数错。要改成 `if (msg.error) reject(new Error(msg.error.message)); else resolve(msg.result);`，并且 reject 时带上方法名，一眼看出是哪个调用炸的。

### node --check 报文件尾 Unexpected token（028）

先怀疑外层 IIFE 缺闭合：Write 第一版漏写收尾 `})();`，各内层块自己闭合后最外层 `(`/`{` 一直剩着，acorn 与 `node --check` 都把错误指到文件尾。排查手法：按注释标记（`// ===== Figure 4`）把脚本切成段，每段单独包 IIFE 跑 `node --check`，哪段报错就定位到哪段。顺带一个假阳性坑：**自写 CDP/提取脚本里模板字符串中的正则 `\s` 会被转义成 `\\s`**，导致「svg 无 viewBox」这类误报，先打印提取结果再下结论。

### 并发锁的凭据是 owner 文件，不是目录（043）

**误判是怎么来的。** 旧协议的判定是「`mkdir /tmp/algo-course-lock-<课号>` 成功 = 抢到锁」，也就是**拿目录的存在与否当「有没有人在做」**。链条是这样断的：会话 A 抢到锁、写完页面、把页面接进 json，收口时执行 `rm -f owner && rmdir <lockdir>` —— 但**沙箱放行删文件、不放行删目录**，`rmdir` 报 `Operation not permitted`，空目录留在 `/tmp`。会话 B 随后开工，`mkdir` 撞上 EEXIST，于是判定「有人在」并打印「<课号> 正在被另一会话转换」退出；而当时的死锁接管规则是「锁 mtime 超 60 分钟**且** json 里**还没有**该课条目」，会话 A 恰恰**已经**把该课写进 json 了，第二个条件不成立 → 接管不了。**锁永久卡死，这一课再也转不了。** 根因是抽象错了：目录只是容器，容器在不在跟「里面有没有人」没有关系。

**修法：判定改认 `owner` 文件。** 目录用 `mkdir -p` 幂等创建（不参与判定），抢锁是往 `<lockdir>/owner` 做一次 O_EXCL 创建 —— 用 `( set -o noclobber; printf '%s|%s\n' "$(date +%s)" "$SESSION" > "$LOCK/owner" ) 2>/dev/null` 实现，zsh/bash 都成立，且**失败时不会截断别人的 owner**（这一点单独验过）。抢不到才去看 owner 的年龄。于是「空目录」天然变成「无人持有」→ 直接抢到，误判消失；收口只 `rm -f owner`（这条本来就能成功），根本不用碰目录，也不会有沙箱问题。**别用 `rm -rf` 去删锁目录绕过它** —— 那只是把判定缺陷盖住，而且 `rm -rf` 会连带删掉别人正在用的 owner。

**死锁判定只看 owner 的 mtime**（`find "$LOCK/owner" -mmin +60`）。别用目录 mtime：任何对目录的读写都会刷新它，判不准。也**别再把「json 里没有该课条目」当死锁条件** —— 已完成的课 json 里必然有条目，这个条件会把它们的残留锁锁死；「这课是否已完成」是另一回事，由兜底双查的第 1 条独立负责（json 已有该课 id → 幂等跳过）。

配套改动：prompt.md 头部「并发防重」整节（含可直接粘贴的脚本），SKILL.md 两处锁说明同步。

### 其它

- **截图与「SVG 越界」口径走本技能自带的脚本，不要转去调外部技能 `ui-verify-cdp`**（2026-10-14 定：本技能自包含，不依赖项目外技能）。截图用 `scripts/shot_element.js`（单个 `.viz`，可先点几下按钮）与 `scripts/shot_steps.js`（逐帧），越界判定用 `scripts/verify_pages.js` 的 `getBBox()` 比 viewBox 那一档，两处口径一致。它们的用法与参数见 SKILL.md 第八节。
