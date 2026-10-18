import random, subprocess
from collections import deque
random.seed(20261018)

def path_nodes(adj, a, b):
    par = [-1] * len(adj)
    q = deque([a]); par[a] = 0
    while q:
        u = q.popleft()
        if u == b: break
        for v in adj[u]:
            if par[v] == -1:
                par[v] = u; q.append(v)
    res = []
    u = b
    while u != 0:
        res.append(u); u = par[u]
    return res

def subtree_nodes(adj, x, frm):
    res = [x]
    for v in adj[x]:
        if v != frm:
            res += subtree_nodes(adj, v, x)
    return res

# 复现 r5
r = 5
n = random.randint(2, 8); m = random.randint(5, 30)
edges = []
for v in range(2, n + 1):
    u = random.randint(1, v - 1); edges.append((u, v))
adj = [[] for _ in range(n + 1)]
for u, v in edges:
    adj[u].append(v); adj[v].append(u)
arr = [0] + [random.randint(1, 100) for _ in range(n)]
root = random.randint(1, n)
cur = arr[:]
ops = []
for _ in range(m):
    op = random.randint(1, 3)
    if op == 1:
        root = random.randint(1, n)
        ops.append(('R', root))
    elif op == 2:
        x = random.randint(1, n); y = random.randint(1, n); v = random.randint(1, 100)
        ops.append(('U', x, y, v))
        for nd in path_nodes(adj, x, y): cur[nd] = v
    else:
        x = random.randint(1, n)
        par = [-1] * (n + 1)
        q = deque([root]); par[root] = 0
        while q:
            u = q.popleft()
            for v in adj[u]:
                if par[v] == -1:
                    par[v] = u; q.append(v)
        sub = subtree_nodes(adj, x, par[x])
        ops.append(('Q', x, sorted(sub), min(cur[nd] for nd in sub)))
print('n,m =', n, m, 'edges =', edges)
print('arr =', arr[1:])
print('root0 =', ops[0] if False else None)
lines = [f"{n} {m}"]
for u, v in edges: lines.append(f"{u} {v}")
lines.append(' '.join(map(str, arr[1:])))
rt0 = None
oplines = []
for o in ops:
    if o[0] == 'R': rt0 = o[1] if rt0 is None else rt0; opline = f"1 {o[1]}"
    elif o[0] == 'U': opline = f"2 {o[1]} {o[2]} {o[3]}"
    else: opline = f"3 {o[1]}"
    oplines.append(opline)
# root 初始行是单独读的：gen 里 root 在 arr 后、op 前
# 但 debug 里 ops 是从初始 root 开始生成的吗？我上面直接用了 root 变量——重跑一遍生成但保留 ops 无法还原 root0。
# 修正：重新生成完整一轮
