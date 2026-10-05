# algorithms-course

初中算法兴趣课教程，纯静态、无构建。

## 目录结构

```
index.html              首页（卡片列表，由导航 json 渲染）
algorithms/             一个算法一个 HTML 页面
assets/
  css/shared.css        全站共用样式
  js/shared.js          导航 / 目录渲染
  data/algorithms.json  算法导航索引（新增算法只改这里）
run.sh                  本地静态服务
```

## 运行

```bash
./run.sh          # 启动，访问 http://localhost:8080/index.html
./run.sh stop
```

`shared.js` 用 fetch 读导航 json，直接双击 `file://` 打开会被 CORS 拦截，必须走 run.sh。

## 新增一个算法

1. 在 `algorithms/` 下新建 `<id>.html`，头部引 `../assets/css/shared.css`、尾部引 `../assets/js/shared.js`。
2. 在 `assets/data/algorithms.json` 的 `algorithms` 里加一条：`id`、`file`（`algorithms/<id>.html`）、`title`、`description`、`group`（必须是 `groups` 里已有的组名）、`level`（1 入门 / 2 进阶 / 3 高阶）。
3. `algorithms` 数组的顺序就是首页和侧边栏的展示顺序，插到对应组的位置即可；要开新组，先在 `groups` 里加一条（`name`、`desc`）。

侧边栏、首页卡片、目录都按这份 json 自动生成，不用改别的地方。`group` 写错或漏写的算法会落到末尾的「其他」组，遇到时先检查组名拼写。
