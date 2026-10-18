import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(3, 8)
# 先随机生成树保证连通
perm = list(range(1, n+1)); random.shuffle(perm)
edges = set()
for i in range(1, n):
    u = perm[i]; v = perm[random.randint(0, i-1)]
    edges.add((min(u,v), max(u,v)))
maxe = n*(n-1)//2
extra = random.randint(0, maxe - (n-1))
while len(edges) < n - 1 + min(extra, 6):
    u = random.randint(1, n); v = random.randint(1, n)
    if u == v: continue
    edges.add((min(u,v), max(u,v)))
el = sorted(edges)
print(n, len(el))
for (u,v) in el:
    print(u, v)
q = random.randint(1, 8)
print(q)
for _ in range(q):
    x = random.randint(1, n); y = random.randint(1, n); z = random.randint(3, n)
    print(x, y, z)
