# 题11 生成器：n<=6、标记边<=3、起点s
import random, sys
seed = int(sys.argv[1])
r = random.Random(seed)
n = r.randint(2, 6)
s = r.randint(1, n)
m = r.randint(0, min(3, n * (n - 1) // 2))
seen = set()
pairs = []
while len(pairs) < m:
    a = r.randint(1, n); b = r.randint(1, n)
    if a == b: continue
    key = tuple(sorted((a, b)))
    if key in seen: continue
    seen.add(key)
    pairs.append(key)
print(n, m, s)
for a, b in pairs: print(a, b)
