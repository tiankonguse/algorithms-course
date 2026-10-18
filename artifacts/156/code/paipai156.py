#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 156 课 9 题对拍器：生成随机小数据 -> 跑课程 C++ bin -> python 独立暴力比对
# 用法: python3 paipai156.py [轮数]
import random, subprocess, sys, os, math

BIN = os.path.dirname(os.path.abspath(__file__))
random.seed(20261018)

def run(bin_name, data):
    p = subprocess.run([os.path.join(BIN, bin_name)], input=data, capture_output=True, text=True, timeout=10)
    if p.returncode != 0:
        raise RuntimeError('%s 崩溃: %s' % (bin_name, p.stderr[:300]))
    return p.stdout

# ============ 图模型暴力（1/2/3/5/6/9 共用思路，各自实现） ============

def brute_interval(n, ops, queries=None):
    """区间和/图暴力：节点 1..n+1（S 下标），边 (u,v,w): S[v]-S[u]=w。
    返回 (答案列表或冲突判定)。ops 只含 (l,r,v) 给定，全部自洽时查询用 BFS。
    用于题1：返回查询答案列表。"""
    adj = {i: {} for i in range(1, n + 2)}
    for (l, r, v) in ops:
        adj[l][r + 1] = v
        adj[r + 1][l] = -v
    out = []
    for (l, r) in queries:
        # BFS 求 S[r] - S[l]
        seen = {l: 0}
        stack = [l]
        found = None
        while stack:
            u = stack.pop()
            if u == r + 1:
                found = seen[u]
                break
            for v, w in adj[u].items():
                if v not in seen:
                    seen[v] = seen[u] + w
                    stack.append(v)
        if found is None:
            out.append('UNKNOWN')
        else:
            out.append(str(found))
    return out

def brute_interval_check(n, ops):
    """题2/3 暴力：按顺序处理，返回 (自洽标志/错误数, ...)。
    图 + BFS：加边前先检查连通性与差值。"""
    adj = {i: {} for i in range(1, n + 2)}
    bad = 0
    ok = True
    for (l, r, v) in ops:
        # 检查 l 与 r 是否连通，连通则差值必须 = v
        seen = {l: 0}
        stack = [l]
        diff = None
        while stack:
            u = stack.pop()
            if u == r + 1:
                diff = seen[u]
                break
            for w, wv in adj[u].items():
                if w not in seen:
                    seen[w] = seen[u] + wv
                    stack.append(w)
        if diff is not None:
            if diff != v:
                bad += 1
                ok = False
        else:
            adj[l][r + 1] = v
            adj[r + 1][l] = -v
    return bad, ok

def brute_div(eqs, vals, qs):
    """题5 除法暴力：字符串变量图，BFS 比值。"""
    adj = {}
    for (a, b), v in zip(eqs, vals):
        adj.setdefault(a, {})[b] = v
        adj.setdefault(b, {})[a] = 1.0 / v
        adj.setdefault(a, {})
        adj.setdefault(b, {})
    out = []
    for (a, b) in qs:
        if a not in adj or b not in adj:
            out.append('-1.0')
            continue
        seen = {a: 1.0}
        stack = [a]
        found = None
        while stack:
            u = stack.pop()
            if u == b:
                found = seen[u]
                break
            for w, wv in adj[u].items():
                if w not in seen:
                    seen[w] = seen[u] * wv
                    stack.append(w)
        if found is None:
            out.append('-1.0')
        else:
            out.append('%.10f' % found)
    return out

def brute_food(n, ops):
    """题6 食物链暴力：%3 差值图 + BFS。
    ops = [(op, l, r)]，返回假话数。"""
    adj = {i: {} for i in range(1, n + 1)}
    ans = 0
    for (op, l, r) in ops:
        if l > n or r > n or (op == 2 and l == r):
            ans += 1
            continue
        # BFS 算 (dist[l]-dist[r])%3
        seen = {l: 0}
        stack = [l]
        diff = None
        while stack:
            u = stack.pop()
            if u == r:
                diff = seen[u]
                break
            for w, wv in adj[u].items():
                if w not in seen:
                    seen[w] = (seen[u] + wv) % 3
                    stack.append(w)
        if diff is not None:
            if op == 1:
                if diff % 3 != 0:
                    ans += 1
            else:
                if diff % 3 != 1:
                    ans += 1
        else:
            v = 0 if op == 1 else 1
            adj[l][r] = v
            adj[r][l] = (-v) % 3
    return ans

def brute_xor(n, stmts, queries):
    """题9 异或暴力：xor 差值图 + 自由变量枚举。
    stmts = [(l, r, v)] 声明（节点 0..n，n 为虚点），queries = [节点列表]。
    返回输出行列表（含冲突行）。"""
    adj = {i: {} for i in range(n + 1)}
    conflict = False
    cnti = 0
    out = []
    for (l, r, v) in stmts:
        cnti += 1
        if conflict:
            continue
        seen = {l: 0}
        stack = [l]
        diff = None
        while stack:
            u = stack.pop()
            if u == r:
                diff = seen[u]
                break
            for w, wv in adj[u].items():
                if w not in seen:
                    seen[w] = seen[u] ^ wv
                    stack.append(w)
        if diff is not None:
            if diff != v:
                conflict = True
                out.append('The first %d facts are conflicting.' % cnti)
        else:
            adj[l][r] = v
            adj[r][l] = v
    for qs in queries:
        if conflict:
            continue
        # 对每个节点 BFS 找出其连通块（用节点 0..n 全量 BFS 构建块表）
        comp_of = {}
        rep_of = {}
        seen_all = set()
        for i in range(n + 1):
            if i in seen_all:
                continue
            comp = []
            stack = [i]
            seen_all.add(i)
            while stack:
                u = stack.pop()
                comp.append(u)
                for w, wv in adj[u].items():
                    if w not in seen_all:
                        seen_all.add(w)
                        stack.append(w)
            mn = min(comp)
            relv = {mn: 0}
            st = [mn]
            while st:
                u = st.pop()
                for w, wv in adj[u].items():
                    if w not in relv:
                        relv[w] = relv[u] ^ wv
                        st.append(w)
            comp_of[mn] = set(comp)
            rep_of[mn] = relv
        # 节点所属块
        def find_rep(x):
            for r, nodes in comp_of.items():
                if x in nodes:
                    return r
            return None
        # 自由块 = 不含虚点 n
        free_blocks = [r for r in comp_of if n not in comp_of[r]]
        # 块值基线：含虚点块的代表值 = relv[n]（虚点值 0）；自由块代表值 = 自由变量
        combos = set()
        for mask in range(1 << len(free_blocks)):
            tot = 0
            ok = True
            for x in qs:
                r = find_rep(x)
                if r is None:
                    ok = False
                    break
                rel = rep_of[r]
                if n in comp_of[r]:
                    val = rel[x] ^ rel[n]
                else:
                    val = rel[x] ^ ((mask >> free_blocks.index(r)) & 1)
                tot ^= val
            if ok:
                combos.add(tot)
        if not combos:
            out.append("I don't know.")
        elif len(combos) == 1:
            out.append(str(combos.pop()))
        else:
            out.append("I don't know.")
    return out

# ============ 生成器 ============

def gen_interval(nmax=8, m=10, q=8, use_queries=True):
    n = random.randint(3, nmax)
    a = [random.randint(-9, 9) for _ in range(n + 1)]  # a[1..n]
    S = [0] * (n + 2)
    for i in range(1, n + 1):
        S[i + 1] = S[i] + a[i]
    # S[i] = a[1..i-1] 前 i-1 项和（S[1]=0, S[n+1]=全和）
    ops = []
    for _ in range(m):
        l = random.randint(1, n)
        r = random.randint(l, n)
        v = S[r + 1] - S[l]
        ops.append((l, r, v))
    queries = []
    for _ in range(q):
        l = random.randint(1, n)
        r = random.randint(l, n)
        queries.append((l, r))
    return n, ops, queries

def gen_interval_check(nmax=8, m=12, conflict_ratio=0.4):
    n = random.randint(3, nmax)
    a = [random.randint(-9, 9) for _ in range(n + 1)]
    S = [0] * (n + 2)
    for i in range(1, n + 1):
        S[i + 1] = S[i] + a[i]
    ops = []
    for _ in range(m):
        l = random.randint(1, n)
        r = random.randint(l, n)
        if random.random() < conflict_ratio:
            v = S[r + 1] - S[l] + random.randint(-5, 5)
            if v == S[r + 1] - S[l]:
                v += 1
        else:
            v = S[r + 1] - S[l]
        ops.append((l, r, v))
    return n, ops

def gen_div():
    pool = ['a', 'b', 'c', 'd', 'e', 'f']
    real = {x: random.randint(1, 9) for x in pool}
    eqn = random.randint(2, 6)
    eqs, vals = [], []
    for _ in range(eqn):
        a, b = random.sample(pool, 2)
        eqs.append([a, b])
        vals.append(round(real[a] / real[b], 6))
    qn = random.randint(3, 8)
    qs = []
    for _ in range(qn):
        a = random.choice(pool)
        b = random.choice(pool)
        qs.append([a, b])
    return eqs, vals, qs

def gen_food(nmax=8, m=14):
    n = random.randint(3, nmax)
    typ = [0] * (n + 1)
    for i in range(1, n + 1):
        typ[i] = random.randint(0, 2)
    ops = []
    for _ in range(m):
        r = random.random()
        if r < 0.7:
            # 真话
            if random.random() < 0.5:
                l = random.randint(1, n)
                r2 = random.randint(1, n)
                ops.append((1, l, r2))  # 同类任意（真实可能不同类 -> 对某些是假话？）
                # 注意：同类任意两动物如果真实不同类，在完备推理下是可判假话的
                # 但若它们不在同一已知连通块，check 会放行（当作真话合并）——生成保持自洽：
                # 简单起见真话同类也用真实同类的 pair
            else:
                l = random.randint(1, n)
                # 找 l 吃的动物：typ[(typ[l]-1)%3]
                target = [i for i in range(1, n + 1) if typ[i] == (typ[l] - 1) % 3]
                r2 = random.choice(target) if target else l
                ops.append((2, l, r2))
        else:
            # 假话
            k = random.random()
            if k < 0.3:
                ops.append((random.choice([1, 2]), random.randint(n + 1, n + 3), random.randint(1, n)))
            elif k < 0.5:
                ops.append((2, random.randint(1, n), random.randint(1, n)))  # 可能自吃或真实不符
            else:
                l = random.randint(1, n)
                r2 = random.randint(1, n)
                if random.random() < 0.5:
                    ops.append((1, l, r2))
                else:
                    ops.append((2, l, r2))
    return n, ops

def gen_xor(nmax=7, m=12):
    n = random.randint(3, nmax)
    real = [random.randint(0, 15) for _ in range(n)]
    stmts = []
    out_lines = []
    queries = []
    for _ in range(m):
        r = random.random()
        if r < 0.6:
            if random.random() < 0.5:
                x = random.randint(0, n - 1)
                v = real[x]
                stmts.append((x, n, v))
            else:
                x, y = random.sample(range(n), 2)
                v = real[x] ^ real[y]
                stmts.append((x, y, v))
        else:
            k = random.randint(1, 3)
            qs = random.sample(range(n), k)
            queries.append(qs)
    return n, stmts, queries

# ============ 题4 银河 ============

def gen_galaxy(t=24):
    n = 8
    ops = []
    for _ in range(t):
        r = random.random()
        l = random.randint(1, n)
        r2 = random.randint(1, n)
        if r < 0.5:
            ops.append(('M', l, r2))
        else:
            ops.append(('C', l, r2))
    return n, ops

def brute_galaxy(ops):
    teams = {i: [i] for i in range(1, 9)}
    out = []
    for (op, l, r) in ops:
        tl = [i for i in teams if l in teams[i]][0]
        tr = [i for i in teams if r in teams[i]][0]
        if op == 'M':
            if tl == tr:
                continue
            teams[tr] = teams[tr] + teams[tl]
            del teams[tl]
        else:
            if tl != tr:
                out.append('-1')
            else:
                pos = teams[tl]
                out.append(str(abs(pos.index(l) - pos.index(r)) - 1))
    return out

# ============ 题7 关押罪犯 ============

def gen_criminal(nmax=8, m=12):
    n = random.randint(3, nmax)
    edges = []
    for _ in range(m):
        l = random.randint(1, n)
        r = random.randint(1, n)
        while r == l:
            r = random.randint(1, n)
        v = random.randint(1, 20)
        edges.append((l, r, v))
    return n, edges

def brute_criminal(n, edges):
    # 二分答案 + BFS 二染色：存在分配使 >w 的边全部跨监狱 <-> >w 的边构成二部图
    def feasible(w):
        adj = {i: [] for i in range(1, n + 1)}
        for (l, r, v) in edges:
            if v > w:
                adj[l].append(r)
                adj[r].append(l)
        color = {}
        for s in range(1, n + 1):
            if s in color:
                continue
            color[s] = 0
            stack = [s]
            while stack:
                u = stack.pop()
                for v in adj[u]:
                    if v not in color:
                        color[v] = color[u] ^ 1
                        stack.append(v)
                    elif color[v] == color[u]:
                        return False
        return True
    # 答案在 {0} U {边权} 中取最小可行
    cands = sorted(set([0] + [v for (_, _, v) in edges]))
    for w in cands:
        if feasible(w):
            return w
    return max(v for (_, _, v) in edges)

# ============ 题8 团伙 ============

def gen_gang(nmax=6):
    n = random.randint(3, nmax)
    # 随机真实划分 1..2 帮
    gang = [random.randint(1, 2) for _ in range(n + 1)]
    ops = []
    m = random.randint(3, 8)
    for _ in range(m):
        l = random.randint(1, n)
        r = random.randint(1, n)
        while r == l:
            r = random.randint(1, n)
        if gang[l] == gang[r]:
            ops.append(('F', l, r))
        else:
            ops.append(('E', l, r))
    return n, ops

def brute_gang(n, ops):
    # 迭代闭包：F 直接朋友 union；E a b 时 a 的旧敌人们与 b 互为朋友
    fa = list(range(n + 1))

    def find(x):
        while fa[x] != x:
            fa[x] = fa[fa[x]]
            x = fa[x]
        return x

    def union(a, b):
        ra, rb = find(a), find(b)
        if ra != rb:
            fa[ra] = rb

    enemy = [set() for _ in range(n + 1)]
    for (op, a, b) in ops:
        if op == 'F':
            union(a, b)
        else:
            for e in enemy[a]:
                union(e, b)
            enemy[a].add(b)
            for e in enemy[b]:
                union(e, a)
            enemy[b].add(a)
    return sum(1 for i in range(1, n + 1) if find(i) == i)

# ============ 主流程 ============

def fmt_interval_in(n, ops, queries):
    lines = ['%d %d %d' % (n, len(ops), len(queries))]
    for (l, r, v) in ops:
        lines.append('%d %d %d' % (l, r, v))
    for (l, r) in queries:
        lines.append('%d %d' % (l, r))
    return '\n'.join(lines) + '\n'

def fmt_interval_check_in(n, ops, multi=False):
    if multi:
        lines = ['1', '%d %d' % (n, len(ops))]
        for (l, r, v) in ops:
            lines.append('%d %d %d' % (l, r, v))
    else:
        lines = ['%d %d' % (n, len(ops))]
        for (l, r, v) in ops:
            lines.append('%d %d %d' % (l, r, v))
    return '\n'.join(lines) + '\n'

def fmt_div_in(eqs, vals, qs):
    lines = [str(len(eqs))]
    for (a, b), v in zip(eqs, vals):
        lines.append('%s %s %.6f' % (a, b, v))
    lines.append(str(len(qs)))
    for (a, b) in qs:
        lines.append('%s %s' % (a, b))
    return '\n'.join(lines) + '\n'

def fmt_food_in(n, ops):
    lines = ['%d %d' % (n, len(ops))]
    for (op, l, r) in ops:
        lines.append('%d %d %d' % (op, l, r))
    return '\n'.join(lines) + '\n'

def fmt_xor_in(n, stmts, queries):
    # 需要按时间顺序混合输出（I 与 Q 交错），但我们生成时分开存了
    # 改：重新交错 —— 简单方案：随机打乱顺序并记录原次序语义
    # 但课程语义：I 计数 + 查询都基于当前图。这里重新生成交错流：
    lines = ['%d %d' % (n, len(stmts) + len(queries))]
    # 为保持声明/查询顺序语义，交替生成
    # 简化：按 stmts 全部先声明再查询
    for (l, r, v) in stmts:
        if r == n:
            lines.append('I %d %d' % (l, v))
        else:
            lines.append('I %d %d %d' % (l, r, v))
    for qs in queries:
        lines.append('Q %d %s' % (len(qs), ' '.join(map(str, qs))))
    lines.append('0 0')
    return '\n'.join(lines) + '\n'

def fmt_galaxy_in(n, ops):
    lines = [str(len(ops))]
    for (op, l, r) in ops:
        lines.append('%s %d %d' % (op, l, r))
    return '\n'.join(lines) + '\n'

def fmt_criminal_in(n, edges):
    lines = ['%d %d' % (n, len(edges))]
    for (l, r, v) in edges:
        lines.append('%d %d %d' % (l, r, v))
    return '\n'.join(lines) + '\n'

def fmt_gang_in(n, ops):
    lines = ['%d %d' % (n, len(ops))]
    for (op, l, r) in ops:
        lines.append('%s %d %d' % (op, l, r))
    return '\n'.join(lines) + '\n'

def check(name, got, want, detail=''):
    got_s = got.strip().splitlines()
    want_s = want.strip().splitlines()
    if got_s == want_s:
        return True
    print('[%s] MISMATCH' % name)
    print('  got :', got_s[:12])
    print('  want:', want_s[:12])
    if detail:
        print('  data:', detail)
    return False

def main():
    rounds = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    fails = 0

    # 题1 p8779
    for _ in range(rounds):
        n, ops, qs = gen_interval()
        data = fmt_interval_in(n, ops, qs)
        got = run('p8779.bin', data)
        want = '\n'.join(brute_interval(n, ops, qs)) + '\n'
        if not check('p8779', got, want, data):
            fails += 1
            if fails > 5:
                break
    print('p8779 完成', 'FAIL' if fails else 'OK')

    # 题2 p2294
    for _ in range(rounds):
        n, ops = gen_interval_check()
        data = fmt_interval_check_in(n, ops, multi=True)
        got = run('p2294.bin', data)
        bad, ok = brute_interval_check(n, ops)
        want = ('true' if ok else 'false') + '\n'
        if not check('p2294', got, want, data):
            fails += 1
    print('p2294 完成', 'FAIL' if fails else 'OK')

    # 题3 p3038
    for _ in range(rounds):
        n, ops = gen_interval_check()
        data = fmt_interval_check_in(n, ops, multi=False)
        got = run('p3038.bin', data)
        bad, ok = brute_interval_check(n, ops)
        want = str(bad) + '\n'
        if not check('p3038', got, want, data):
            fails += 1
    print('p3038 完成', 'FAIL' if fails else 'OK')

    # 题4 p1196
    for _ in range(rounds):
        n, ops = gen_galaxy()
        data = fmt_galaxy_in(n, ops)
        got = run('p1196.bin', data)
        want = '\n'.join(brute_galaxy(ops)) + ('\n' if brute_galaxy(ops) else '')
        if not check('p1196', got, want, data):
            fails += 1
    print('p1196 完成', 'FAIL' if fails else 'OK')

    # 题5 p0399
    for _ in range(rounds):
        eqs, vals, qs = gen_div()
        data = fmt_div_in(eqs, vals, qs)
        got = run('p0399_test.bin', data)
        want = '\n'.join(brute_div(eqs, vals, qs)) + '\n'
        g = [float(x) for x in got.strip().splitlines()]
        w = [float(x) for x in want.strip().splitlines()]
        if len(g) != len(w) or any(abs(a - b) > 1e-4 for a, b in zip(g, w)):
            print('[p0399] MISMATCH')
            print('  got :', got.strip().splitlines()[:8])
            print('  want:', want.strip().splitlines()[:8])
            fails += 1
    print('p0399 完成', 'FAIL' if fails else 'OK')

    # 题6 p2024
    for _ in range(rounds):
        n, ops = gen_food()
        data = fmt_food_in(n, ops)
        got = run('p2024.bin', data)
        want = str(brute_food(n, ops)) + '\n'
        if not check('p2024', got, want, data):
            fails += 1
            print('  样例数据:', data)
            break
    print('p2024 完成', 'FAIL' if fails else 'OK')

    # 题7 p1525
    for _ in range(rounds):
        n, edges = gen_criminal()
        data = fmt_criminal_in(n, edges)
        got = run('p1525.bin', data)
        want = str(brute_criminal(n, edges)) + '\n'
        if not check('p1525', got, want, data):
            fails += 1
            print('  样例数据:', data)
            break
    print('p1525 完成', 'FAIL' if fails else 'OK')

    # 题8 p1892
    for _ in range(rounds):
        n, ops = gen_gang()
        data = fmt_gang_in(n, ops)
        got = run('p1892.bin', data)
        want = str(brute_gang(n, ops)) + '\n'
        if not check('p1892', got, want, data):
            fails += 1
            print('  样例数据:', data)
            break
    print('p1892 完成', 'FAIL' if fails else 'OK')

    # 题9 p3234
    for _ in range(rounds):
        n, stmts, queries = gen_xor()
        data = fmt_xor_in(n, stmts, queries)
        got = run('p3234.bin', data)
        want = '\n'.join(brute_xor(n, stmts, queries)) + '\n'
        g_lines = got.strip().splitlines()
        w_lines = want.strip().splitlines()
        # got 第一行是 'Case 1:'，跳过
        if g_lines and g_lines[0].startswith('Case'):
            g_lines = g_lines[1:]
        if g_lines != w_lines:
            print('[p3234] MISMATCH')
            print('  got :', g_lines[:12])
            print('  want:', w_lines[:12])
            print('  样例数据:', data)
            fails += 1
            fails += 1
            print('  样例数据:', data)
            break
    print('p3234 完成', 'FAIL' if fails else 'OK')

    print('\n=== 结果:', 'FAIL' if fails else 'ALL PASS', '===')

if __name__ == '__main__':
    main()
