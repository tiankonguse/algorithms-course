# 题10 生成器：随机有向小图 + 序列
import random, sys
seed = int(sys.argv[1])
r = random.Random(seed)
n = r.randint(2, 5)
seen = set()
m = r.randint(1, min(5, n * (n - 1)))
cnt = 0
edges = []
while cnt < m:
    u = r.randint(1, n); v = r.randint(1, n)
    if u == v: continue
    if (u, v) in seen: continue
    seen.add((u, v))
    edges.append((u, v))
    cnt += 1
t = r.randint(0, 2)
seqs = []
for _ in range(t):
    k = r.randint(1, 3)
    s = [r.randint(1, n) for _ in range(k)]
    seqs.append(s)
print(n, m)
for u, v in edges: print(u, v)
print(t)
for s in seqs:
    print(len(s), *s)
