import random, sys

seed = int(sys.argv[1]); which = int(sys.argv[2])
rnd = random.Random(seed)

def connected_edges(n, rnd):
    # 随机树保证连通 + 额外随机边
    es = []
    for v in range(2, n + 1):
        es.append((rnd.randint(1, v - 1), v))
    extra = rnd.randint(0, min(8, n * (n - 1) // 2 - (n - 1)))
    for _ in range(extra):
        u = rnd.randint(1, n); v = rnd.randint(1, n)
        if u == v: v = 1 if u != 1 else 2
        es.append((u, v))
    rnd.shuffle(es)
    return es

def fmt(es):
    return '\n'.join(f'{u} {v}' for u, v in es)

n = rnd.randint(2, 8)
if which == 1:
    es = connected_edges(n, rnd)
    s, t = 1, n
    ws = [rnd.randint(0, 1) for _ in es]
    print(n, len(es))
    for (u, v), w in zip(es, ws):
        print(u, v, w)
    print(s, t)
elif which == 2:
    es = connected_edges(n, rnd)
    print(n, len(es)); print(fmt(es))
elif which == 3:
    es = connected_edges(n, rnd)
    print(n, len(es))
    for u, v in es:
        print(u, v, rnd.randint(1, 10))
elif which == 4:
    es = connected_edges(n, rnd)
    # 加一些重边（两点间可多连）
    for _ in range(rnd.randint(0, 4)):
        u = rnd.randint(1, n); v = rnd.randint(1, n)
        if u != v: es.append((u, v))
    q = rnd.randint(1, 6)
    print(n, len(es)); print(fmt(es))
    print(q)
    for _ in range(q):
        print(rnd.randint(1, n), rnd.randint(1, n))
elif which == 5:
    es = connected_edges(n, rnd)
    q = rnd.randint(1, 6)
    print(n, len(es))
    print(' '.join(str(rnd.randint(1, 20)) for _ in range(n)))
    print(fmt(es))
    print(q)
    for _ in range(q):
        print(rnd.randint(1, n), rnd.randint(1, n))
elif which == 6:
    # 可多连通区：树 + 额外边，可能断
    n2 = rnd.randint(2, 8)
    es = []
    for v in range(2, n2 + 1):
        es.append((rnd.randint(1, v - 1), v))
    extra = rnd.randint(0, 6)
    for _ in range(extra):
        u = rnd.randint(1, n2); v = rnd.randint(1, n2)
        if u == v: v = 1 if u != 1 else 2
        es.append((u, v))
    rnd.shuffle(es)
    q = rnd.randint(1, 6)
    print(n2, len(es), q); print(fmt(es))
    for _ in range(q):
        print(rnd.randint(1, n2), rnd.randint(1, n2))
elif which == 7:
    es = connected_edges(n, rnd)
    print(n, len(es)); print(fmt(es))
