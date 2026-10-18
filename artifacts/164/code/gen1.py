import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 7)
maxe = n * (n - 1) // 2
m = random.randint(0, min(12, maxe))
edges = set()
while len(edges) < m:
    u = random.randint(1, n); v = random.randint(1, n)
    if u == v: continue
    edges.add((min(u,v), max(u,v)))
print(n, m)
for (u,v) in sorted(edges):
    print(u, v, random.randint(1, 15))
q = random.randint(1, 10)
print(q)
for _ in range(q):
    print(random.randint(1,n), random.randint(1,n))
