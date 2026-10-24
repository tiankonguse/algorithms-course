import random, sys

def tree_edges(n, rnd):
    es = set()
    for i in range(2, n + 1):
        es.add((rnd.randint(1, i - 1), i))
    return es

def connected_edges(n, rnd):
    es = tree_edges(n, rnd)
    extra = rnd.randint(0, min(8, n * (n - 1) // 2 - (n - 1)))
    for _ in range(extra):
        u = rnd.randint(1, n); v = rnd.randint(1, n)
        if u != v:
            es.add((min(u, v), max(u, v)))
    return sorted(es)

def main():
    seed, which = int(sys.argv[1]), int(sys.argv[2])
    rnd = random.Random(seed)
    n = rnd.randint(2, 8)
    if which == 1:
        m = rnd.randint(0, min(15, n * (n - 1) // 2))
        es = set()
        while len(es) < m:
            u, v = rnd.randint(1, n), rnd.randint(1, n)
            if u != v: es.add((min(u, v), max(u, v)))
        print(n, m)
        for u, v in sorted(es): print(u, v)
    elif which == 2:
        m = rnd.randint(0, min(15, n * (n - 1) // 2))
        es = set()
        while len(es) < m:
            u, v = rnd.randint(0, n - 1), rnd.randint(0, n - 1)
            if u != v: es.add((min(u, v), max(u, v)))
        print(n, m)
        for u, v in sorted(es): print(u, v)  # 0-indexed（课程读入 +1）
        print(0, 0)
    elif which == 3:
        es = connected_edges(n, rnd)
        print(n)
        for u, v in es: print(u, v)
        print(0, 0)
        a, b = 1, n
        print(a, b)
    elif which == 4:
        m = rnd.randint(0, 15)
        print(n, m)
        for _ in range(m):
            u = rnd.randint(1, n); v = rnd.randint(1, n)
            print(u, v)  # 允许自环
    elif which == 5:
        m = rnd.randint(0, min(15, n * (n - 1) // 2))
        es = set()
        while len(es) < m:
            u, v = rnd.randint(1, n), rnd.randint(1, n)
            if u != v: es.add((min(u, v), max(u, v)))
        print(n, m)
        for u, v in sorted(es): print(u, v)
    elif which == 6:
        es = connected_edges(n, rnd)
        print(n, len(es))
        for u, v in es: print(u - 1, v - 1)  # 0-indexed
        print(' '.join(str(rnd.randint(1, 20)) for _ in range(n)))
    elif which == 7:
        es = connected_edges(n, rnd)
        print(len(es))
        for u, v in es: print(u, v)
        print(0)
    elif which == 8:
        allp = [(i, j) for i in range(1, n + 1) for j in range(i + 1, n + 1)]
        rnd.shuffle(allp)
        m = rnd.randint(0, len(allp))
        print(n, m)
        for u, v in allp[:m]: print(u, v)
        print(0, 0)

main()
