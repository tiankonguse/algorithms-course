import random, sys
random.seed(int(sys.argv[1]))
n = random.randint(2, 10); m = random.randint(5, 20)
print(n, m)
ws = [random.randint(1, 10) for _ in range(n)]
print(' '.join(map(str, ws)))
edges = []
for v in range(2, n+1):
    u = random.randint(1, v-1)
    edges.append((u, v))
    print(u, v)
g = [[] for _ in range(n+1)]
for u, v in edges:
    g[u].append(v); g[v].append(u)
w = [0] + ws
lastAns = 0
ops = []
for _ in range(m):
    if random.random() < 0.6:
        x = random.randint(1, n); y = random.randint(1, n-1)
        d = [-1]*(n+1); d[x] = 0; stack=[x]
        while stack:
            u = stack.pop()
            for v in g[u]:
                if d[v] < 0: d[v] = d[u]+1; stack.append(v)
        s = sum(w[u] for u in range(1, n+1) if d[u] <= y)
        ops.append(f'0 {x ^ lastAns} {y ^ lastAns}')
        lastAns = s
    else:
        x = random.randint(1, n); y = random.randint(1, 10)
        ops.append(f'1 {x ^ lastAns} {y ^ lastAns}')
        w[x] = y
print('\n'.join(ops))
