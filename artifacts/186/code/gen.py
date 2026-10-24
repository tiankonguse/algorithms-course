#!/usr/bin/env python3
# 生成随机树 + 随机 LCA 查询（P3379 格式：n m root, n-1 边, m 查询）
import random, sys

seed = int(sys.argv[1])
r = random.Random(seed)
n = r.randint(2, 60)
m = r.randint(1, 80)
root = r.randint(1, n)
out = [f"{n} {m} {root}"]
par = [0] * (n + 1)
edges = []
for i in range(2, n + 1):
    p = r.randint(1, i - 1)
    par[i] = p
    edges.append((p, i))
r.shuffle(edges)
for u, v in edges:
    out.append(f"{u} {v}")
# 树邻接 + 深度（供暴力）
adj = [[] for _ in range(n + 1)]
for u, v in edges:
    adj[u].append(v); adj[v].append(u)
dep = [0] * (n + 1)
def dfs(u, fa):
    for v in adj[u]:
        if v != fa:
            dep[v] = dep[u] + 1
            dfs(v, u)
dfs(root, 0)
for _ in range(m):
    u = r.randint(1, n)
    v = r.randint(1, n)
    out.append(f"{u} {v}")
print('\n'.join(out))
