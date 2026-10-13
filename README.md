# algorithms-course

初中算法兴趣课教程，纯静态、无构建。

## 目录结构

```
index.html              首页（卡片列表，由导航 json 渲染）
algorithms/             一个算法一个 HTML 页面
nav/                    导航真值：三层目录树，每层一个 index.json
tools/build_nav.py      递归扫 nav/ 生成导航 json
assets/
  css/shared.css        全站共用样式
  js/shared.js          导航 / 目录渲染
  data/algorithms.json  导航索引（**由 nav/ 生成，勿手改**）
run.sh                  本地静态服务（启动时先跑 build_nav.py）
```

## 运行

```bash
./run.sh          # 启动（先生成导航），访问 http://localhost:8080/index.html
./run.sh stop
```

`shared.js` 用 fetch 读导航 json，直接双击 `file://` 打开会被 CORS 拦截，必须走 run.sh。

## 导航怎么组织

真值是 `nav/` 目录树，**每层只管自己这一层**：

```
nav/index.json                         [{dir, name, desc}]  一级分类，数组顺序即展示顺序
nav/<cat>/index.json                   [{dir, name, desc}]  该一级下的二级分类
nav/<cat>/<group>/index.json           [{file, title, description, level}]  该二级下的页面
```

目录名用英文 slug，中文显示名与备注写在**父层**的 json 里（不存两份）。页面条目的 `file` 只写文件名，实际路径由生成器拼成 `algorithms/<file>`。

改完跑 `python3 tools/build_nav.py` 重新生成 `assets/data/algorithms.json`（`run.sh` 启动时会自动跑）。只校验不写用 `--check`，警告当错误用 `--strict`。

## 新增一个算法

1. 在 `algorithms/` 下新建 `<slug>.html`，头部引 `../assets/css/shared.css`、尾部引 `../assets/js/shared.js`。
2. 往所属二级的 `nav/<cat>/<group>/index.json` 里插一条，四个字段：`file`（`xxx.html`）、`title`、`description`、`level`（1 入门 / 2 进阶 / 3 高阶 / 4 挑战 / 5 硬核，**必须是数字**）。
3. `python3 tools/build_nav.py --check` —— 少字段、`file` 拼错、页面没收录都会被拦下。
4. 提交时把重新生成的 `assets/data/algorithms.json` 一起提交（线上只认这份生成物）。

组内插在哪一位，就是首页卡片、侧边栏、上下篇的顺序。要开新组：先在该一级的 `index.json` 里登记 `{dir, name, desc}`，再建目录写页面列表。
