import random, sys

seed = int(sys.argv[1])
tid = int(sys.argv[2])
rnd = random.Random(seed)

if tid == 1:  # 遗产 CF786B
    n = rnd.randint(2, 8); q = rnd.randint(1, 12); s = rnd.randint(1, n)
    print(n, q, s)
    for _ in range(q):
        op = rnd.randint(1, 3)
        w = rnd.randint(1, 10)
        if op == 1:
            x = rnd.randint(1, n); y = rnd.randint(1, n)
            print(op, x, y, w)
        elif op == 2:
            x = rnd.randint(1, n)
            l = rnd.randint(1, n); r = rnd.randint(1, n)
            if l > r: l, r = r, l
            print(op, x, l, r, w)
        else:
            x = rnd.randint(1, n)
            l = rnd.randint(1, n); r = rnd.randint(1, n)
            if l > r: l, r = r, l
            print(op, x, l, r, w)
elif tid == 2:  # 旅程 P6348（保证整体连通）
    for _try in range(200):
        n = rnd.randint(2, 8); m = rnd.randint(1, 8); p = rnd.randint(1, n)
        ops = []
        adj = [set() for _ in range(n + 1)]
        for _ in range(m):
            a = rnd.randint(1, n); b = rnd.randint(1, n); c = rnd.randint(1, n); d = rnd.randint(1, n)
            if a > b: a, b = b, a
            if c > d: c, d = d, c
            ops.append((a, b, c, d))
            for u in range(a, b + 1):
                for v in range(c, d + 1):
                    adj[u].add(v); adj[v].add(u)
        vis = [0] * (n + 1); vis[p] = 1
        qu = [p]
        for u in qu:
            for v in adj[u]:
                if not vis[v]: vis[v] = 1; qu.append(v)
        if all(vis[1:]):
            print(n, m, p)
            for op in ops: print(*op)
            break
    else:
        raise RuntimeError('生成连通图失败')
elif tid == 3:  # 道路 HDU5669
    print(1)
    n = rnd.randint(2, 8); m = rnd.randint(1, 8); k = rnd.randint(0, 3)
    print(n, m, k)
    for _ in range(m):
        a = rnd.randint(1, n); b = rnd.randint(1, n); c = rnd.randint(1, n); d = rnd.randint(1, n); w = rnd.randint(1, 10)
        if a > b: a, b = b, a
        if c > d: c, d = d, c
        print(a, b, c, d, w)
elif tid == 4:  # 沙漠 P3588
    n = rnd.randint(2, 6); s = rnd.randint(0, 3); m = rnd.randint(1, 5)
    print(n, s, m)
    used = set()
    for _ in range(s):
        x = rnd.randint(1, n); v = rnd.randint(1, 20)
        print(x, v)
    for _ in range(m):
        l = rnd.randint(1, n); r = rnd.randint(l, n)
        k = rnd.randint(1, min(3, r - l + 1))
        pos = sorted(rnd.sample(range(l, r + 1), k))
        print(l, r, k, *pos)
elif tid == 5:  # 炸弹 P5025
    n = rnd.randint(2, 10)
    print(n)
    loc = sorted(rnd.sample(range(-20, 21), n))
    for i in range(n):
        ri = rnd.randint(0, 15)
        print(loc[i], ri)
elif tid in (6, 7):  # 美丽的树 CF1904F
    n = rnd.randint(2, 7); m = rnd.randint(1, 6)
    print(n, m)
    for v in range(2, n + 1):
        u = rnd.randint(1, v - 1)
        print(u, v)
    for _ in range(m):
        op = rnd.randint(1, 2)
        a = rnd.randint(1, n); b = rnd.randint(1, n)
        # 树上唯一路径取一个点 c
        # 先直接取 a 或 b 或中间：简化取 a
        c = rnd.choice([a, b])
        print(op, a, b, c)
elif tid == 8:  # 逛森林 P5344
    n = rnd.randint(2, 8); m = rnd.randint(1, 14); s = rnd.randint(1, n)
    print(n, m, s)
    for _ in range(m):
        op = rnd.randint(1, 2)
        w = rnd.randint(1, 10)
        if op == 1:
            a = rnd.randint(1, n); b = rnd.randint(1, n); c = rnd.randint(1, n); d = rnd.randint(1, n)
            print(op, a, b, c, d, w)
        else:
            u = rnd.randint(1, n); v = rnd.randint(1, n)
            print(op, u, v, w)
elif tid == 9:  # 监狱 P9520
    while True:
        n = rnd.randint(2, 8); m = rnd.randint(1, 4)
        if n >= 2 * m: break
    print(1)
    print(n)
    for v in range(2, n + 1):
        u = rnd.randint(1, v - 1)
        print(u, v)
    # 卧室互异、工作室互异、x!=y
    beds = rnd.sample(range(1, n + 1), m)
    rest = [x for x in range(1, n + 1) if x not in beds]
    rnd.shuffle(rest)
    works = rest[:m]
    print(m)
    for i in range(m):
        print(beds[i], works[i])
