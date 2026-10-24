import random, sys
def gen_block(start, size, want_ring=None):
    k = random.randint(3, min(size, 6)) if want_ring is None else want_ring
    ring = random.sample(range(start, start + size), k)
    edges = []
    for i in range(k):
        edges.append((ring[i], ring[(i + 1) % k]))
    used = set(ring)
    rest = [x for x in range(start, start + size) if x not in used]
    for x in rest:
        p = random.choice(list(used))
        edges.append((x, p))
        used.add(x)
    return edges

def gen(n, comps, seed):
    random.seed(seed)
    if comps > 1:
        sizes = [3] * comps
        rem = n - 3 * comps
        while rem > 0:
            i = random.randrange(comps)
            sizes[i] += 1
            rem -= 1
    else:
        sizes = [n]
    edges = []
    start = 1
    for s in sizes:
        edges += gen_block(start, s)
        start += s
    return edges, sizes

def gen_func(n, comps, seed):
    # 每个点恰好一个出边（函数图），底层无向图是基环树；返回 out[1..n]
    r = random.Random(seed * 7919)
    sizes = [3] * comps
    rem = n - 3 * comps
    while rem > 0:
        i = r.randrange(comps)
        sizes[i] += 1
        rem -= 1
    out = [0] * (n + 1)
    start = 1
    for size in sizes:
        k = r.randint(3, min(size, 6))
        ring = r.sample(range(start, start + size), k)
        for i in range(k):
            out[ring[i]] = ring[(i + 1) % k]
        used = set(ring)
        for x in range(start, start + size):
            if x not in used:
                p = r.choice(list(used))
                out[x] = p
                used.add(x)
        start += size
    return out

def gen_t1(n, seed):
    comps = random.Random(seed).randint(1, min(3, n // 3))
    out = gen_func(n, comps, seed)
    lines = ["1", str(n)]
    for i in range(1, n + 1):
        lines.append(str(out[i]))
    return "\n".join(lines)

def gen_t2(t, nmax, seed):
    r = random.Random(seed)
    out = ["2", str(t)]
    for _ in range(t):
        n = r.randint(3, nmax)
        edges, _ = gen(n, 1, r.randrange(10**9))
        out.append(str(n))
        for (a, b) in edges:
            out.append(f"{a} {b}")
    return "\n".join(out)

def gen_t3(n, seed):
    r = random.Random(seed)
    comps = r.randint(1, min(3, n // 3))
    out = gen_func(n, comps, seed)
    lines = ["1", str(n)]
    for i in range(1, n + 1):
        lines.append(f"{r.randint(1, 20)} {out[i]}")
    return "\n".join(lines)

def gen_t4(n, seed):
    r = random.Random(seed)
    edges, _ = gen(n, 1, seed)
    lines = ["4", str(n)]
    lines.append(" ".join(str(r.randint(1, 20)) for _ in range(n)))
    for (a, b) in edges:
        lines.append(f"{a - 1} {b - 1}")
    lines.append(str(r.choice([0.5, 1.0, 1.5, 2.0, 2.5])))
    return "\n".join(lines)



def gen_t5(n, seed):
    r = random.Random(seed * 1009)
    edges, _ = gen(n, 1, seed * 1009)
    lines = ["5", str(n)]
    for (a, b) in edges:
        lines.append(f"{a} {b} {r.randint(1, 20)}")
    return "\n".join(lines)

def gen_t6(n, seed):
    r = random.Random(seed * 1013)
    comps = r.randint(1, min(2, n // 3))
    out = gen_func(n, comps, seed * 1013)
    lines = ["6", str(n)]
    for u in range(1, n + 1):
        lines.append(f"{out[u]} {r.randint(1, 20)}")
    return "\n".join(lines)

def gen_t7(n, seed):
    r = random.Random(seed * 1019)
    edges, _ = gen(n, 1, seed * 1019)
    lines = ["7", f"{n} {n * 2}"]
    for (a, b) in edges:
        lines.append(f"{a} {b} {r.randint(0, 8)}")
    for _ in range(n * 2):
        if r.random() < 0.5:
            x = r.randint(1, n)
            lines.append(f"1 {r.randint(1, n)} {r.randint(0, 8)}")
        else:
            x, y = r.sample(range(1, n + 1), 2)
            lines.append(f"2 {x} {y}")
    return "\n".join(lines)

def gen_t8(n, seed):
    r = random.Random(seed * 1021)
    edges, _ = gen(n, 1, seed * 1021)  # 基环树 n 条边，前 k 条是环边
    if r.random() < 0.5:
        m = n - 1
        del edges[0]  # 删一条环边 → 普通树
    else:
        m = n
    r.shuffle(edges)
    lines = ["8", f"{n} {m}"]
    for (a, b) in edges:
        lines.append(f"{a} {b}")
    return "\n".join(lines)

if __name__ == "__main__":
    what = sys.argv[1]; seed = int(sys.argv[2]); n = int(sys.argv[3]) if len(sys.argv) > 3 else 8
    if what == "t1": print(gen_t1(n, seed))
    elif what == "t2": print(gen_t2(3, n, seed))
    elif what == "t3": print(gen_t3(n, seed))
    elif what == "t4": print(gen_t4(n, seed))
    elif what == "t5": print(gen_t5(n, seed))
    elif what == "t6": print(gen_t6(n, seed))
    elif what == "t7": print(gen_t7(n, seed))
    elif what == "t8": print(gen_t8(n, seed))

