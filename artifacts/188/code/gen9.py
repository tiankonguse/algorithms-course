# 题9 生成器：随机无向图 s/t 状态
import random, sys
seed = int(sys.argv[1])
r = random.Random(seed)
n = r.randint(2, 8)
m = r.randint(1, min(8, n * (n - 1) // 2))
print(n, m)
seen = set()
cnt = 0
while cnt < m:
    u = r.randint(1, n); v = r.randint(1, n)
    if u == v: continue
    if (u, v) in seen or (v, u) in seen: continue
    seen.add((u, v))
    print(u, v, r.randint(0, 1), r.randint(0, 1))
    cnt += 1
