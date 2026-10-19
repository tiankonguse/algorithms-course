#!/usr/bin/env python3
import random, sys

seed0 = int(sys.argv[1]) if len(sys.argv) > 1 else 0

def mulberry32(seed):
    def rnd():
        nonlocal seed
        seed = (seed + 0x6D2B79F5) & 0xFFFFFFFF
        t = seed
        t = (t ^ (t >> 15)) * (t | 1) & 0xFFFFFFFF
        t = t ^ (t + ((t ^ (t >> 7)) * (t | 61) & 0xFFFFFFFF))
        return ((t ^ (t >> 14)) & 0xFFFFFFFF) / 4294967296
    return rnd

rng = mulberry32(seed0 * 100000 + 7)
def ri(a, b): return a + int(rng() * (b - a + 1))
def pick(seq): return seq[int(rng() * len(seq))]

# ---------- 题1 领导集团问题 ----------
def gen1():
    n = ri(2, 12)
    arr = [0] + [ri(1, 20) for _ in range(n)]
    fa = [0] * (n + 1)
    for i in range(2, n + 1):
        fa[i] = ri(1, i - 1)
    out = [str(n), ' '.join(map(str, arr[1:])), ' '.join(map(str, fa[2:]))]
    return '\n'.join(out) + '\n'

# ---------- 题2 根节点的取值 ----------
def gen2():
    n = ri(3, 12)
    fa = [0] * (n + 1)
    for i in range(2, n + 1):
        fa[i] = i // 2  # 二叉形态
    # 打乱父亲保持二叉但形态随机? 保持 i/2 即可（完全二叉）
    arr = [0] * (n + 1)
    soncnt = [0] * (n + 1)
    for i in range(2, n + 1):
        soncnt[fa[i]] += 1
    leaves = [i for i in range(1, n + 1) if soncnt[i] == 0]
    vals = list(range(1, 10 * n + 1))
    random.shuffle(vals)
    r = rng
    # 用独立洗牌
    import random as _r
    _r.seed(seed0 * 7 + 3)
    vals = list(range(1, 10 * n + 1)); _r.shuffle(vals)
    for idx, leaf in enumerate(leaves):
        arr[leaf] = vals[idx]
    for i in range(1, n + 1):
        if soncnt[i] > 0:
            arr[i] = ri(1, 9999)
    # 输出: n, fa[1..n], arr[1..n]
    out = [str(n), ' '.join(map(str, fa[1:])), ' '.join(map(str, arr[1:]))]
    return '\n'.join(out) + '\n'

# ---------- 题3 命运 ----------
def gen3():
    n = ri(3, 8)
    edges = []
    for i in range(2, n + 1):
        edges.append((i, ri(1, i - 1)))
    m = ri(1, 4)
    pairs = []
    used = set()
    for _ in range(m):
        y = ri(2, n)
        # x 为 y 的祖先：沿 fa 链随机取
        anc = []
        cur = y
        while cur != 1:
            cur = edges[cur - 2][1]
            anc.append(cur)
        x = pick(anc) if anc else 1
        if (x, y) in used:
            continue
        used.add((x, y))
        pairs.append((x, y))
    if not pairs:
        pairs.append((1, 2))
    out = [str(n)]
    for u, v in edges:
        out.append(f'{u} {v}')
    out.append(str(len(pairs)))
    for x, y in pairs:
        out.append(f'{x} {y}')
    return '\n'.join(out) + '\n'

# ---------- 题4 线段树分裂 ----------
def gen4():
    n = ri(3, 8)
    m = ri(10, 20)
    init = [ri(0, 3) for _ in range(n)]
    out = [f'{n} {m}', ' '.join(map(str, init))]
    nxt = 2  # 下一个新集合编号
    alive = [1]  # 有效集合编号
    sizes = {1: sum(init)}  # 集合大小（操作4 合法范围依据）
    for _ in range(m):
        op = ri(0, 4)
        if op == 0:
            x = pick(alive)
            y = ri(1, n); z = ri(y, n)
            out.append(f'0 {x} {y} {z}')
            nxt += 1
            alive.append(nxt - 1)
            # 取出 [y,z]：数量取决于 x 集合中落在该区间的个数（粗略按 z-y+1 上限随机）
            cnt = ri(0, min(sizes.get(x, 0), z - y + 1))
            sizes[x] = sizes.get(x, 0) - cnt
            sizes[nxt - 1] = cnt
        elif op == 1:
            if len(alive) < 2:
                out.append(f'2 1 {ri(0, 2)} {ri(1, n)}')
                continue
            x = pick(alive)
            y = pick([a for a in alive if a != x])
            out.append(f'1 {x} {y}')
            sizes[x] = sizes.get(x, 0) + sizes.get(y, 0)
            alive.remove(y)
        elif op == 2:
            x = pick(alive)
            y = ri(0, 3); z = ri(1, n)
            out.append(f'2 {x} {y} {z}')
            sizes[x] = sizes.get(x, 0) + y
        elif op == 3:
            x = pick(alive)
            y = ri(1, n); z = ri(y, n)
            out.append(f'3 {x} {y} {z}')
        else:
            x = pick(alive)
            # 原题数据保证 k <= 集合大小（空集合则输出 -1）
            y = ri(1, max(1, sizes.get(x, 0)))
            out.append(f'4 {x} {y}')
    return '\n'.join(out) + '\n'
    return '\n'.join(out) + '\n'

# ---------- 题5 值全改 ----------
def gen5():
    n = ri(3, 8)
    q = ri(3, 10)
    arr = [ri(1, 5) for _ in range(n)]
    out = [str(n), ' '.join(map(str, arr)), str(q)]
    for _ in range(q):
        l = ri(1, n); r = ri(l, n)
        x = ri(1, 5); y = ri(1, 5)
        while y == x:
            y = ri(1, 5)
        out.append(f'{l} {r} {x} {y}')
    return '\n'.join(out) + '\n'

which = sys.argv[2] if len(sys.argv) > 2 else '1'
gen = {'1': gen1, '2': gen2, '3': gen3, '4': gen4, '5': gen5}[which]
sys.stdout.write(gen())
