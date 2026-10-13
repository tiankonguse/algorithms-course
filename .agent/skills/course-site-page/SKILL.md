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

## 〇、进度与触发

- **任何「转换下一课 / 把第 NNN 课做成页面 / 批量转换 N 课」任务，第一步完整读 prompt.md**（下一课/已完成/跳过都在那里），再按本文件执行；收口时回写。
- **2026-10-03 用户定调：从小到大转（001 → 207）**。已完成 159~207 共 49 课（历史倒序产物，不回炉重排），下一课以 prompt.md 为准。
- 推进时的三种情况：
  - **导学课不产页**（001/002 等）：PPT 无可讲的方法/算法/数据结构，或 `src/classNNN/` 无代码 → 记进 prompt.md「跳过」并注明原因。
  - **方法论课要产页**（判据是「有没有可讲的方法」不是「有没有算法」：005 对数器、007 复杂度、008、019 IO、020 递归、026 容器、029 排序总结先例）。别一看到 PPT 薄、代码不是算法就记「跳过」。
  - **讲数据/概率的页面，正文数字必须自己跑实验量出来**（005 先例：抓取率、命中轮数等都是 /tmp/cppNNN 跑出来的），顺带验证代码能跑。
- **开工先查半成品**（不只查自己中断的残留）：`ls -lT algorithms/ | tail -20` + 可疑页 `grep -o '@@[A-Za-z0-9_]*@@'`、`grep -c 'figure class="fig"'`、`grep -r '<页名>.html' nav/*/*/index.json` 看是否已入 nav。中断残留往往很完整，判断归属后走「复用 + 补占位符 + 自检」，比重写省一大截。
- **同课并发互斥（抢锁，2026-10-07 恢复）**：所有「转换下一课」实例都读同一个「下一课」编号，写作期没有互斥，容易重复做同一课白烧 token。**开工第一步（读完 prompt.md 立刻）抢锁**，规则与可直接粘贴的脚本全文在 prompt.md 头部「并发防重」小节（第 9 行起）。要点：**锁的凭据是 `<lockdir>/owner` 文件、不是目录**——`mkdir -p` 幂等建容器，抢锁靠 noclobber 下 `printf ... > owner`（等价 O_EXCL，唯一成功者开工），抢不到且 owner 年龄 ≤ 60 分钟就打印「NNN 正在被另一会话转换」退出；收口最后一步只 `rm -f /tmp/algo-course-lock-<课号>/owner`（**别 rmdir，也别删目录**；目录留着是正常状态）；死锁 = `find owner -mmin +60` 命中（只看 owner mtime，见 pitfalls §8）；锁失效时兜底双查：`grep -r '<页名>.html' nav/*/*/index.json` 命中 → 已完成跳过；页面文件 mtime 60 分钟内 → 有人在做，等对方收口后只做复核（复用 + 核验，不重写、不抢写 nav/prompt.md）。
- 「转换下一课」→ **主循环直接做，不开子智能体**（见十一节）；「连转 N 课」串行，一课收口干净（nav 条目 + 重新生成的 json + prompt.md 都更新过）再开下一课。

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

**页面完全独立（2026-10-06 用户定，三条都要落实）**：

- **正文不写相邻算法链接/指代**：不写「上一篇/下一篇」「上一节/下一节」等指向相邻页的链接，页面结尾也不插任何翻页标记。相邻关系由 `nav/` 三层的数组顺序决定，`shared.js` 在每页底部按索引自动生成「上一篇/下一篇」。
- **侧边栏分组默认折叠**：`shared.js` 打开页面时只展开当前算法所在组、并把高亮项自动滚动定位到侧边栏可视区**正中**（手动算 `scrollTop` 并钳位到顶/底边界；页面内容很短无法物理居中时钳位到顶属正常）；其余分组默认收起，点击分组标题可展开/收起（用户手动展开的组会记住）。生成页面不需要为侧边栏写任何标记，也不用管分组展开状态。
- **pager 链自检照跑**：验证的是 `shared.js` 按 nav 铺平顺序生成的翻页文本，不是页面里写死的内容。

**第二批体验细节（2026-10-06 用户验收后补，实现都在共享层、页面无需任何标记）**：

- **上一篇/下一篇同一行**：`shared.js` 在每页底部渲染**一个** `nav.pager`，里面两个 `.pager-item`（上一篇/下一篇）并排同一行；只有单侧链接时渲染单个卡片（下一篇靠右对齐）。旧的「每链一个 nav」已废弃，别回退。
- **首页左侧专题目录**：`index.html` 的 `<aside id="homeNav">` 由 `shared.js`（8b-0 段）按导航数据（nav 生成的 groups）渲染——`#group-<i>` 锚点 + 序号/组名/篇数，点击平滑定位（`.group{scroll-margin-top:76px}`），滚动时 scrollspy 高亮当前组；首页分组区 `#index-list .group` 带 `id="group-<i>"`。900px 以下 `.home-nav` 自动隐藏。

**演示的控制按钮容器统一写 `class="viz-controls"`**：shot_element.js 按 `.viz-controls button` 索引点击，自造名字会点空、截图停在初始帧。shared.css 已有该样式。

**演示里要用随机数时，一律用 mulberry32，整段照抄勿默写**（005 起反复踩，027 默错一次把种子清零）：

```js
seed = (seed + 0x6D2B79F5) | 0; t = Math.imul(t ^ (t >>> 15), t | 1); t ^= t + Math.imul(t ^ (t >>> 7), t | 61); return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
```

经典 LCG（`seed * 1103515245 + 12345`）在 JS 双精度下丢低位（3000 轮重复 1144 vs mulberry32 只 683）。固定 seed 让截图/逐帧可复现，页面上配「重置」把 seed 打回去。

**演示要展示「某种结论」时，先让数据现形**（029）：改随机种子别瞎碰，用 Node 把页面同款随机流与算法原样仿真一遍，遍历种子找出让所有想展示现象都出现的那个，再写回页面（029 用 mulberry32(1) 让选择/快排/堆排全现形不稳定）。固定种子意义不只是可复现，还要保证「演示剧本」成立。

**演示出「算出来的结果」时配自写 CDP 脚本**（005）：控制区是 select/滑块时 shot_steps 点不到——先 `select.value = ...; dispatchEvent(new Event('change'))`，再按文字找按钮点击，最后截图。判断标色别只信缩略图（005 把橙色看成蓝色），读回 DOM 属性（`rect.getAttribute('fill')`）逐格核对最稳。

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
- **nav 文件并发写保护**（多会话可能同时读写）：写前 `cp` 备份到 /tmp，`json.load` → `splice` 插入 → `json.dump`，中间不夹长命令。比旧的大 json 安全得多——各会话通常写各自组目录下的小文件，但仍要：写完核对 `len(items) == 原数 + 新增数` 并重读确认自己的条目还在；核对改动用 `diff` 对比写前备份，别用 `git diff HEAD`。**开新组先写父层 `index.json` 再建目录**（反过来会被生成器报「野目录」）。忘了写 nav 的页面不会静默丢失——下次生成会报「孤儿页」。

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

**发布（WorkBuddy「发布为应用」打包本地目录）前必须先跑生成器**：线上那份 json 是静态文件，不会自己更新。`nav/` 与 `tools/` 会被一起打包，无害。发布后冒烟：`curl -s <host>/assets/data/algorithms.json | python3 -c "import json,sys;d=json.load(sys.stdin);print(len(d['algorithms']),len(d['groups']))"`。

## 八、自检（必做）

```bash
# 技能装两处：仓库 .agent/skills（随本仓库走）与 ~/.workbuddy/skills（用户级），脚本取当前加载的那份
SKILL=~/project/github/AIProject/algorithms-course/.agent/skills/course-site-page/scripts
[ -d "$SKILL" ] || SKILL=~/.workbuddy/skills/course-site-page/scripts
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
- **CDP 端口从环境变量 `CDP_PORT` 读**：换端口必须成对写 `--remote-debugging-port=940X` 与 `CDP_PORT=940X`，否则 ECONNREFUSED 易误判「Chrome 没起来」。默认用 9333（脚本默认端口）、`--user-data-dir=/tmp/cdpX`；**并发会话同跑时不要 `pkill -f "remote-debugging-port="`**（会把别人会话的 Chrome 一起杀掉），改用没人占的端口 + 独立 `--user-data-dir`；只有确认站内没有别的会话时清残留才用 pkill。
- **`verify_pages.js` / `verify_viz_steps.js` 在某些环境对任何页报全 0**（2026-10-05，028 实测，macOS 桌面端豆包 + 特定 Chrome：navigate 后 location.href 变 `chrome-error://chromewebdata/`，换端口/换 user-data-dir 都一样，同环境 verify_home.js 却正常）→ **环境怪癖不是页面问题**。自救：自写 CDP 脚本按「先 `Page.enable` 再 `Page.navigate`、轮询 `readyState` 到 complete、再 `Runtime.evaluate`」顺序连页面，检查项照抄 verify_pages 的 FRAME（figs/svg/nav/toc/viz/code/bbox），实测等价覆盖。
- 首页要单独查（verify_pages 只跑 algorithms/ 下页）→ 直接 verify_home.js；**首页没有侧边栏**，navGroups 为 0 正常，不要当 FAIL 去修。它不查字段齐全/类型对，插过新条目另跑 `python3 tools/build_nav.py --check`（13 条硬错全在这，比 Counter 管用）。
- 全站页列表直接从 nav 生成别手敲（生成物下标无意义，扫 nav 才是真值）：
  ```bash
  PAGES=$(python3 -c "import json,glob;print(' '.join(i['file'] for p in sorted(glob.glob('nav/*/*/index.json')) for i in json.load(open(p, encoding='utf-8'))))")
  $N $SKILL/verify_pages.js ${PAGES}
  ```
- **拼完页面先跑「杂字扫描」**（003 实测出现过俄文/英文杂字与同音错字）：
  ```bash
  python3 - <<'PY'
  import glob, re
  for f in glob.glob('algorithms/*.html'):
      s = open(f, encoding='utf-8').read()
      bad = sorted({c for c in s if not c.isascii() and c.isalpha() and ord(c) < 0x4E00})
      flag = []
      if bad: flag.append('杂字' + str(bad))                      # 只看 ASCII 区间外文字母
      if re.search(r'<pre class="code">\s*<pre class="code">', s): flag.append('代码块真嵌套')
      if re.search(r'@@[A-Za-z0-9_]*@@', s): flag.append('占位符')
      if flag: print(f, flag)
  PY
  ```
  说明：老页常见 `ᵢ Σ α ⁿ` 这类下标/符号会一起被扫出，属正常忽略；单独的 `</pre></pre>` 多闭合在 40 多个老页上都有、浏览器忽略，**不是自己这课的问题就别动别人的页**。
- **收口前跑「AI 味扫描」**（词表只覆盖中文套话，英文类痕迹照 humanize.md 自己读一遍；必然误报技术对比，那种留）：
  ```bash
  python3 - <<'PY'
  import glob, re, sys
  BAD = ['总而言之','毫无疑问','不可否认','综上所述','双刃剑','首先','其次','再者',
         '值得注意的是','我们可以看到','不难发现','由此可见','总体而言','基于以上',
         '不仅','赋能','底层逻辑','降维打击','矩阵','值得一提','换句话说']
  for f in sorted(glob.glob('algorithms/*.html')):
      s = open(f, encoding='utf-8').read()
      m = s[s.find('<main'):s.find('</main>')] or s
      t = re.sub(r'<[^>]+>', '', m)
      hits = [w for w in BAD if w in t]
      neg = [x.strip() for x in re.findall(r'[^。；！？\n]*?(?:不是|而是|并非)[^。；！？\n]*。', t)]
      if hits or neg:
          print(f, '| 套话:', hits or '-', '| 否定并列:', len(neg))
          for x in neg: print('    ·', x[:80])
  PY
  ```
  顺带扫：`——`（破折号）、`<strong`（通篇加粗）、emoji（正则 `[\U0001F300-\U0001FAFF]`）。**站点老页面大量存在这些问题，别顺手去改别人的页**，只对自己这轮产出的页面负责。
- 其余自检坑录（越界口径、第二演示、标注压线、末帧、截图白图、CDP 自写细节等）→ **references/pitfalls.md 第八节**，按需查。

## 九、收尾

- 更新 `algorithms-course/prompt.md` 进度：完成课挪进「已完成」，「下一课」+1（从小到大推进）；导学课挪进「跳过」并写一句原因。
- **python 替换锚点截断会把旧句残留成重复**（029 实测）：锚点只写到「* 下一课：**029**。」就整行换掉，行尾原有「003~028 已产页…」留在原地变一句两半。替换后 `grep -n '下一课：' prompt.md` 整行看一遍。
- 往 `algorithms-course/.workbuddy/memory/YYYY-MM-DD.md` 追加一条：本次建了哪几页、源自第几课、页码/组的位置。共享层有改动（shared.css/js）时写进 `memory/MEMORY.md`。

## 十、对拍与课程代码语义（2026-10-01 增）

- **课程代码 ≠ 题面直觉，先做语义假设检验，别急着改代码**（实例：BZOJ2870 虚点把单点链算进答案——先写假设检验脚本跑几百组随机小用例确认真实语义，再按它写暴力；网上题解与课程代码一致，页面按课程代码呈现，正文一句「这里有个细节」点出）。
- **强制在线的题（如 CF757G 异或 lastAns）不能随机喂原始参数对拍**：解码依赖上轮输出，随机值会解出越界下标跑崩（表现「code 输出空、brute 输出 0」的假 mismatch）。正确做法是**交互编码**——以暴力为参考实现，逐条喂解码后的操作、读输出当 lastAns、再把 `值 ^ lastAns` 写进输入，最后拿完整输入跑被测程序逐行比对。暴力输出用 `endl` 不用 `"\n"`（否则管道块缓冲卡死）。
- **沙箱本机没有 `bits/stdc++.h`**：自建 shim（/tmp/cppNNN/inc/bits/stdc++.h 里 include 常用头），编译加 `-I inc` 和 `-std=c++17`（缺后者报 `>>` 与初始化列表语法错误）。
- **课程代码把树状数组上界写成 n 的题，随机数据把值限制在 ≤ n**（171 实测：值域超 n 时写入被静默丢、判出假 MISMATCH，代码没问题；页面正文顺手点明「上界用 n，因为本题值域与 n 同阶」）。**先怀疑数据，再怀疑代码**。
- 跑对拍把输出 `grep -E 'OK|MISMATCH|FAIL'`，别只看 tail（失败详情在末尾几行）。
- **写对拍器先怀疑自己的生成器**（162 两条都踩）：节点编号基准对齐题面（P1505 是 0 基准，读入 `x++`，生成器按 1..n 会一半操作越界）；算「子树内」的 DFS 起点父亲要显式给 `par[u]`（u 是根给 0），否则真实父节点被当儿子扫进来、答案整片偏大。**MISMATCH 成片、随机、换种子复现率稳定 = 生成器形状问题，不是代码边界问题**。
- **DAG 最长路 / 最长链的暴力参考必须按拓扑序扫**（072 踩：信封、数对链、K 递增三题的暴力都是「枚举前驱取 max + 1」，随手按输入顺序扫会报成片假 MISMATCH，如 `n=7 2 vs 3`、`k=5 -2 vs 0`）。信封按「宽度升序、同宽高度升序」扫，数对链按右端点升序扫，分组题空组先 `if(size==0) return 0`。**看到 MISMATCH 先确认暴力的枚举顺序是不是一个合法拓扑序，再回头改实现**。
- **错法演示的抓取率随值域单调下降，挑小值域才现形**（072 实测：写反等号 V=3 → 86.78%、V=10 → 63.81%、V=50 → 20.30%、V=1000 → 1.10%、V=10⁶ → 0.00%；信封同宽排序、K 递增选哪个 bs 同规律）。演示里的「错误率」档位要覆盖小值域，别只给一组大随机数据——那样错版和正解答案一样，演示看着像没区别。
- **探针报 FAIL 先怀疑探针自己的参考实现**（108 实测连着踩两次）：① 「范围加 + 单点查」的答案就是差分数组的前缀和，探针又对 `a` 求了一次前缀和当期望，末帧永远差一截；② 探针重放随机算子时从 mulberry32 流里取 `v` 的位置与页面不一致，同一组算子被铺成两条不同的序列。**先用一组手算的固定小用例把「页面末帧打印值」和「参考实现的期望值」对齐，再上随机**；随机算子只写一份生成逻辑给两边共用，别各写一份。
- **演示网格要把哨兵行 / 列一并画进来**（108：二维 `tree` 是 0 基准且带第 0 行第 0 列哨兵，渲染 `a` 用 off=0、`tree` 用 off=1，混用整行显示 0；二维差分的 4 个角会落在第 n+1 行 / 列，网格不铺到 n+2 就出现「图上找不到这个更新点」）。行列的 `py(i) = Y0 + i*CS` 别从 `i-1` 起算，否则第一行 y 为负、`getBBox()` 越界。
- **降序 ends 的二分不是换个符号就行**（072：求「以 arr[j] 开头的最长不下降」时 ends 是降序，要找 `< num` 最左，和升序版找 `>=` 最左不是同一段代码改个比较符）。写这类「倒着扫」的预处理时把方向单独立一个函数（如 `bsDesc`），别复用升序版。
- **性能数字要连同 `-O0` 一起量，别只报 `-O2`**（031 实测：clang 把 BK 的 `while(n){n&=n-1;c++;}` 识别成 popcount 编成 NEON `cnt` 指令（`-S` 能看到），一千万次 0.53ms 比手写 SWAR 3.16ms 快 6 倍；关优化后 SWAR 恒 36.59ms、BK 随 1 的个数 32.15~235.08ms）。想确认是否被识别成内在函数：`-S` 后 grep 指令名。

## 十一、主循环直做模式（默认，单课与「连转N课」都走这里）

> **2026-10-02 用户定调：转换一节课不开子智能体，太浪费 token。** 现在改为**主循环直接做**。

**单课**：按一~九节完整跑一遍，跑完就收口（更新 nav、重新生成 json、更新 prompt.md、补 memory 日志）。**连转 N 课**：串行，一课做完收口干净再开下一课；上下文太满提示用户 `/compact`，不要自己开子智能体「腾地方」。

**控制上下文的小技巧**：PPT 文本别整篇 cat 进上下文（抽到 /tmp/pptNNN.txt 再 grep/sed 按段取）；源码同理（编译跑对拍、不整份读）；页面用「先 Write 带占位符 + 脚本替换」避免手写转义；收口核验只用 grep -c / ls -lT / find -newermt 这类不进上下文的检查。

**就地改写页面 HTML 的坑（016/031 实测）**：别用 `s.index(a):s.index(b)` 切片段（b 若在 a 之前先命中会切出空串，`str.replace('', new)` 把新块插进每两个字符之间，59KB 页面瞬间 80MB；写成 `i = s.index(a); j = s.index(b, i); assert i < j`，替换完立刻 `wc -c` 查体积；真炸了 `s.replace(NEW, '')` 能还原）；演示的 `rebuild()` 里**不要写 `st.step = st.steps.length - 1`**（切 select 触发 rebuild 会把演示停末帧，verify_viz_steps 只跑 1 帧看起来像没生效，rebuild 固定回第 0 步）；自写链表/数组模拟器要带保护（取不到节点置 `broken = true` 显示「链已经断了」而不是真崩）；演示随机序列要加权别等概率（016：六操作等概率 16 步整条序列一个 deleteFront 都没落到，改 40% 插/40% 删/20% 查后每个 bug 版本都能现形）。

## 十二、多会话共存的几条纪律（锁只挡同课，跨课并发仍要注意）

同一站点可能同时有几个会话在转不同的课，共享 `nav/` 目录树、生成物 `assets/data/algorithms.json` 与 `prompt.md`：

- **别人的半成品页面不要动**（哪怕占位符还没替换），等它自己收口；先确认自己这课的边界，别顺手「修好」。
- **组可能已经被别人建好**：插入前扫一遍 `grep -h '"name"' nav/*/index.json` 与各组的 `title`，同主题新课**插到组内对应位置**，不要重复建组（重复组名会让首页出现两张一样的卡片头）。
- 收口时 verify_home.js 可能因别人的中间态报 FAIL（某组还没页、组名对不上）→ 先看差异是不是自己那组，不是就放过并在 memory 记一笔。
- 写 nav 的安全纪律（cp 备份、最小改动、核条数、别用 `git diff HEAD`）见七节。**收口必须 `python3 tools/build_nav.py` 重新生成并把生成物一起提交**，否则线上不生效；别人的中间态可能让 `--check` 报 FAIL，只看自己那组。

## 十三、技能自身装多处，改完要同步

本技能有**两个物理副本**，调用时命中哪份不一定：

| 副本 | 路径 |
|---|---|
| 仓库版（随 git 走） | `~/project/github/AIProject/algorithms-course/.agent/skills/course-site-page/` |
| 用户级 | `~/.workbuddy/skills/course-site-page/` |

**`~/.agents/skills/course-site-page/` 不是第三份**——它与 `~/.workbuddy/skills/course-site-page/` 共享同一批 inode（每个文件都是硬链接，在其中一边新建/改文件，另一边立刻可见）。所以真正要同步的只有「仓库版 ↔ 用户级」这一次 `cp`；同步完 `~/.agents` 自动跟着一致，不用再管。

改了 SKILL.md 或 scripts，**两份都要落**（`cp` 过去再 `diff -rq` 确认），否则下次会话可能读到旧版，按已废弃的流程手改 json 或跑旧的自检脚本。

```bash
R=~/project/github/AIProject/algorithms-course/.agent/skills/course-site-page
U=~/.workbuddy/skills/course-site-page
cp $R/SKILL.md $U/SKILL.md            # 改哪个文件 cp 哪个
diff -rq $R $U                         # 应只剩 .DS_Store
```

CDP 类脚本（verify_pages / verify_viz_steps / shot_* / check_branch / example_controls_check）依赖同目录的 `cdp_page.js`，复制时别漏。**不知道当前会话加载的是哪份时**：`grep -n 'tools/build_nav.py' <路径>/SKILL.md` 有 7 处命中就是 nav 版。

## 附：详细坑录索引

本文件为瘦身版，完整踩坑实录（每条的完整场景与修法）在 `references/pitfalls.md`，按节组织：§4 代码高亮、§5 拼页与演示、§8 自检与截图（最长）、§11 对拍、§12 页面改写。执行中遇到本文件未展开的细节先查 pitfalls.md。
