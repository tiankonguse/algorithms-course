# 190 课统一生成器：python3 gen190.py <seed> <题号>
import random, sys

seed = int(sys.argv[1])
which = int(sys.argv[2])
r = random.Random(seed)

if which == 1:
    n = r.randint(3, 10)
    m = r.randint(0, min(25, n * (n - 1)))
    out = [f"{n} {m}"]
    out.append(' '.join(str(r.randint(0, 20)) for _ in range(n)))
    edges = set()
    while len(edges) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        edges.add((a, b))
    for a, b in sorted(edges):
        out.append(f"{a} {b}")
    print('\n'.join(out))

elif which == 2:
    n = r.randint(3, 10)
    m = r.randint(0, min(25, n * (n - 1)))
    out = [f"{n} {m}"]
    edges = set()
    while len(edges) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        edges.add((a, b))
    for a, b in sorted(edges):
        init = r.randint(0, 20)
        rec = r.randint(0, 8)
        out.append(f"{a} {b} {init} {rec / 10:.1f}")
    out.append(str(r.randint(1, n)))
    print('\n'.join(out))

elif which == 3:
    n = r.randint(3, 10)
    m = r.randint(0, min(25, n * (n - 1)))
    out = [f"{n} {m}"]
    edges = set()
    while len(edges) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        edges.add((a, b))
    for a, b in sorted(edges):
        out.append(f"{a} {b}")
    out.append(' '.join(str(r.randint(0, 20)) for _ in range(n)))
    s = r.randint(1, n)
    # 保证 s 可达至少一个酒吧：随机挑酒吧，若 s 到不了则补一条链
    p = r.randint(1, min(3, n))
    bars = r.sample([x for x in range(1, n + 1) if x != s], min(p, n - 1))
    out.append(f"{s} {len(bars)}")
    out.append(' '.join(str(x) for x in bars))
    print('\n'.join(out))

elif which == 4:
    n = r.randint(3, 10)
    m = r.randint(0, min(25, n * (n - 1)))
    out = [f"{n} {m}"]
    out.append(' '.join(str(r.randint(0, 20)) for _ in range(n)))
    edges = set()
    while len(edges) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        edges.add((a, b))
    for a, b in sorted(edges):
        op = r.randint(1, 2)
        out.append(f"{a} {b} {op}")
    print('\n'.join(out))

elif which == 5:
    n = r.randint(3, 10)
    m = r.randint(0, min(30, n * n * 2))
    x = r.randint(2, 100)
    out = [f"{n} {m} {x}"]
    edges = []
    for _ in range(m):
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        edges.append((a, b))
    for a, b in edges:
        out.append(f"{a} {b}")
    print('\n'.join(out))

elif which == 6:
    n = r.randint(3, 10)
    m = r.randint(0, min(25, n * (n - 1)))
    out = [f"{n} {m}"]
    edges = set()
    while len(edges) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        edges.add((a, b))
    for a, b in sorted(edges):
        out.append(f"{a} {b} {r.randint(0, 20)}")
    print('\n'.join(out))

elif which == 7:
    n = r.randint(3, 8)
    m = r.randint(0, 15)
    out = [f"{n} {m}"]
    out.append(' '.join(str(r.randint(1, 10)) for _ in range(n)))
    out.append(' '.join(str(r.randint(0, 20)) for _ in range(n)))
    deps = []
    for i in range(1, n + 1):
        d = r.randint(0, n)  # 0=无依赖；i 自身=自环；可能成环
        deps.append(d)
    out.append(' '.join(str(d) for d in deps))
    print('\n'.join(out))

elif which == 8:
    n = r.randint(3, 10)
    k = r.randint(2, 15)
    out = [f"{n} {k}"]
    for _ in range(k):
        op = r.randint(1, 5)
        u = r.randint(1, n); v = r.randint(1, n)
        out.append(f"{op} {u} {v}")
    print('\n'.join(out))

elif which == 9:
    n = r.randint(3, 10)
    m = r.randint(0, min(25, n * (n - 1)))
    out = [f"{n} {m}"]
    edges = set()
    while len(edges) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        edges.add((a, b))
    for a, b in sorted(edges):
        out.append(f"{a} {b}")
    print('\n'.join(out))
