#!/usr/bin/env python3
"""158 课 5 题对拍：课程 .bin vs 题意暴力。
强制在线题：生成真实参数 → 反推「给定参数」输入，课程与暴力同流解码。
"""
import random, subprocess, sys

random.seed(20261018)

def run_bin(binpath, inp):
    r = subprocess.run([binpath], input=inp.encode(), capture_output=True)
    return r.stdout.decode().strip()

# ---------- 题1 HDU5919 首次出现位置序列（强制在线） ----------
def gen1(n, m, maxv):
    arr = [random.randint(0, maxv) for _ in range(n)]
    real_qs = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    # 反推给定参数（解码 a=(gl+lastAns)%n+1）
    lines = ["1", f"{n} {m}", " ".join(map(str, arr))]
    given = []
    lastAns = 0
    for (rl, rr) in real_qs:
        gl = ((rl - 1 - lastAns) % n) + 1
        gr = ((rr - 1 - lastAns) % n) + 1
        given.append((gl, gr))
        # 预演解码
        a = (gl + lastAns) % n + 1
        b = (gr + lastAns) % n + 1
        l, r = min(a, b), max(a, b)
        seen = set(); seq = []
        for i in range(l - 1, r):
            if arr[i] not in seen:
                seen.add(arr[i]); seq.append(i + 1)
        lastAns = seq[(len(seq) + 1) // 2 - 1]
    for (gl, gr) in given:
        lines.append(f"{gl} {gr}")
    inp = "\n".join(lines) + "\n"
    # 暴力（重跑同流）
    out = []
    lastAns = 0
    for (gl, gr) in given:
        a = (gl + lastAns) % n + 1
        b = (gr + lastAns) % n + 1
        l, r = min(a, b), max(a, b)
        seen = set(); seq = []
        for i in range(l - 1, r):
            if arr[i] not in seen:
                seen.add(arr[i]); seq.append(i + 1)
        lastAns = seq[(len(seq) + 1) // 2 - 1]
        out.append(str(lastAns))
    return inp, "Case #1:" + "".join(" " + x for x in out)

# ---------- 题2 P4137 最小自然数 ----------
def gen2(n, m, maxv):
    arr = [random.randint(0, maxv) for _ in range(n)]
    qs = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    lines = [f"{n} {m}", " ".join(map(str, arr))]
    for (l, r) in qs:
        lines.append(f"{l} {r}")
    inp = "\n".join(lines) + "\n"
    out = []
    for (l, r) in qs:
        s = set(arr[l - 1:r])
        v = 0
        while v in s:
            v += 1
        out.append(str(v))
    return inp, "\n".join(out)

# ---------- 题3 P2839 浮动区间最大上中位数（强制在线） ----------
def gen3(n, m, maxv):
    # 唯一值：避免重复值 + 不稳定排序歧义，对拍聚焦算法语义
    arr = random.sample(range(1, maxv + 1), n)
    real_qs = []
    for _ in range(m):
        pts = sorted(random.sample(range(1, n + 1), 4))
        real_qs.append(tuple(pts))
    lines = [str(n), " ".join(map(str, arr)), str(m)]
    given = []
    lastAns = 0
    for q in real_qs:
        g = tuple(((x - 1 - lastAns) % n) + 1 for x in q)
        given.append(g)
        vals = sorted((x + lastAns) % n + 1 for x in g)
        lastAns = brute3_compute(arr, vals)
    for g in given:
        lines.append(" ".join(map(str, g)))
    inp = "\n".join(lines) + "\n"
    out = []
    lastAns = 0
    for g in given:
        vals = sorted((x + lastAns) % n + 1 for x in g)
        best = brute3_compute(arr, vals)
        lastAns = best
        out.append(str(best))
    return inp, "\n".join(out)


def brute3_check(arr, a, b, c, d, v):
    """课程 check 语义：root[v] 中 值>=v 标 +1，其余 -1；
    best = max_{L in [a,b]} sum([L,b]) + sum([b+1,c-1]) + max_{R in [c,d]} sum([c,R])；
    return best >= 0。"""
    sign = [1 if x >= v else -1 for x in arr]  # 0-indexed
    NEG = -10**9
    run = 0; suf = NEG
    for i in range(b - 1, a - 2, -1):
        run += sign[i]
        if run > suf: suf = run
    mid_sum = sum(sign[b:c - 1])  # 1-idx [b+1, c-1]
    run = 0; pre = NEG
    for i in range(c - 1, d):
        run += sign[i]
        if run > pre: pre = run
    return suf + mid_sum + pre >= 0


def brute3_compute(arr, vals):
    """二分最大 v：check(v) true 的最大值。vals 是解码后的 a,b,c,d。"""
    a, b, c, d = vals
    uniq = sorted(set(arr))
    lo, hi = 0, len(uniq) - 1
    ans = uniq[0]
    while lo <= hi:
        mid = (lo + hi) // 2
        if brute3_check(arr, a, b, c, d, uniq[mid]):
            ans = uniq[mid]; lo = mid + 1
        else:
            hi = mid - 1
    return ans

# ---------- 树生成 ----------
def gen_tree(n):
    edges = []
    for i in range(2, n + 1):
        p = random.randint(1, i - 1)
        edges.append((p, i))
    return edges

# ---------- 题4 P2633 路径第 k 小（强制在线 u^lastAns） ----------
def gen4(n, m, maxv):
    arr = [random.randint(1, maxv) for _ in range(n)]
    edges = gen_tree(n)
    # 邻接表
    adj = [[] for _ in range(n + 1)]
    for (u, v) in edges:
        adj[u].append(v); adj[v].append(u)
    # 暴力路径收集（每次现场 BFS 找路径）
    lines = [f"{n} {m}", " ".join(map(str, arr))]
    for (u, v) in edges:
        lines.append(f"{u} {v}")
    real_qs = []
    for _ in range(m):
        u = random.randint(1, n); v = random.randint(1, n)
        # 先算路径长度，k 限定在 [1, len]（原题 k 不超路径点数）
        par0 = {u: 0}
        q0 = [u]
        while q0:
            x = q0.pop(0)
            if x == v:
                break
            for y in adj[x]:
                if y not in par0:
                    par0[y] = x; q0.append(y)
        plen = 0
        x = v
        while True:
            plen += 1
            if x == u:
                break
            x = par0[x]
        k = random.randint(1, plen)
        real_qs.append((u, v, k))
    given = []
    lastAns = 0
    for (ru, rv, rk) in real_qs:
        given.append((ru ^ lastAns, rv, rk))
        # 预演找路径
        par = {ru: 0}
        q = [ru]
        while q:
            x = q.pop(0)
            if x == rv:
                break
            for y in adj[x]:
                if y not in par:
                    par[y] = x; q.append(y)
        path = []
        x = rv
        while True:
            path.append(arr[x - 1])
            if x == ru:
                break
            x = par[x]
        path.sort()
        lastAns = path[rk - 1]
    for (gu, gv, gk) in given:
        lines.append(f"{gu} {gv} {gk}")
    inp = "\n".join(lines) + "\n"
    out = []
    lastAns = 0
    for (gu, gv, gk) in given:
        ru = gu ^ lastAns; rv = gv; rk = gk
        par = {ru: 0}
        q = [ru]
        while q:
            x = q.pop()
            if x == rv:
                break
            for y in adj[x]:
                if y not in par:
                    par[y] = x; q.append(y)
        path = []
        x = rv
        while True:
            path.append(arr[x - 1])
            if x == ru:
                break
            x = par[x]
        path.sort()
        lastAns = path[rk - 1]
        out.append(str(lastAns))
    return inp, "\n".join(out)

# ---------- 题5 P3899 更为厉害 ----------
def gen5(n, m, maxk):
    edges = gen_tree(n)
    adj = [[] for _ in range(n + 1)]
    for (u, v) in edges:
        adj[u].append(v); adj[v].append(u)
    # 预处理：父、深度、dfn、siz、子树区间
    par = [0] * (n + 1); dep = [0] * (n + 1)
    dfn = [0] * (n + 1); siz = [0] * (n + 1)
    order = []
    st = [1]; par[1] = 0; dep[1] = 1
    # 迭代 DFS 求 dfn/siz（后序）
    visited = [False] * (n + 1)
    stack = [(1, 0, 0)]  # (node, parent, state)
    cntd = 0
    while stack:
        u, f, stt = stack.pop()
        if stt == 0:
            visited[u] = True
            dfn[u] = cntd + 1; cntd += 1
            stack.append((u, f, 1))
            for y in adj[u]:
                if y != f:
                    dep[y] = dep[u] + 1; par[y] = u
                    stack.append((y, u, 0))
        else:
            siz[u] = 1
            for y in adj[u]:
                if y != f:
                    siz[u] += siz[y]
    tin = [0] * (n + 1); tout = [0] * (n + 1)
    for i in range(1, n + 1):
        tin[i] = dfn[i]; tout[i] = dfn[i] + siz[i] - 1

    def isAnc(x, y):  # x 是 y 的祖先？
        return tin[x] <= tin[y] <= tout[x]

    lines = [f"{n} {m}"]
    for (u, v) in edges:
        lines.append(f"{u} {v}")
    qs = [(random.randint(1, n), random.randint(1, maxk)) for _ in range(m)]
    for (a, k) in qs:
        lines.append(f"{a} {k}")
    inp = "\n".join(lines) + "\n"
    out = []
    for (a, k) in qs:
        cnt = 0
        for b in range(1, n + 1):
            if b == a:
                continue
            # a、b 邻居：路径边数 <= k（树上距离）
            # 算距离：LCA 近似——小数据直接 BFS
            dist_ab = None
            distq = [(a, 0)]
            dist_seen = {a: 0}
            while distq:
                x, d = distq.pop(0)
                if x == b:
                    dist_ab = d; break
                for y in adj[x]:
                    if y not in dist_seen:
                        dist_seen[y] = d + 1; distq.append((y, d + 1))
            if dist_ab is None or dist_ab > k:
                continue
            for c in range(1, n + 1):
                if c == a or c == b:
                    continue
                if isAnc(a, c) and isAnc(b, c):
                    cnt += 1
        out.append(str(cnt))
    return inp, "\n".join(out)

# ---------- 主循环 ----------
def paipai(name, genfn, binpath, rounds):
    fails = 0
    for r in range(rounds):
        inp, expect = genfn()
        got = run_bin(binpath, inp)
        if got != expect:
            fails += 1
            print(f"  MISMATCH round {r}")
            print("  IN :", inp.replace("\n", " | ")[:300])
            print("  EXP:", expect[:200])
            print("  GOT:", got[:200])
            if fails >= 3:
                break
    print(f"{name}: {rounds} 轮, {fails} fails")
    return fails

total = 0
total += paipai("题1 HDU5919", lambda: gen1(10, 25, 12), "/tmp/cpp158/Code01_FirstTimeSequence.bin", 300)
total += paipai("题2 P4137 ", lambda: gen2(15, 25, 18), "/tmp/cpp158/Code02_MissingSmallest.bin", 300)
total += paipai("题3 P2839 ", lambda: gen3(12, 12, 40), "/tmp/cpp158/Code03_LargestUpMedian.bin", 200)
total += paipai("题4 P2633 ", lambda: gen4(12, 20, 25), "/tmp/cpp158/Code04_CountOnTree.bin", 300)
total += paipai("题5 P3899 ", lambda: gen5(14, 8, 5), "/tmp/cpp158/Code05_MoreImpressive.bin", 150)
print("=== 总 fails:", total, "===")
sys.exit(1 if total else 0)
