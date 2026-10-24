#!/usr/bin/env python3
# 194 课对拍生成器：python3 gen194.py <seed> <题号>  n=2..8
import sys, random

seed = int(sys.argv[1])
which = int(sys.argv[2])
rnd = random.Random(seed)

def conn_graph(n, allow_multi=False, no_selfloop=False):
    # 随机连通图（无重边），返回边列表 1-indexed
    es = set()
    # 随机生成树
    order = list(range(2, n + 1))
    rnd.shuffle(order)
    for v in order:
        u = rnd.randint(1, v - 1)
        es.add((min(u, v), max(u, v)))
    # 附加边
    max_extra = min(n * (n - 1) // 2 - (n - 1), 4 * n)
    extra = rnd.randint(0, max_extra)
    cand = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1) if (u, v) not in es]
    rnd.shuffle(cand)
    for e in cand[:extra]:
        es.add(e)
    return sorted(es)

def forest_graph(n, allow_multi=False):
    # 随机森林+随机边（可多连通区），无重边
    es = set()
    for v in range(2, n + 1):
        if rnd.random() < 0.8:
            u = rnd.randint(1, v - 1)
            es.add((min(u, v), max(u, v)))
    max_extra = min(n * (n - 1) // 2 - len(es), 3 * n)
    extra = rnd.randint(0, max(0, max_extra))
    cand = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1) if (u, v) not in es]
    rnd.shuffle(cand)
    for e in cand[:extra]:
        es.add(e)
    return sorted(es)

n = rnd.randint(2, 8)

if which == 1:
    es = conn_graph(n)
    m = len(es)
    print(n, m)
    for u, v in es: print(u, v)
    q = rnd.randint(1, 6)
    print(q)
    for _ in range(q):
        x = rnd.randint(1, n)
        y = rnd.randint(1, n)
        print(x, y)

elif which == 2:
    es = conn_graph(n)
    m = len(es)
    print(n, m)
    for u, v in es: print(u, v)
    q = rnd.randint(1, 6)
    print(q)
    for _ in range(q):
        print(rnd.randint(1, n), rnd.randint(1, n), rnd.randint(1, n))

elif which == 3:
    while n < 3: n = rnd.randint(2, 8)  # 需要 3 个不同点
    es = conn_graph(n)
    m = len(es)
    a, b, c = rnd.sample(range(1, n + 1), 3)
    print(n, m, a, b, c)
    for u, v in es: print(u, v)

elif which == 4:
    es = conn_graph(n)
    m = len(es)
    print(n, m)
    for u, v in es: print(u, v)

elif which == 5:
    es = forest_graph(n)
    m = len(es)
    print(n, m)
    for u, v in es: print(u, v)

elif which == 6:
    es = conn_graph(n)
    m = len(es)
    print(n, m)
    for u, v in es: print(u, v)
    q = rnd.randint(1, 6)
    print(q)
    for _ in range(q):
        x, y = rnd.sample(range(1, n + 1), 2)
        print(x, y)

elif which == 7:
    es = conn_graph(n)
    m = len(es)
    print(n, m)
    for u, v in es: print(u, v)
    q = rnd.randint(1, 6)
    print(q)
    for _ in range(q):
        if rnd.random() < 0.5:
            # 删边：选一条存在的边 c d
            c, d = rnd.choice(es)
            print(1, rnd.randint(1, n), rnd.randint(1, n), c, d)
        else:
            print(2, rnd.randint(1, n), rnd.randint(1, n), rnd.randint(1, n))

elif which == 8:
    es = conn_graph(n)
    m = len(es)
    q = rnd.randint(1, 6)
    print(n, m, q)
    print(' '.join(str(rnd.randint(1, 10)) for _ in range(n)))
    for u, v in es: print(u, v)
    for _ in range(q):
        if rnd.random() < 0.5:
            x = rnd.randint(1, n)
            print('C', x, rnd.randint(1, 10))
        else:
            x, y = rnd.sample(range(1, n + 1), 2)
            print('A', x, y)

elif which == 9:
    t = 2
    print(t)
    for _ in range(t):
        n2 = rnd.randint(2, 8)
        # 可有重边
        es = conn_graph(n2)
        # 加一点重边
        if n2 >= 2 and rnd.random() < 0.4:
            u, v = rnd.sample(range(1, n2 + 1), 2)
            es.append((min(u, v), max(u, v)))
        m = len(es)
        print(n2, m)
        for u, v in es: print(u, v)
        q = rnd.randint(1, 4)
        print(q)
        for _ in range(q):
            s = rnd.randint(1, min(4, n2))
            pts = rnd.sample(range(1, n2 + 1), s)
            print(s, ' '.join(map(str, pts)))

elif which == 10:
    es = conn_graph(n)
    # 允许重边
    if n >= 2 and rnd.random() < 0.4:
        u, v = rnd.sample(range(1, n + 1), 2)
        es.append((min(u, v), max(u, v)))
    m = len(es)
    q = rnd.randint(1, 6)
    print(n, m, q)
    for u, v in es: print(u, v)
    for _ in range(q):
        print(rnd.randint(1, n))
