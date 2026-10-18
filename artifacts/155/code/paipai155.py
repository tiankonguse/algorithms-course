#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""155 课 5 题对拍：课程 C++ vs 暴力。
题1 城市攻占 P3261 / 题2 棘手操作 P3273 / 题4 Y的积木 P2409 / 题5 k短路 P2483
题3 可持久化左偏树自带对数器，单独跑。
"""
import random, subprocess, sys, os, itertools, math

BIN = '/tmp/cpp155'

def run(binname, inp):
    p = subprocess.run([os.path.join(BIN, binname)], input=inp.encode(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if p.returncode != 0:
        raise RuntimeError(binname + ' 崩溃: ' + p.stderr.decode()[:200] + '\n输入:\n' + inp[:500])
    return p.stdout.decode().strip().split('\n')

# ---------------- 题1 城市攻占 P3261 ----------------
def gen_city(rng):
    n = rng.randint(2, 10)
    m = rng.randint(1, 12)
    defend = [rng.randint(1, 10) for _ in range(n + 1)]  # 1-based
    belong = [0] * (n + 1)
    typ = [0] * (n + 1)
    gain = [0] * (n + 1)
    for i in range(2, n + 1):
        belong[i] = rng.randint(1, i - 1)
        typ[i] = rng.randint(0, 1)
        gain[i] = rng.randint(1, 3)
    atk = [0] * (m + 1)
    first = [0] * (m + 1)
    for i in range(1, m + 1):
        atk[i] = rng.randint(1, 15)
        first[i] = rng.randint(1, n)
    lines = ['%d %d' % (n, m)]
    lines.append(' '.join(str(defend[i]) for i in range(1, n + 1)))
    for i in range(2, n + 1):
        lines.append('%d %d %d' % (belong[i], typ[i], gain[i]))
    for i in range(1, m + 1):
        lines.append('%d %d' % (atk[i], first[i]))
    return '\n'.join(lines) + '\n'

def brute_city(inp):
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    defend = list(map(int, lines[1].split()))
    defend = [0] + defend
    belong = [0] * (n + 1); typ = [0] * (n + 1); gain = [0] * (n + 1)
    for i in range(2, n + 1):
        b, t, g = map(int, lines[i].split())
        belong[i] = b; typ[i] = t; gain[i] = g
    atk = [0] * (m + 1); first = [0] * (m + 1)
    for i in range(1, m + 1):
        a, f = map(int, lines[n + i].split())
        atk[i] = a; first[i] = f
    sac = [0] * (n + 1)
    conqu = [0] * (m + 1)
    for i in range(1, m + 1):
        cur = first[i]
        a = atk[i]
        cnt = 0
        while cur != 0:
            if a >= defend[cur]:
                cnt += 1
                if cur != 1:
                    if typ[cur] == 0:
                        a += gain[cur]
                    else:
                        a *= gain[cur]
                cur = belong[cur]
            else:
                sac[cur] += 1
                break
        conqu[i] = cnt
    out = [str(sac[i]) for i in range(1, n + 1)] + [str(conqu[i]) for i in range(1, m + 1)]
    return out

# ---------------- 题2 棘手操作 P3273 ----------------
def gen_tricky(rng):
    n = rng.randint(2, 10)
    num = [rng.randint(1, 20) for _ in range(n + 1)]
    ops = []
    m = rng.randint(10, 60)
    for _ in range(m):
        op = rng.choice(['U', 'A1', 'A2', 'A3', 'F1', 'F2', 'F3'])
        if op == 'U':
            ops.append('U %d %d' % (rng.randint(1, n), rng.randint(1, n)))
        elif op == 'A1':
            ops.append('A1 %d %d' % (rng.randint(1, n), rng.randint(-5, 5)))
        elif op == 'A2':
            ops.append('A2 %d %d' % (rng.randint(1, n), rng.randint(-5, 5)))
        elif op == 'A3':
            ops.append('A3 %d' % rng.randint(-5, 5))
        elif op == 'F1':
            ops.append('F1 %d' % rng.randint(1, n))
        elif op == 'F2':
            ops.append('F2 %d' % rng.randint(1, n))
        else:
            ops.append('F3')
    lines = [str(n), ' '.join(str(num[i]) for i in range(1, n + 1)), str(m)]
    lines.extend(ops)
    return '\n'.join(lines) + '\n'

def brute_tricky(inp):
    lines = inp.strip().split('\n')
    n = int(lines[0])
    w = [0] + list(map(int, lines[1].split()))
    m = int(lines[2])
    fa = list(range(n + 1))
    members = {i: {i} for i in range(1, n + 1)}
    globalAdd = 0
    def find(x):
        while fa[x] != x:
            fa[x] = fa[fa[x]]
            x = fa[x]
        return x
    def union(x, y):
        a, b = find(x), find(y)
        if a == b: return
        if len(members[a]) < len(members[b]):
            a, b = b, a
        fa[b] = a
        members[a] |= members[b]
        del members[b]
    out = []
    for line in lines[3:]:
        parts = line.split()
        op = parts[0]
        if op == 'U':
            union(int(parts[1]), int(parts[2]))
        elif op == 'A1':
            x, v = int(parts[1]), int(parts[2])
            w[x] += v
        elif op == 'A2':
            x, v = int(parts[1]), int(parts[2])
            h = find(x)
            for i in members[h]:
                w[i] += v
        elif op == 'A3':
            globalAdd += int(parts[1])
        elif op == 'F1':
            out.append(str(w[int(parts[1])] + globalAdd))
        elif op == 'F2':
            h = find(int(parts[1]))
            out.append(str(max(w[i] for i in members[h]) + globalAdd))
        else:
            out.append(str(max(w[i] for i in range(1, n + 1)) + globalAdd))
    return out

# ---------------- 题4 Y的积木 P2409 ----------------
def gen_blocks(rng):
    n = rng.randint(1, 4)
    arrs = []
    for _ in range(n):
        mi = rng.randint(1, 4)
        arrs.append([rng.randint(1, 100) for _ in range(mi)])
    total = 1
    for a in arrs: total *= len(a)
    k = rng.randint(1, min(30, total))
    lines = ['%d %d' % (n, k)]
    for a in arrs:
        lines.append('%d %s' % (len(a), ' '.join(map(str, a))))
    return '\n'.join(lines) + '\n'

def brute_blocks(inp):
    lines = inp.strip().split('\n')
    n, k = map(int, lines[0].split())
    arrs = []
    for i in range(n):
        parts = list(map(int, lines[1 + i].split()))
        arrs.append(parts[1:])
    sums = sorted(sum(comb) for comb in itertools.product(*arrs))
    return [str(sums[i]) for i in range(min(k, len(sums)))]

# ---------------- 题5 k短路 P2483 ----------------
def gen_ksp(rng):
    n = rng.randint(2, 5)
    m = rng.randint(1, 6)
    edges = []
    seen = set()
    for _ in range(m):
        u = rng.randint(1, n)
        v = rng.randint(1, n)
        if u == v or (u, v) in seen:
            continue
        seen.add((u, v))
        edges.append((u, v, rng.randint(1, 4)))
    if len(edges) < 1:
        edges.append((1, n, 1))
    money = rng.randint(1, 14)
    lines = ['%d %d %d' % (n, len(edges), money)]
    for u, v, w in edges:
        lines.append('%d %d %d' % (u, v, w))
    return '\n'.join(lines) + '\n'

def brute_ksp(inp):
    lines = inp.strip().split('\n')
    n, m, money = map(int, lines[0].split())
    edges = []
    for i in range(m):
        u, v, w = map(int, lines[1 + i].split())
        if u != n:  # 到达 n 即停，n 的出边不用
            edges.append((u, v, w))
    # 课程语义：路径允许边重复（树边段可重复），费用正、按 money 剪枝。
    # 枚举所有 1→n 路径（费用 ≤ money），排序后逐个扣钱。
    costs = []
    def dfs(u, cost):
        for a, b, w in edges:
            if a != u:
                continue
            ncost = cost + w
            if ncost > money:
                continue
            if b == n:
                costs.append(ncost)
            else:
                dfs(b, ncost)
    dfs(1, 0)
    costs.sort()
    ans = 0
    rem = money
    for c in costs:
        if rem >= c:
            rem -= c
            ans += 1
        else:
            break
    return [str(ans)]

# ---------------- 主循环 ----------------
def paipai(name, gen, brute, binname, rounds):
    ok = 0
    for r in range(rounds):
        rng = random.Random(r * 7919 + len(name))
        inp = gen(rng)
        want = ' '.join(brute(inp))
        got = ' '.join(run(binname, inp))
        if want != got:
            print('MISMATCH round', r)
            print('输入:\n' + inp)
            print('暴力:', want)
            print('课程:', got)
            return False
        ok += 1
    print('%-12s %d 轮 OK' % (name, ok))
    return True

if __name__ == '__main__':
    random.seed(20261018)
    allok = True
    allok &= paipai('P3261', gen_city, brute_city, 'Code01_CityCapture2.bin', 300)
    allok &= paipai('P3273', gen_tricky, brute_tricky, 'Code02_TrickyOperation2.bin', 300)
    allok &= paipai('P2409', gen_blocks, brute_blocks, 'Code04_Blocks2.bin', 200)
    allok &= paipai('P2483', gen_ksp, brute_ksp, 'Code05_KShortestPath2.bin', 200)
    print('=== 全部', '通过' if allok else '有 MISMATCH', '===')
    sys.exit(0 if allok else 1)
