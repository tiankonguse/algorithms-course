#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""一次性迁移脚本（2026-10）：把旧的单一 assets/data/algorithms.json 倒推成 nav/ 目录树。

**仅用于 2026-10 那次迁移，不要再次运行** —— 它会用旧 json 覆盖 nav/ 下的全部内容。
保留下来是为了留一条「从旧格式恢复」的路径。之后导航的唯一真值是 nav/，
产物由 tools/build_nav.py 生成。

用法:
    python3 tools/migrate_json_to_nav.py [旧json路径]      # 默认 assets/data/algorithms.json
"""
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NAV = os.path.join(ROOT, 'nav')

# 一级分类名 -> 目录 slug
CAT_SLUG = {
    '语言基础': 'lang',
    '算法基础': 'basics',
    '搜索': 'search',
    '动态规划': 'dp',
    '字符串': 'string',
    '数学': 'math',
    '数据结构': 'ds',
    '图论': 'graph',
    '计算几何': 'geometry',
    '杂项': 'misc',
    '专题': 'topics',
}

# 二级分类名 -> 目录 slug（名字全局唯一，slug 也全局唯一）
GROUP_SLUG = {
    # 语言基础
    '标准库容器': 'stl',
    '输入输出': 'io',
    # 算法基础
    '复杂度': 'complexity',
    '排序': 'sort',
    '二分': 'binary-search',
    '递归与分治': 'divide-conquer',
    '前缀和与差分': 'prefix-diff',
    '贪心': 'greedy',
    # 搜索
    'DFS 与 BFS': 'dfs-bfs',
    '回溯': 'backtrack',
    '双向搜索': 'bidirectional',
    '启发式搜索': 'heuristic',
    '递归与栈': 'recursion-stack',
    # 动态规划
    '动态规划入门': 'dp-intro',
    '线性 dp': 'dp-linear',
    '背包 dp': 'dp-knapsack',
    '区间 dp': 'dp-interval',
    '数位 dp': 'dp-digit',
    '状压 dp': 'dp-bitmask',
    '树形 dp': 'dp-tree',
    '二维与多维 dp': 'dp-multi',
    '转移优化': 'dp-optimize',
    '方案还原': 'dp-restore',
    '按数据量猜解法': 'dp-guess',
    '动态规划总结': 'dp-summary',
    # 字符串
    '字典树': 'trie',
    'KMP': 'kmp',
    'AC 自动机': 'ac-automaton',
    '后缀数组': 'suffix-array',
    'Manacher 与 Z 算法': 'manacher-z',
    '字符串哈希': 'string-hash',
    # 数学
    '位运算': 'bitwise',
    '数论': 'number-theory',
    '组合数学': 'combinatorics',
    '快速幂与矩阵': 'power-matrix',
    '博弈论': 'game-theory',
    '数字与进制': 'digit-base',
    # 数据结构
    '线性表': 'linear-list',
    '栈与队列': 'stack-queue',
    '哈希表': 'hash-table',
    '堆': 'heap',
    '二叉树': 'binary-tree',
    '单调栈': 'mono-stack',
    '单调队列': 'mono-queue',
    '并查集': 'dsu',
    '树状数组': 'fenwick',
    '线段树': 'segtree',
    '线段树分治': 'segtree-divide',
    '可持久化数据结构': 'persistent',
    '分块': 'block',
    'K-D 树': 'kd-tree',
    '树套树': 'tree-of-tree',
    '动态树': 'lct',
    # 图论
    '图的存储与可达性': 'graph-storage',
    '拓扑排序': 'topo-sort',
    '最短路': 'shortest-path',
    '最小生成树': 'mst',
    '连通性': 'connectivity',
    '欧拉图': 'euler',
    '2-SAT': 'two-sat',
    '优化建图': 'graph-opt',
    '最近公共祖先': 'lca',
    '树链剖分': 'hld',
    '虚树': 'virtual-tree',
    '树分治': 'tree-divide',
    '树上启发式合并': 'dsu-on-tree',
    '基环树与仙人掌': 'cactus',
    # 杂项
    '离散化': 'discretize',
    '双指针': 'two-pointer',
    'CDQ 分治': 'cdq',
    '离线查询': 'offline-query',
    '莫队算法': 'mo',
    '整体二分': 'overall-bin',
    '根号分治': 'sqrt-divide',
    '打表与构造': 'construct',
    '对数器与猜解': 'validator',
    '概率与海量数据': 'prob-bigdata',
}


def write_json(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        f.write(json.dumps(data, ensure_ascii=False, indent=2))
        f.write('\n')


def main():
    src = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'assets', 'data', 'algorithms.json')
    old = json.load(open(src, encoding='utf-8'))

    missing_cat = [c for c in old['categories'] if c not in CAT_SLUG]
    missing_grp = [g['name'] for g in old['groups'] if g['name'] not in GROUP_SLUG]
    if missing_cat or missing_grp:
        sys.exit('slug 表缺项：一级 %s / 二级 %s' % (missing_cat, missing_grp))

    dup = [s for s in GROUP_SLUG.values() if list(GROUP_SLUG.values()).count(s) > 1]
    if dup:
        sys.exit('slug 表内部重复：%s' % sorted(set(dup)))

    # 页面按 group 分桶；桶内保持旧 algorithms 数组的相对顺序
    by_group = {}
    for a in old['algorithms']:
        by_group.setdefault(a['group'], []).append(a)

    # 一级：nav/index.json
    write_json(os.path.join(NAV, 'index.json'),
               [{'dir': CAT_SLUG[c], 'name': c, 'desc': ''} for c in old['categories']])

    for cat in old['categories']:
        cat_dir = CAT_SLUG[cat]
        subs = [g for g in old['groups'] if g['cat'] == cat]
        # 二级列表：顺序源是旧 groups 数组
        write_json(os.path.join(NAV, cat_dir, 'index.json'),
                   [{'dir': GROUP_SLUG[g['name']], 'name': g['name'], 'desc': g['desc']}
                    for g in subs])
        for g in subs:
            pages = [{'file': a['file'].split('/')[-1],
                      'title': a['title'],
                      'description': a['description'],
                      'level': a['level']}
                     for a in by_group.get(g['name'], [])]
            write_json(os.path.join(NAV, cat_dir, GROUP_SLUG[g['name']], 'index.json'), pages)

    n_files = sum(1 for _, _, fs in os.walk(NAV) for f in fs)
    print('已生成 nav/：一级 %d、二级 %d、index.json %d 个'
          % (len(old['categories']), len(old['groups']), n_files))


if __name__ == '__main__':
    main()
