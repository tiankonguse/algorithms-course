# 题1/2 生成器：有向/无向 小图
# d: P7771 输入 n m + m 行有向边；u: P2731 输入 m + m 行无向边
import random, sys
seed = int(sys.argv[1])
kind = sys.argv[2]  # 'd' 有向 / 'u' 无向
r = random.Random(seed)
n = r.randint(2, 6)
edges = []
if kind == 'd':
    maxe = n * n
    m = r.randint(1, min(7, maxe))
    for _ in range(m):
        u = r.randint(1, n); v = r.randint(1, n)
        edges.append((u, v))
    print(n, m)
    for u, v in edges:
        print(u, v)
else:
    maxe = n * (n - 1) // 2
    m = r.randint(1, min(6, maxe))
    seen = set()
    while len(edges) < m:
        u = r.randint(1, n); v = r.randint(1, n)
        if u == v: continue
        if (u, v) in seen or (v, u) in seen: continue
        seen.add((u, v))
        edges.append((u, v))
    print(m)
    for u, v in edges:
        print(u, v)
