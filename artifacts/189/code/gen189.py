import sys, random

seed = int(sys.argv[1])
which = int(sys.argv[2])
r = random.Random(seed + which * 1000003)

def gen_directed(n, maxm):
    cap = n * (n - 1) // 2
    m = r.randint(0, min(maxm, cap))
    es = set()
    while len(es) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        es.add((a, b))
    return list(es)

if which == 1 or which == 2:
    n = r.randint(2, 12)
    es = gen_directed(n, 40)
    out = [f"{n} {len(es)}"]
    for a, b in es: out.append(f"{a} {b}")
    print('\n'.join(out))

elif which == 3:
    n = r.randint(2, 10)
    cap = n * (n - 1) // 2
    m = r.randint(0, min(30, cap))
    es = set()
    lines = []
    while len(es) < m:
        a = r.randint(1, n); b = r.randint(1, n)
        if a == b: continue
        key = (min(a,b), max(a,b))
        if key in es: continue
        es.add(key)
        t = r.choice([1, 2])
        lines.append(f"{a} {b} {t}")
    out = [f"{n} {len(lines)}"] + lines
    print('\n'.join(out))

elif which == 4 or which == 5:
    n = r.randint(2, 12)
    es = gen_directed(n, 40)
    out = [f"{n} {len(es)}"]
    for a, b in es: out.append(f"{a} {b}")
    print('\n'.join(out))

elif which == 6:
    n = r.randint(2, 10)
    lines = [str(n)]
    for i in range(1, n + 1):
        ks = [x for x in range(1, n + 1) if x != i]
        r.shuffle(ks)
        take = r.randint(0, len(ks))
        row = [str(x) for x in ks[:take]] + ["0"]
        lines.append(' '.join(row))
    print('\n'.join(lines))

elif which == 7:
    n = r.randint(2, 10)
    p = r.randint(1, n)
    buy = r.sample(range(1, n + 1), p)
    cost = {u: r.randint(0, 20) for u in buy}
    es = gen_directed(n, 30)
    out = [f"{n} {p}"]
    for u in buy: out.append(f"{u} {cost[u]}")
    out.append(str(len(es)))
    for a, b in es: out.append(f"{a} {b}")
    print('\n'.join(out))

elif which == 8:
    n = r.randint(2, 12)
    es = gen_directed(n, 40)
    vals = [str(r.randint(1, 50)) for _ in range(n)]
    out = [str(n), ' '.join(vals), str(len(es))]
    for a, b in es: out.append(f"{a} {b}")
    print('\n'.join(out))

elif which == 9:
    t = r.randint(1, 3)
    out = [str(t)]
    for _ in range(t):
        n = r.randint(2, 10)
        es = gen_directed(n, 30)
        out.append(f"{n} {len(es)}")
        for a, b in es: out.append(f"{a} {b}")
    print('\n'.join(out))

elif which == 10:
    n = r.randint(1, 6)
    # 夫妻：女名 + 男名配对
    girls = []
    boys = []
    letters = 'abcdefghijklmnopqrstuvwxyz'
    used_names = set()
    def new_name():
        while True:
            s = ''.join(r.choice(letters) for _ in range(r.randint(1, 2)))
            if s not in used_names:
                used_names.add(s)
                return s
    pairs = []
    for i in range(n):
        g = new_name(); b = new_name()
        girls.append(g); boys.append(b)
        pairs.append((g, b))
    m = r.randint(0, n * n)
    aff = set()
    while len(aff) < m:
        g = r.choice(girls); b = r.choice(boys)
        if (g, b) in aff: continue
        aff.add((g, b))
    out = [str(n)]
    for a, b in pairs: out.append(f"{a} {b}")
    out.append(str(len(aff)))
    for g, b in aff: out.append(f"{g} {b}")
    print('\n'.join(out))
