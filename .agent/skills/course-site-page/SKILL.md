---
name: course-site-page
description: 把 algorithm-journey 的课件（PPT + Java 源码里的 C++ 版）整理成 algorithms-course 站点的一门算法页面——抽文本、抽代码、生成高亮片段、按占位符拼页、接入索引、CDP 自检。触发词：把第 NNN 课做成页面、整理这门课、转换下一课、批量转换N课、算法课件转网页、algorithm-journey 生成本站页面。
agent_created: true
---

# 课件 → 站点算法页

把左程云 `algorithm-journey` 课件的某一讲，做成 `algorithms-course` 站点上的一个/几个算法页面。

## 实测过的目标

- 站点根：`~/project/github/AIProject/algorithms-course`；课件根：`~/project/github/AIProject/algorithm-journey`
- **开工前必读**：`algorithms-course/prompt.md`（进度/下一课/约定权威）与 `algorithms-course/.workbuddy/memory/MEMORY.md`（站点约定权威），缺一不可，别凭印象写。本文件是流程，prompt.md 是状态，互相关联。
- **prompt.md 已瘦身（2026-10-16）**：每课详细记录（题目拆页、对拍数字、踩坑、自检）按课号拆到 `algorithms-course/progress/NNN.md`（如 `progress/128.md`），prompt.md 只留头部、进度指针、跳过与已完成列表。要参考某课先例按课号读对应文件，不要全量加载历史。
- **本文件是唯一自动加载的那份**（2026-10-14 瘦身后 27KB），细节全在 `references/`，**按需读**：读哪份看文末「按需读索引」表，不要一次把 `references/` 全读进来。
- **本技能自包含（2026-10-14）**：脚本、参考资料（写作范本四篇、`humanize.md` 去 AI 味规则、分节坑录 `pitfalls-*.md`）全在本目录下，跑流程**不依赖项目外的任何技能**——不再调用 `ui-verify-cdp`（截图与越界用本目录 `scripts/shot_*.js` / `verify_pages.js`）、不再调用 `去AI味` / `humanizer`（规则已内联在 `references/humanize.md`）。脚本也只用 node 内置模块 + 同目录 `cdp_page.js`，不读 `node_modules`。

## 〇、进度与触发

- **任何「转换下一课 / 把第 NNN 课做成页面 / 批量转换 N 课」任务，第一步完整读 prompt.md**（下一课/已完成/跳过都在那里），再按本文件执行；收口时回写。某课的详细记录在 `progress/NNN.md`，需要参考先例（同主题、相邻课）时按课号读，别整批加载。
- **2026-10-03 用户定调：从小到大转（001 → 207）**。159~207 是倒序阶段旧版产物（原「不回炉重排」；**2026-10-18 用户改定**：升序推进到这些课时走「〇·五 二次转换」——参考旧页与旧 progress 完善，不是跳过也不是从零重写），下一课以 prompt.md 为准。
- 推进时的四种情况：
  - **导学课不产页**（001/002 等）：PPT 无可讲的方法/算法/数据结构，或 `src/classNNN/` 无代码 → 记进 prompt.md「跳过」并注明原因。
  - **方法论课要产页**（判据是「有没有可讲的方法」不是「有没有算法」：005 对数器、007 复杂度、008、019 IO、020 递归、026 容器、029 排序总结先例）。别一看到 PPT 薄、代码不是算法就记「跳过」。
  - **讲数据/概率的页面，正文数字必须自己跑实验量出来**（005 先例：抓取率、命中轮数等都是 /tmp/cppNNN 跑出来的），顺带验证代码能跑。
  - **已有旧版的课（159~207 倒序产物）**：nav 里能查到该课页面但编号 == 「下一课」→ 不是「已完成」，走「〇·五 二次转换」：先读旧页 + progress（如有）再完善。
- **开工先查半成品**（不只查自己中断的残留）：`ls -lT algorithms/ | tail -20` + 可疑页 `grep -o '@@[A-Za-z0-9_]*@@'`、`grep -c 'figure class="fig"'`、`grep -r '<页名>.html' nav/*/*/index.json` 看是否已入 nav。中断残留往往很完整，判断归属后走「复用 + 补占位符 + 自检」，比重写省一大截。
- **同课并发互斥（抢锁）**：开工第一步（读完 prompt.md 立刻）抢锁，**可直接粘贴的脚本全文在 prompt.md 头部「并发防重」小节（第 9 行起），照抄那段，别自己写**。三条要点：凭据是 `/tmp/algo-course-lock-<课号>/owner` 文件不是目录（`mkdir -p` 建容器，noclobber 下 `printf > owner` 抢）；抢不到且 owner ≤ 60 分钟就退出；收口只 `rm -f owner`（别 rmdir）。死锁判定与兜底双查见 `pitfalls-selfcheck.md`。
- 「转换下一课」→ **主循环直接做，不开子智能体**（见十一节）；「连转 N 课」串行，一课收口干净（nav 条目 + 重新生成的 json + prompt.md 都更新过）再开下一课。

## 〇·五、二次转换：该课已有旧版页面（159~207）

> **2026-10-18 用户定**：159~207 是倒序阶段转过的旧版（页面都在 nav、`algorithms/` 下有 html，progress 文件只有 159~173 与 175，174 与 176~207 没写 progress 但页面都在）。升序推进到这些课时是**第二次转换**：**不是跳过、也不是从零重写**——**先按一~三节重读原始来源材料（PPT + src/classNNN/ 源码），再对照旧页**，以来源为准按当前标准完善。旧页只是已生成产物（草稿），**不是核对权威**——来源材料才是，旧页与来源偏差大的章节按来源重写；只在旧页基础上改而不回来源核对，偏差只会越改越大。旧页文件名尽量沿用（拆页有出入时按新标准重排并同步 nav）。

**什么时候走这里**：下一课编号落在 prompt.md「倒序阶段已完成」清单（159~207），且 nav 里已能查到该课页面。**页面存在 ≠ 已完成**，别被「nav 已存在 → 跳过」带偏；只有「编号 < 当前下一课」才是真·已完成（清单里 151~157 已在升序阶段重转过，按指针规则自然跳过）。

**开工读两份材料：原始来源（核对权威）+ 旧页（草稿）**。省 token，别整页 cat：

1. **原始来源**（同全新转换一~三节，缺一不可）：
   - `ppt/算法讲解NNN【难度】标题.pptx` → 抽到 `/tmp/pptNNN.txt`：题目清单、数据范围、测试链接、算法思路，与旧页题单/正文逐项核对；
   - `src/classNNN/` 源码 → 按头注释认 java/C++ 版、awk 抽 C++：代码语义、边界处理，旧页代码与课程代码不一致的按课程代码改（低号课无 C++ 版就按 Java 自改，照一~三节）。
2. **旧页与旧记录**：
   - `progress/NNN.md`：159~173 与 175 有倒序记录，174 与 176~207 没有——没有就跳过这步，别当缺失报错；有记录的，对拍数字要抽查复现。
   - 该课页名清单：`grep -rn '"file"' nav/*/*/index.json` 反查（或从 progress 文件抄）；再 `grep -o '<h[12][^>]*>[^<]*' algorithms/<页名>.html` 拿章节骨架、`grep -c 'figure class="fig"\|class="viz'` 看图示与演示规模。

**以来源为准的核对清单（旧页与来源冲突一律以来源为准）**：

- **题单与拆页**：旧页题目清单对照 PPT 讲稿与 Java 头注释（「测试链接 : URL」），漏题补、错题改、拆页不合理的按六节重拆。
- **代码**：旧页 C++ 对照 src 的 C++/Java 版语义，不一致的按课程代码改；改完照十节对拍验证（174、176~207 没对拍记录的一并补跑）。
- **数字**：正文的数据范围、复杂度、示例数字对照 PPT；旧页跑过的对拍数字抽查复现。
- **演示与 SVG 复用前先核语义**：旧页演示数据也可能语义错（157 的 tags 数据踩过），对照来源确认后再复用。
- **顺序**：背景与推进按升序教学序理顺，但来源里的关键要点一个不能丢（改顺序 ≠ 删内容）。

**完善目标（对照来源核对后，重点改结构与约定，不是重画图）**：

- **结尾段标题统一「总结」**：旧版有的写「最后」（pstrie-basic 先例），站内现行统一「总结」，放「总结」之后是固定两小节「相关题目 / 参考材料」。
- **删跨页指代**：倒序期正文写过「讲解 N」「前置是 XX（讲解 N）」这类指代（159/161/167 的 progress 有记录），按「页面完全独立」约定改成直接陈述。
- **教学顺序**：旧版是倒序产物，背景可能先出现高阶再铺垫基础；升序后前置课已在站内，背景与推进路线按教学序理顺。
- **组归属与组内顺序**：倒序期「新组放全站最末」「插在 XX 之前」的布点按升序重排（组内按课号升序），归属不合理的挪组，两处 index.json 都要动（父层登记 + 组内条目）。
- **nav 条目**：四字段齐全、level 是数字；旧条目缺字段/字符串难度顺手改（`build_nav.py --check` 拦）。
- **复用已验证资产**：题单、对拍数字、演示、SVG 图保留——它们对拍过、截图核对过，重写反而引入新错；改的是上面几类结构与约定。

**收口与全新转换相同**：`build_nav.py --check` + 生成、CDP 自检四项（verify_pages / verify_viz_steps / verify_home）、scan_typos + scan_ai_style、`progress/NNN.md` 更新（顶部注明「二次转换：参考旧页 xx.html、旧 progress」）、prompt.md 指针 +1、memory 日志。

**别为改而改**：159~175 阶段就跑过自检，旧页可能已大体合规；但「讲解 N」指代、结尾段标题这两类硬约定要逐页 grep 确认（`grep -c '讲解 [0-9]' <页名>.html`、`grep -o '<h2>最后</h2>' <页名>.html`），命中再改。

## 〇·六、材料归档：每课一个目录 + 页面入口（2026-10-19 用户定）

转换过程的中间产物很珍贵——PPT 抽取文本、Java→C++ 转换结果、对拍/压测程序与结果，以前丢 `/tmp`（重启即清）。现在**每课归档到站点 `artifacts/<课号>/`**，对应算法页底部由 shared.js 自动给出「本课材料」入口。

**收口时必跑（全新转换与二次转换都一样，放在 CDP 自检之后、提交之前）**：

```bash
python3 tools/archive_course.py <课号> <页1.html> <页2.html> ...
```

脚本做的事：`/tmp/ppt<课号>.txt` → `artifacts/<课号>/ppt.txt`；`/tmp/cpp<课号>/` 下非 `.bin` 文件 → `artifacts/<课号>/code/`（含对拍器 `paipai*.py/.out/.log` 与辅助脚本）；生成 `index.json`（课号/标题/页面清单/文件清单）与 `index.html`（材料查看页：PPT 全文、C++ 源码、对拍程序与结果**内联**展示，另附原始文件下载链接与本站页面回链）；最后重生成 `assets/data/artifacts.json`（页面 → 归档入口映射）。

- **页面入口是 shared.js 统一注入的，页面本身不写任何标记**：算法页加载时查 `assets/data/artifacts.json`，当前页有归档就在 pager 之前插一个「本课材料」链接（指向 `../artifacts/<课号>/index.html`）；没有归档的页面不显示、不报错。改 shared.js 后必须跑一遍 verify_pages 确认无 JS 错。
- 目录结构：`artifacts/<NNN>/{ppt.txt, code/*, index.json, index.html}`。`.bin` 编译产物不入库；`artifacts/` 与 `assets/data/artifacts.json` 都要提交（发布时一起打包）。
- **回填**：历史课 003~130 的 /tmp 产物已被系统清掉，无法回填；131~169 的 /tmp 还在，已用 `python3 tools/archive_course.py --backfill` 批量回填（页名清单从 git 提交反查，自动跳过锁仍被占用的课）。
- 映射重生成：run.sh 启动已内置 `python3 tools/build_artifacts.py`；手动发布前与 build_nav.py 一起跑（见七节）。

## 一、定位材料

```
algorithm-journey/ppt/算法讲解NNN【难度】标题.pptx     # 每页含讲稿
algorithm-journey/src/classNNN/CodeXX_*.java           # 同题 4 份：2 Java + 2「C++ 塞进注释」
```

- 用 `head` 看文件头注释认「java版 / C++版」，**别按奇偶号猜**（168 课是 1/2 号 Java、3/4 号 C++）。题目清单、数据范围、测试链接在 PPT 与 Java 头注释里互证。
- **低号课（001~）材料另一个样**：目录往往只有一个 Java 文件（文件名是题目名不是 Code0X_），**没有「C++ 塞进注释」版**，有的课无代码（空目录 → 按〇节导学课不产页）。本站代码统一 C++，**自己按 Java 改写成 C++**（`#include <bits/stdc++.h>` + `using namespace std`，照站内其它页写法），改完照样编译 + 对拍（十一节）。有的课有两份 Java（ACM + LeetCode 风格）但都无 C++ 版，同样自改（028 先例）。

## 二、抽 PPT 文本

```python
import zipfile, re
import xml.etree.ElementTree as ET
z = zipfile.ZipFile(pptx)
slides = sorted([n for n in z.namelist() if re.match(r'ppt/slides/slide\d+\.xml$', n)],
                key=lambda x: int(re.search(r'(\d+)', x.split('/')[-1]).group(1)))
P = '{http://schemas.openxmlformats.org/presentationml/2006/main}sp'
A = '{http://schemas.openxmlformats.org/drawingml/2006/main}t'
for n in slides:
    root = ET.fromstring(z.read(n))
    for sp in root.iter(P):
        print(''.join(t.text or '' for t in sp.iter(A)).strip())
```

一次拿全（含讲稿），注意标题/讲稿/页码粘在一起需自己切。

## 三、抽 Java 里的 C++

「C++ 版」两份文件的正文是 C++ 每行加 `//` 后塞进注释的（编号不固定，以头注释为准）：

```bash
awk '/^\/\/#include/{f=1} f&&/^\/\//{sub(/^\/\//,"");print;next} f{print}' Code0X_yyy2.java > Code0X_yyy.cpp
```

`-e` 不能省（否则行首空格被吃掉，缩进少一格）。抽出的自带 `//` 注释，本站在页面上不放注释，正常。

## 四、代码高亮片段

```bash
CPP_SRC=/tmp/cppNNN HL_OUT=/tmp/hlNNN/out python3 $SKILL/gen_code.py
```

要点：关键字集合照站内已用（`int if void return for else while false bool const using nullptr namespace true long char` + `#include`；**`std` 和单独的 `include` 不高亮**）；`#include` 必须先换哨兵、跑关键词正则、最后还原成 `<span class="kw">#include</span>`（否则 `include` 被二次包裹）；函数名只高亮定义处（`\b(类型)\s+(\w+)\s*\(`，哨兵 `\x01…\x02` 保护）；带尖括号返回类型（`pair<int,int> key(...)`）补独立规则且在转义后匹配（`>` 已变 `&gt;`）；单行注释先切出（按引号状态判断），最后拼 `<span class="cm">`。

## 五、拼页面

先 Write 一版带 `@@片段名@@` 占位符的 HTML，再替换：

```bash
HL_OUT=/tmp/hlNNN/out python3 $SKILL/apply_placeholders.py algorithms/xxx.html
```

- 占位符名**必须和片段文件名一致**（先对一遍名字再跑）。占位符外面**不要自己套 `<pre class="code">`**——gen_code.py 产出已是完整一对 `<pre class="code">…</pre>`，再套会双层嵌套双渲染；已写错的表现是页面出现 `</pre></pre>`，一次替换修回：`<pre class="code">    <pre class="code">` → `<pre class="code">`、`</pre></pre>` → `</pre>`。
- 页面骨架单独一份，别抄错：

```html
<div class="layout">
  <aside class="sidebar" id="sidebar"></aside>
  <main class="main">
    <section class="algo-section">
      <h1>…</h1>
      <p class="intro">…</p>
      <h2>零、背景</h2>
      …
    </section>
  </main>
  <aside class="toc-panel"><p class="toc-title">目录</p><ul class="toc-list" id="toc"></ul></aside>
</div>
<script src="../assets/js/shared.js"></script>
<script>(function(){ /* 页面独占的交互演示 */ })();</script>
```

侧边栏、目录、翻页、代码复制全由 `shared.js` 注入，页面里**一行都不要自带**。

**页面完全独立（2026-10-06 用户定，三条都要落实）**：正文不写相邻算法链接/指代（「上一篇/下一篇」都不写，翻页由 shared.js 按 nav 顺序生成）；侧边栏分组默认折叠（只展开当前组，shared.js 负责，页面不写标记）；pager 链自检照跑（验的是 shared.js 生成的文本）。

**演示与绘图四条硬规则**（完整场景与修法在 `references/pitfalls-build.md`，踩了再去查）：

- 控制按钮容器统一 `class="viz-controls"`（脚本按索引点击，自造名字会点空）。
- 随机数一律 mulberry32（整段照抄勿默写，027 默错过一次），固定 seed + 配「重置」。
- 绘图 helper 名字极短（`T/R/LN/mount`），**变量别撞名**（`L/R/T/W/H` 全禁用，`H` 还会撞哈希函数，画布高写 `HH`）；撞名时整块空白而自检照样 PASS。
- 画图预算按**最长预设**算，select 全组合跑一遍；文字重叠只有逐对 `getBBox()` 能发现。

### 写作风格（去 AI 味）——写每一段时就照这个来

**完整 24 类清单在 references/humanize.md**（Wikipedia: Signs of AI writing，含意义拔高/-ing 伪分析/宣传腔/模糊归因/AI 高频词/copula 规避/否定并列/三段式/同义词轮换/破折号/通篇加粗/内联标题列表/emoji/套话结语…），**写页面时和收口前各读一遍**；references/humanize-readme.md 是速览索引。两条冲突以本站为准：引号用「」，章节标题是「零、一、二…」。下面是最容易犯、用户逐条点名确立的几条：

- **开头不铺垫**。别写「零、背景」去罗列后面讲什么（那是 intro 的活）。直接抛反直觉事实或具体坑（例：「int 最大正数是 2147483647，最小负数是 -2147483648，负数那边多一个」）。
- **结尾必有「总结」段**（2026-10-04 用户定）：具体要点列出来 + 一句前瞻，不空谈；「标志着/意味着/让我们一起」这类升华句禁用。
- **正文不提课件材料与语言来源**（2026-10-04 用户定）：背景与正文不出现「PPT」「课件」「左程云」等来源词；代码部分不提「Java」「JDK」，直接给 C++ 实现。
- **「不是 X，是 Y」否定并列是最隐蔽的 AI 句式，收口必须 grep**。判据：**在两个具体选项里挑一个的留**（「写 `1u << i` 而不是 `1 << i`」「向下取整而不是向零取整」），**拿它当修辞强调的一律删**（「这不是冷知识，是会让你算错的东西」）。
- **禁用**：emoji、破折号 `——`、通篇 `<strong>`、`「」`以外引号、「首先/其次/总而言之/值得注意的是/不难发现/不仅…而且/赋能/底层逻辑」；别用「手滑」这类网络造词，直接写「写错」。
- **要有立场**。允许吐槽、直接下结论（「这笔买卖划算得很」「编译器一句话都不说」）；不许端水、不许「一方面…另一方面」。
- 段落长短不齐，允许单句成段；用具体数字和场景替代抽象判断（写「`printf("%08x")` 比打 32 个 0/1 省事得多」，别写「可读性更好」）；讲设计时主动回答读者会有的反问（讲补码接住「最高位当符号、剩下 31 位写绝对值不就行了？」→ 那样 0 会冒 +0/-0，加法还得先比符号再比大小）；自己抛反问后先接「确实…」再转折；关键观察编号（1）2）3））。
- **改第二轮以上时先 grep 关键词防新旧段撞车**（同一个知识点两处各讲一遍，如「第 31 位是符号位」写了两段）。

**范本对齐：四篇 tiankonguse.github.io OI 文章在 references/**（2025-12-20-ds-st-rmq / 2025-12-23-oi-dfs-seq / 2025-12-25-oi-lca / 2025-12-29-oi-virtual-tree），写作前翻一遍：背景只定位 2~3 段不罗列章节；问题驱动推进（朴素 → 正解，每迈一步抛代价：O(n²)、O(nh)）；讲不下去或读者疑惑处直接抛反问再答；用「如果…就…」评估代价引出设计选择；关键观察用「可以发现/观察/其实只需要」带出可编号；复杂度跟着推导走不单开大节；每节末一句定型小结（概念定型不是升华）；「最后」= 注意事项或「要点+适用+前瞻」；代码紧贴所讲小节不堆文末（函数名大写开头、`typedef long long ll`）；口吻「我们」一起推，不出现「本文」。

**页面末尾固定两个小节（2026-10-06 用户定，放「总结」之后、页面最后，h2 自动进 toc）**：

- **相关题目**：一张 `table.complexity` 三列表（平台 / 题目 / 链接），列本页覆盖的每一道题。数据来源：Java 源码头注释「测试链接 : URL」与 PPT 题目清单；平台按链接域名判（`leetcode.cn/com`→LeetCode、`luogu.com.cn`→洛谷、`codeforces.com`→Codeforces、`nowcoder.com`→牛客、`acwing.com`→AcWing、`spoj.com`→SPOJ、`atcoder.jp`→AtCoder、`uoj.ac`→UOJ、`hduoj`→HDU、`poj.org`→POJ，未知域名显示可读域名）。链接写 `<a href="URL" target="_blank">中文题目名</a>`（如「N 皇后 II」）。
- **参考材料**：一张 `table.complexity` 三列表（来源 / 名字 / 链接）。第一行固定主参考：来源「左程云」，名字「算法讲解NNN【难度】标题」（照 PPT 文件名，如「算法讲解003【入门】二进制和位运算」），链接固定 `https://github.com/algorithmzuo/algorithm-journey/`（课件无算法独立页，统一指向仓库根）。其他参考材料（OI-wiki、题解博客等）有具体页面链接时列具体链接，没有就不列。
- 源码无测试链接的方法论课只加参考材料（005/007/008/019/020/026/029 先例）。格式基准：`algorithms/n-queens.html` 末尾两个小节。

## 六、一讲拆几页

- 一讲 ≤4 题且技巧同源 → 一页（先例：lct-subtree、lct-graph）；明显分两半（如「LCT 维护边权」+「线段树分治」）→ 两页（lct-edge + lct-segtree）。
- **每页至少 2 张内联 SVG 图示 + 1 个可点开的演示**，否则判不达标。

## 七、接入索引与翻页顺序

**导航真值是 `nav/` 目录树，`assets/data/algorithms.json` 是生成物，不要手改**（2026-10 改版；`run.sh` 启动时自动跑 `tools/build_nav.py`）。加页面改 `nav/<一级>/<二级>/index.json`。

**新条目固定四个字段，缺一个或类型写错首页渲染 undefined**（2026-10-04 用户定；006/007/010/011/012 缺字段、linked-list-reverse level 写字符串都踩过）：

- `file`：页面**文件名**（`xxx.html`，不带目录，路径由生成器拼成 `algorithms/xxx.html`）；`title`；`description`（**字段名是 description，不是 desc**）；`level`：**数字 1~5**（1 入门/2 进阶/3 高阶/4 挑战/5 硬核），**不能写字符串「入门」**。2026-10 起不再有 `id` 与 `group` 字段——归属由目录决定，`group` 由生成器从目录名反查。

收口必跑（verify_home.js 只查卡片数和组名，查不出缺字段、类型错）：

```bash
python3 tools/build_nav.py --check      # 只校验不写；--strict 把警告也当错误
python3 tools/build_nav.py --stats      # 打印 406 / 77 / 9 与难度分布
```

- 新页插到对应组的**正确顺序位置**（组内按课程编号排，即首页卡片/侧栏/翻页顺序）。**页面正文不需要也不允许写相邻页链接或指代**——上一节/下一节的翻页由 shared.js 按铺平顺序自动生成（2026-10-06 用户定：页面完全独立）。首页不用动，卡片由 json 生成。
- **显示顺序看 `nav/` 三层 json 的数组顺序**：`nav/index.json` 定一级 → `nav/<cat>/index.json` 定二级 → 组内 `index.json` 定页面。生成物里 `algorithms` 数组的下标没有意义（生成器按组输出，必然重排），**定位页面按 `(group, file)`，不要按下标**。
- 改完**必须重新生成并一起提交**生成物，否则线上不生效：`python3 tools/build_nav.py`。
- **nav 文件并发写保护**：写前 `cp` 备份到 /tmp，`json.load` → 插入 → `json.dump`，中间不夹长命令；写完核对 `len(items) == 原数 + 新增数` 并重读确认自己的条目还在；核改动用 `diff` 对比写前备份，**别用 `git diff HEAD`**（整份重写会报上千行假改动）。**开新组先写父层 `index.json` 再建目录**（反过来报「野目录」）。忘了写 nav 不会静默丢——下次生成报「孤儿页」。

**加一页的标准动作**（三步，别的层一个字都不用碰）：

```bash
# 1 建页（algorithms/ 平铺，不进子目录）
# 2 往所属组的 json 插一条四字段
python3 - <<'PY'
import json
p = 'nav/graph/topo-sort/index.json'
d = json.load(open(p, encoding='utf-8'))
d.insert(2, {'file': 'xxx.html', 'title': '标题', 'description': '一句话', 'level': 2})
json.dump(d, open(p, 'w', encoding='utf-8'), ensure_ascii=False, indent=2)
PY
# 3 生成 + 提交（生成物必须入库，否则线上不生效）
python3 tools/build_nav.py --check && python3 tools/build_nav.py && git add nav assets/data/algorithms.json
```

**发布（WorkBuddy「发布为应用」打包本地目录）前必须先跑生成器**：线上那份 json 是静态文件，不会自己更新。`nav/` 与 `tools/` 会被一起打包，无害；`artifacts/` 也会一起打包，是归档材料的静态入口。发布前跑 `python3 tools/build_nav.py && python3 tools/build_artifacts.py`（run.sh 启动时已自动跑）。发布后冒烟：`curl -s <host>/assets/data/algorithms.json | python3 -c "import json,sys;d=json.load(sys.stdin);print(len(d['algorithms']),len(d['groups']))"`，再 `curl -s <host>/assets/data/artifacts.json | python3 -c "import json,sys;d=json.load(sys.stdin);print(len(d))"` 看归档映射非空。

## 八、自检（必做）

```bash
# 技能只有一份：仓库 .agent/skills（随 git 走）。2026-10-14 起 ~/.agents/skills 那份已删除
SKILL=~/project/github/AIProject/algorithms-course/.agent/skills/course-site-page/scripts
CHROME="/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"
N=~/.workbuddy/binaries/node/versions/$(cat ~/.workbuddy/binaries/node/versions/current)/bin/node   # 版本号随机器，读 current 拼出

# 一条命令里「起 + 用」：Chrome 随 Bash 命令结束被回收，跨命令连调试端口必 ECONNREFUSED
rm -rf /tmp/cdpX; "$CHROME" --headless=new --no-sandbox --disable-gpu \
  --remote-debugging-port=9333 --user-data-dir=/tmp/cdpX \
  --allow-file-access-from-files about:blank >/dev/null 2>&1 &
sleep 8
$N $SKILL/verify_pages.js a.html b.html       # 四项：JS 错误 / fig·svg 数 / nav·toc·viz / svg 越界；并打印 pager 链
$N $SKILL/verify_viz_steps.js a.html b.html   # 交互演示逐帧查越界与空图
$N $SKILL/verify_home.js                      # 首页 + 目录页：卡片数 / 组名顺序 / JS 错误（期望值读生成物 json）
$N $SKILL/shot_element.js a.html ".viz" /tmp/s.png 0 5   # 看图（第几个 .viz、点 .viz-controls 第几个按钮，只点一次）
$N $SKILL/shot_steps.js a.html 0 4 /tmp/s.png            # 多帧截图（页面、viz 序号、点几次「下一步」、输出）
```

- 用 `file://` + `--allow-file-access-from-files`，shared.js 才能 fetch json；沙箱 Chrome 连不上 localhost。
- **CDP 端口从环境变量 `CDP_PORT` 读**：换端口必须成对写 `--remote-debugging-port=940X` 与 `CDP_PORT=940X`，否则 ECONNREFUSED 易误判「Chrome 没起来」。**并发会话同跑时不要 `pkill -f "remote-debugging-port="`**（会把别人的 Chrome 一起杀掉），改用没人占的端口 + 独立 `--user-data-dir`；收工时清自己起的 headless Chrome（`pgrep -fl 'MacOS/Google Chrome' | grep -v Helper`，只杀带 `--headless` 的）。
- **报全 0 /「no viz」= 路径没拼对**（传 `algorithms/x.html` 会被拼成 `algorithms/algorithms/x.html`，Chrome 打开的是自己的报错页，四项检查全部静默通过只剩一堆 0）。脚本已能认出 `chrome-error` 并直接打印打不开的 url，看到 0/0 先读那一行。
- 首页要单独查（verify_pages 只跑 algorithms/ 下页）→ 直接 verify_home.js；**首页没有侧边栏**，navGroups 为 0 正常，不要当 FAIL 去修。它不查字段齐全/类型对，插过新条目另跑 `python3 tools/build_nav.py --check`（13 条硬错全在这）。
- 全站页列表直接从 nav 生成别手敲（生成物下标无意义，扫 nav 才是真值）：
  ```bash
  PAGES=$(python3 -c "import json,glob;print(' '.join(i['file'] for p in sorted(glob.glob('nav/*/*/index.json')) for i in json.load(open(p, encoding='utf-8'))))")
  $N $SKILL/verify_pages.js ${PAGES}     # zsh 要写 ${=PAGES} 才会按空格拆词
  ```
  **只传文件名，别带 `algorithms/` 前缀**（脚本自己会拼）：带上前缀会拼出重复路径，见上一条。
- **拼完页面先跑杂字扫描，收口前再跑 AI 味扫描**（两个脚本，2026-10-14 从本文内联块抽出来的，别再手敲 python）：
  ```bash
  python3 $SKILL/scan_typos.py a.html b.html      # 杂字 / 占位符残留 / 代码块真嵌套（不带参数 = 全站）
  python3 $SKILL/scan_ai_style.py a.html b.html   # 中文套话 / 「不是X是Y」否定并列 / 破折号 / strong / emoji
  ```
  老页自带的 `ᵢ Σ α ⁿ` 与 `</pre></pre>` 会被扫出来，属正常忽略；**只对自己这轮产出的页面负责，别顺手改别人的页**。否定并列必然误报技术对比（「写 `1u << i` 而不是 `1 << i`」这种留着）。
- 其余自检坑录（越界口径、第二演示、标注压线、末帧、截图白图、CDP 自写细节等）→ **按需读 `references/pitfalls-selfcheck.md`**，别整份读坑录。

## 九、收尾

- 更新 `algorithms-course/prompt.md` 进度：完成课挪进「已完成」，「下一课」+1（从小到大推进）；导学课挪进「跳过」并写一句原因。**详细记录写进 `algorithms-course/progress/NNN.md`（每课一个文件，文件头一行 `* NNN 课产出 X 页…`，正文格式照旧）**，prompt.md 不放正文。
- **归档材料（〇·六）**：`python3 tools/archive_course.py <课号> <页1.html> ...`，产物 `artifacts/<课号>/` + `assets/data/artifacts.json` 随提交入库。全新转换与二次转换都要做。
- **python 替换锚点截断会把旧句残留成重复**（029 实测）：锚点只写到「* 下一课：**029**。」就整行换掉，行尾原有「003~028 已产页…」留在原地变一句两半。替换后 `grep -n '下一课：' prompt.md` 整行看一遍。
- 往 `algorithms-course/.workbuddy/memory/YYYY-MM-DD.md` 追加一条：本次建了哪几页、源自第几课、页码/组的位置。共享层有改动（shared.css/js）时写进 `memory/MEMORY.md`。

## 十、对拍与课程代码语义

七条要点，完整场景与修法在 `references/pitfalls-verify.md`（写对拍器或报 MISMATCH 时读那一份）：

- **课程代码 ≠ 题面直觉**，先跑几百组随机小用例确认真实语义，再按它写暴力；页面按课程代码呈现，正文一句「这里有个细节」点出。
- **强制在线的题（CF757G 异或 lastAns）要交互编码**：以暴力为参考实现逐条喂解码后的操作、读输出当 lastAns，暴力输出用 `endl` 不用 `"\n"`。
- **先怀疑数据，再怀疑代码**（树状数组上界写成 n 的题，随机值要限制在 ≤ n），**再怀疑自己的生成器**（编号基准、DFS 起点父亲），最后才回头看实现。
- **DAG 最长路 / 最长链的暴力必须按拓扑序扫**，看到 MISMATCH 先确认暴力的枚举顺序是不是合法拓扑序。
- 跑对拍把输出 `grep -E 'OK|MISMATCH|FAIL'`，别只看 tail。
- **错法演示挑小值域才现形**（抓取率随值域单调下降），演示的「错误率」档位要覆盖小值域。
- **探针报 FAIL 先怀疑探针自己**：先用一组手算固定小用例对齐「页面末帧值」与「参考实现期望值」，再上随机；随机算子只写一份给两边共用。
## 十一、主循环直做模式（默认，单课与「连转N课」都走这里）

> **2026-10-02 用户定调：转换一节课不开子智能体，太浪费 token。** 现在改为**主循环直接做**。

**单课**：按一~九节完整跑一遍，跑完就收口（更新 nav、重新生成 json、更新 prompt.md、写 `progress/NNN.md`、补 memory 日志）。**连转 N 课**：串行，一课做完收口干净再开下一课；上下文太满提示用户 `/compact`，不要自己开子智能体「腾地方」。**二次转换（〇·五）同样主循环直做**：读旧材料 → 改页 → 收口一条龙，不开子智能体；「连转 N 课」碰到旧版课照走二次转换。

**控制上下文的小技巧**：PPT 文本别整篇 cat 进上下文（抽到 /tmp/pptNNN.txt 再 grep/sed 按段取）；源码同理（编译跑对拍、不整份读）；页面用「先 Write 带占位符 + 脚本替换」避免手写转义；收口核验只用 grep -c / ls -lT / find -newermt 这类不进上下文的检查。

**就地改写页面 HTML 前先读 `references/pitfalls-rewrite.md`**（016/031 实测：`s.index(a):s.index(b)` 顺序写反会把 59KB 页面撑到 80MB；`rebuild()` 里设末帧会让演示只跑 1 帧；演示随机序列要加权不等概率）。三条都在那份里。

## 十二、多会话共存的几条纪律（锁只挡同课，跨课并发仍要注意）

同一站点可能同时有几个会话在转不同的课，共享 `nav/` 目录树、生成物 `assets/data/algorithms.json` 与 `prompt.md`：

- **别人的半成品页面不要动**（哪怕占位符还没替换），等它自己收口。
- **组可能已经被别人建好**：插入前扫一遍 `grep -h '"name"' nav/*/index.json` 与各组的 `title`，同主题新课插到组内对应位置，不重复建组。
- 收口时 verify_home.js 可能因别人的中间态报 FAIL → 先看差异是不是自己那组，不是就放过并在 memory 记一笔。
- **收口必须 `python3 tools/build_nav.py` 重新生成并把生成物一起提交**（写 nav 的安全纪律见七节）。

## 十三、技能只装一份（2026-10-14 收敛）

本技能**唯一副本**在仓库里，随 git 走：

| 副本 | 路径 |
|---|---|
| 仓库版（唯一） | `~/project/github/AIProject/algorithms-course/.agent/skills/course-site-page/` |

改 SKILL.md / scripts **直接改这份就行，不用再 cp 同步**（历史上挂过 `~/DoubaoWork/skills/` → `~/.agents/skills/` → 仓库+用户级两份，2026-10-14 已全部收敛，home 那份备份在 `/tmp/course-site-page-home-backup-20261014.tar.gz`）。本技能**不依赖项目外的任何技能**（见开头「本技能自包含」），整个目录打包带走就能用。

CDP 类脚本依赖同目录 `cdp_page.js`，别单独搬走一个文件用。**怀疑读到了旧版时**：`grep -c 'tools/build_nav.py' <本 SKILL.md>` 得 8 处即当前 nav 版。

## 附：按需读索引（本文件是瘦身版，细节都在 `references/`）

**别一次把 `references/` 全读进来**（全读约 80KB）。只在下面这一步真需要时读对应那一份：

| 什么时候 | 读哪个 |
|---|---|
| 写正文前（风格） | `2025-12-20-ds-st-rmq.md` 等范本四篇挑一篇翻；`humanize.md` 是完整 24 类 AI 味清单（收口前查） |
| 拼页面 / 做演示 | `pitfalls-build.md`（§5） |
| 跑 CDP 自检 / 截图出怪事 | `pitfalls-selfcheck.md`（§8） |
| 写对拍器 / 报 MISMATCH | `pitfalls-verify.md`（§11） |
| 就地改写已有页面 | `pitfalls-rewrite.md`（§12） |
| 不确定读哪个 | `pitfalls.md`（索引表，1KB） |
