# 题10 生成器：保证有欧拉回路的图 + 可能无解的序列混合
import random, sys
seed = int(sys.argv[1])
r = random.Random(seed)
for _ in range(2000):
    n = r.randint(2, 5)
    m = r.randint(3, 6)
    seen = set(); edges = []
    for _ in range(m):
        u = r.randint(1, n); v = r.randint(1, n)
        if u == v or (u, v) in seen:
            continue
        seen.add((u, v)); edges.append((u, v))
    if len(edges) < 3: continue
    # 欧拉回路条件：出度=入度、从1可达（含孤立点除外——所有出现点都可达）
    outd = [0]* (n+1); ind = [0]*(n+1)
    for u, v in edges: outd[u] += 1; ind[v] += 1
    if any(outd[i] != ind[i] for i in range(1, n+1)): continue
    # 连通性（有边点都从 1 可达）
    g = [[] for _ in range(n+1)]
    for u, v in edges: g[u].append(v)
    seen2 = {1}
    stack = [1]
    while stack:
        x = stack.pop()
        for y in g[x]:
            if y not in seen2: seen2.add(y); stack.append(y)
    if any(outd[i] > 0 for i in range(1, n+1) if i not in seen2): continue
    # 建一条已知欧拉回路（Hierholzer）用于取子段
    gg = [list(es) for es in [[] for _ in range(n+1)]]
    for u, v in edges: gg[u].append(v)
    # 课程风格 Hierholzer（迭代）
    for i in range(1, n+1): gg[i].sort()
    path = []
    cur = [0]*(n+1)
    stack = [1]
    while stack:
        x = stack[-1]
        if cur[x] < len(gg[x]):
            y = gg[x][cur[x]]; cur[x] += 1
            stack.append(y)
        else:
            path.append(x); stack.pop()
    circuit = list(reversed(path))
    # 序列：一半取自回路子段（必有解），一半随机（可能无解）
    appear = sorted({x for u, v in edges for x in (u, v)})
    seqs = []
    for _ in range(2):
        k = r.randint(1, 3)
        if r.random() < 0.5:
            st = r.randint(0, len(circuit) - k)
            seqs.append(circuit[st:st+k])
        else:
            seqs.append([r.choice(appear) for _ in range(k)])
    break
else:
    raise SystemExit('未找到欧拉图')
print(n, len(edges))
for u, v in edges: print(u, v)
print(len(seqs))
for sq in seqs:
    print(len(sq), *sq)
