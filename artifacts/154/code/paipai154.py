#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""154 课 7 题对拍器：课程 C++ 二进制 vs python 暴力。结果写 /tmp/cpp154/paipai154.out"""
import random, subprocess, sys, os, time

CPP = '/tmp/cpp154'
random.seed(20261018)
R = random.randrange

# ---------- 1. P3377 左偏树模板（LeftistTree2）----------
def brute_p3377(n, nums, ops):
    """暴力：集合 = list of list。删除标记 del[i]"""
    groups = [None] + [[i] for i in range(1, n+1)]  # 每个节点所属集合（按编号）
    val = nums[:]  # 1-based 拷贝一份
    deleted = [False]*(n+1)
    out = []
    for op, a, b in ops:
        if op == 1:
            if not deleted[a] and not deleted[b]:
                ga, gb = groups[a], groups[b]
                if ga is not gb:
                    # 合并：小值头（相等编号小）——暴力直接合并列表，找最小用 min
                    ga.extend(gb)
                    for v in gb:
                        groups[v] = ga
        else:
            if deleted[a]:
                out.append(-1)
            else:
                g = groups[a]
                # 最小值 = min(值, 编号) —— 课程语义：小值当头，相等取编号小
                mn = min(g, key=lambda i: (val[i], i))
                out.append(val[mn])
                deleted[mn] = True
    return out

def gen_p3377():
    n = R(2, 8); m = R(1, 40)
    nums = [0] + [R(0, 20) for _ in range(n)]
    ops = []
    for _ in range(m):
        op = R(1, 2)
        if op == 1:
            ops.append((1, R(1, n), R(1, n)))
        else:
            ops.append((2, R(1, n)))
    return n, m, nums, ops

def run_p3377():
    fails = 0
    for t in range(300):
        n, m, nums, ops = gen_p3377()
        inp = [f"{n} {m}"]
        inp.append(' '.join(map(str, nums[1:])))
        for op, a, b in ops:
            inp.append(f"{op} {a} {b}" if op == 1 else f"{op} {a}")
        inp = '\n'.join(inp) + '\n'
        p = subprocess.run([f'{CPP}/Code01_LeftistTree2.bin'], input=inp.encode(), capture_output=True)
        got = p.stdout.decode().split()
        want = list(map(str, brute_p3377(n, nums, ops)))
        if got != want:
            fails += 1
            if fails <= 2:
                print(f'P3377 MISMATCH t={t}\ngot={got}\nwant={want}\ninp={inp!r}', file=sys.stderr)
    print(f'P3377 300 轮: {fails} fails')

# ---------- 2. P2713 增强模板（LeftistTree4）----------
def brute_p2713(n, nums, ops):
    groups = [None] + [[i] for i in range(1, n+1)]
    val = nums[:]
    deleted = [False]*(n+1)
    out = []
    for op, a, b in ops:
        if op == 'M':
            if not deleted[a] and not deleted[b]:
                ga, gb = groups[a], groups[b]
                if ga is not gb:
                    ga.extend(gb)
                    for v in gb:
                        groups[v] = ga
        else:
            if deleted[a]:
                out.append(0)
            else:
                g = groups[a]
                mn = min(g, key=lambda i: (val[i], i))
                out.append(val[mn])
                deleted[mn] = True
    return out

def gen_p2713():
    n = R(2, 6); m = R(1, 30)
    nums = [0] + [R(0, 10) for _ in range(n)]
    ops = []
    for _ in range(m):
        op = R(0, 1)
        if op == 0:
            ops.append(('M', R(1, n), R(1, n)))
        else:
            ops.append(('K', R(1, n)))
    return n, m, nums, ops

def run_p2713():
    fails = 0
    for t in range(200):
        n, m, nums, ops = gen_p2713()
        inp = [f"{n}", ' '.join(map(str, nums[1:])), f"{m}"]
        for op, a, b in ops:
            inp.append(f"{op} {a} {b}" if op == 'M' else f"{op} {a}")
        inp = '\n'.join(inp) + '\n'
        p = subprocess.run([f'{CPP}/Code01_LeftistTree4.bin'], input=inp.encode(), capture_output=True)
        got = p.stdout.decode().split()
        want = list(map(str, brute_p2713(n, nums, ops)))
        if got != want:
            fails += 1
            if fails <= 2:
                print(f'P2713 MISMATCH t={t}\ngot={got}\nwant={want}\ninp={inp!r}', file=sys.stderr)
    print(f'P2713 200 轮: {fails} fails')

# ---------- 3. P4971 断罪者（Convict2）----------
def brute_p4971(t, w, k, people):
    """people: list of (n, m, nums, ops). 返回每人一行输出（Gensokyo/Hell/Heaven）"""
    out = []
    for n, m, nums, ops in people:
        # 集合：set of 编号。比较用 (值, -编号)：值大优先，值等编号小优先
        belong = list(range(n+1))  # 节点 -> 集合 id
        sets = [set()] + [{i} for i in range(1, n+1)]
        val = nums[:]
        def setmax(s):
            return max(s, key=lambda i: (val[i], -i))
        for op, a, b in ops:
            if op == 2:
                sa = sets[belong[a]]
                sa.discard(a)
                val[a] = 0
                sa.add(a)
            elif op == 3:
                sa = sets[belong[a]]
                mx = setmax(sa)
                sa.discard(mx)
                val[mx] = max(val[mx] - b, 0)
                sa.add(mx)
            else:
                ga, gb = belong[a], belong[b]
                if ga != gb:
                    # 大并小
                    if len(sets[ga]) < len(sets[gb]):
                        ga, gb = gb, ga
                    for v in sets[gb]:
                        belong[v] = ga
                    sets[ga].update(sets[gb])
                    sets[gb] = set()
        # compute：每个集合堆顶（值最大编号小优先）罪恶值求和
        ans = 0; mx = 0
        seen = set()
        for i in range(1, n+1):
            g = belong[i]
            if g in seen or not sets[g]:
                continue
            seen.add(g)
            top = setmax(sets[g])
            ans += val[top]
            mx = max(mx, val[top])
        if w == 2: ans -= mx
        elif w == 3: ans += mx
        if ans == 0: out.append(f"Gensokyo {ans}")
        elif ans > k: out.append(f"Hell {ans}")
        else: out.append(f"Heaven {ans}")
    return out

def gen_p4971():
    t = R(1, 3); w = R(1, 3); k = R(0, 50)
    people = []
    for _ in range(t):
        n = R(2, 6); m = R(1, 20)
        nums = [0] + [R(0, 30) for _ in range(n)]
        ops = []
        for _ in range(m):
            op = R(2, 4)
            if op == 2:
                ops.append((2, R(1, n), None))
            elif op == 3:
                ops.append((3, R(1, n), R(1, 10)))
            else:
                ops.append((4, R(1, n), R(1, n)))
        people.append((n, m, nums, ops))
    return t, w, k, people

def run_p4971():
    fails = 0
    for _ in range(300):
        t, w, k, people = gen_p4971()
        inp = [f"{t} {w} {k}"]
        for n, m, nums, ops in people:
            inp.append(f"{n} {m}")
            inp.append(' '.join(map(str, nums[1:])))
            for op, a, b in ops:
                if op == 3:
                    inp.append(f"{op} {a} {b}")
                else:
                    inp.append(f"{op} {a}")
        inp = '\n'.join(inp) + '\n'
        p = subprocess.run([f'{CPP}/Code02_Convict2.bin'], input=inp.encode(), capture_output=True)
        got = p.stdout.decode().strip().split('\n')
        want = brute_p4971(t, w, k, people)
        if got != want:
            fails += 1
            if fails <= 2:
                print(f'P4971 MISMATCH\ngot={got}\nwant={want}\ninp={inp!r}', file=sys.stderr)
    print(f'P4971 300 轮: {fails} fails')


# ---------- 4. P1456 猴王（MonkeyKing2）----------
def brute_p1456(n, nums, fights):
    groups = [None] + [[i] for i in range(1, n+1)]
    val = nums[:]
    out = []
    for x, y in fights:
        gx, gy = groups[x], groups[y]
        if gx is gy:
            out.append(-1)
            continue
        # 各阵营最强：值最大，相等编号小（课程 merge 只比值不比编号，但实际堆顶=值最大）
        a = max(gx, key=lambda i: val[i])
        b = max(gy, key=lambda i: val[i])
        val[a] //= 2; val[b] //= 2
        # 合并
        gx.extend(gy)
        for v in gy:
            groups[v] = gx
        out.append(val[max(gx, key=lambda i: val[i])])
    return out

def run_p1456():
    fails = 0
    for t in range(300):
        n = R(2, 6); m = R(1, 30)
        nums = [0] + [R(0, 15) for _ in range(n)]
        fights = [(R(1, n), R(1, n)) for _ in range(m)]
        inp = [f"{n}", ' '.join(map(str, nums[1:])), f"{m}"]
        for x, y in fights:
            inp.append(f"{x} {y}")
        inp = '\n'.join(inp) + '\n'
        p = subprocess.run([f'{CPP}/Code03_MonkeyKing2.bin'], input=inp.encode(), capture_output=True)
        got = p.stdout.decode().split()
        want = list(map(str, brute_p1456(n, nums, fights)))
        if got != want:
            fails += 1
            if fails <= 2:
                print(f'P1456 MISMATCH t={t}\ngot={got}\nwant={want}\ninp={inp!r}', file=sys.stderr)
    print(f'P1456 300 轮: {fails} fails')

# ---------- 5. P1552 派遣（Dispatch2）----------
def brute_p1552(n, m, leader, cost, ability):
    # 每个节点 i：子树内所有节点集合
    ans = 0
    for i in range(1, n+1):
        sub = set()
        def collect(u):
            sub.add(u)
            for v in range(u+1, n+1):
                if leader[v] == u:
                    collect(v)
        collect(i)
        cs = sorted(cost[u] for u in sub)
        cnt, sm = 0, 0
        for c in cs:
            if sm + c <= m:
                sm += c; cnt += 1
            else:
                break
        ans = max(ans, cnt * ability[i])
    return ans

def run_p1552():
    fails = 0
    for t in range(300):
        n = R(1, 8)
        m = R(5, 30)
        leader = [0]*(n+1); cost = [0]*(n+1); ability = [0]*(n+1)
        for i in range(2, n+1):
            leader[i] = 1 if i == 2 else R(1, i-1)  # 上级编号 < 编号
        for i in range(1, n+1):
            cost[i] = R(1, m)
            ability[i] = R(1, 20)
        inp = [f"{n} {m}"]
        for i in range(1, n+1):
            inp.append(f"{leader[i]} {cost[i]} {ability[i]}")
        inp = '\n'.join(inp) + '\n'
        p = subprocess.run([f'{CPP}/Code04_Dispatch2.bin'], input=inp.encode(), capture_output=True)
        got = p.stdout.decode().split()
        want = [str(brute_p1552(n, m, leader, cost, ability))]
        if got != want:
            fails += 1
            if fails <= 2:
                print(f'P1552 MISMATCH t={t}\ngot={got}\nwant={want}\ninp={inp!r}', file=sys.stderr)
    print(f'P1552 300 轮: {fails} fails')

# ---------- 6. P4331 数字序列（NumberSequence2）----------
def brute_p4331(A):
    """课程语义：arr[i] = A[i]-i（1-based），对 arr 求不降 C，B[i]=C[i]+i，sum=sum|arr-C|"""
    n = len(A)
    arr = [A[i] - (i+1) for i in range(n)]   # 0-based：arr[i] = A[i] - (i+1)
    VMIN, VMAX = -10, 30
    INF = 10**18
    dp = [[INF]*(VMAX-VMIN+1) for _ in range(n+1)]
    dp[0] = [0]*(VMAX-VMIN+1)
    for i in range(1, n+1):
        for v in range(VMAX-VMIN+1):
            best = min(dp[i-1][:v+1])
            dp[i][v] = best + abs(arr[i-1] - (VMIN+v))
    bestv = min(range(VMAX-VMIN+1), key=lambda v: dp[n][v])
    C = [0]*n
    cur = bestv
    for i in range(n, 0, -1):
        C[i-1] = cur
        for v in range(cur+1):
            if dp[i-1][v] + abs(arr[i-1]-(VMIN+cur)) == dp[i][cur]:
                cur = v
                break
    B = [VMIN + c + (i+1) for i, c in enumerate(C)]   # B[i] = C[i] + i (1-based)
    return dp[n][bestv], B

def run_p4331():
    fails = 0
    for t in range(200):
        n = R(1, 6)
        A = [R(0, 20) for _ in range(n)]
        inp = [f"{n}", ' '.join(map(str, A))]
        inp = '\n'.join(inp) + '\n'
        p = subprocess.run([f'{CPP}/Code05_NumberSequence2.bin'], input=inp.encode(), capture_output=True)
        lines = p.stdout.decode().strip().split('\n')
        got_sum = int(lines[0])
        got_B = list(map(int, lines[1].split())) if len(lines) > 1 and lines[1] else []
        want_sum, want_B = brute_p4331(A)
        # 注意：课程求的是 A[i]-i 的不降 + i（B[i] = ans[i] + i，arr[i] = A[i]-i）
        # 因此课程输出 B 是「递增」的；暴力 DP 求的是不降。对拍时应比较 sum 相同 + B 递增
        ok_sum = got_sum == want_sum
        ok_inc = all(got_B[i] < got_B[i+1] for i in range(len(got_B)-1)) if len(got_B) >= 2 else True
        if not ok_sum:
            fails += 1
            if fails <= 2:
                print(f'P4331 MISMATCH t={t} sum got={got_sum} want={want_sum}\nA={A}\nB={got_B}\ninp={inp!r}', file=sys.stderr)
    print(f'P4331 200 轮: {fails} fails (仅比 sum)')

# ---------- 7. P11266 可并堆2（FollowUp2）----------
def brute_p11266(n, nums, ops):
    sets = [{i} for i in range(1, n+1)]
    sets.insert(0, None)
    val = nums[:]
    out = []
    for op, x, y, z in ops:
        if op == 0:
            sx = sets[x]
            sx.discard(y)
        elif op == 1:
            if not sets[x]:
                out.append(0)
            else:
                mn = min(sets[x], key=lambda i: (val[i], i))
                out.append(val[mn])
        elif op == 2:
            sx, sy = sets[x], sets[y]
            sx.update(sy)
            sets[y] = set()
        else:
            sx = sets[x]
            sx.discard(y)
            val[y] = z
            sx.add(y)
    return out

def run_p11266():
    fails = 0
    for t in range(200):
        n = R(2, 6); m = R(1, 30)
        nums = [0] + [R(0, 15) for _ in range(n)]
        # gen 端维护集合，保证 op0/op3 只操作集合内元素；op2 合并后 y 作废（题目保证）
        gsets = [set()] + [{i} for i in range(1, n+1)]
        alive = set(range(1, n+1))
        ops = []
        for _ in range(m):
            op = R(0, 3)
            if op == 0:
                x = sorted(alive)[R(0, len(alive))]
                if gsets[x]:
                    sl = sorted(gsets[x])
                    y = sl[R(0, len(sl))]
                    gsets[x].discard(y)
                    ops.append((0, x, y, None))
                else:
                    ops.append((1, x, None, None))
            elif op == 1:
                ops.append((1, sorted(alive)[R(0, len(alive))], None, None))
            elif op == 2:
                if len(alive) >= 2:
                    la = sorted(alive)
                    x = la[R(0, len(la))]
                    la2 = [v for v in la if v != x]
                    y = la2[R(0, len(la2))]
                    gsets[x].update(gsets[y]); gsets[y] = set()
                    alive.discard(y)
                    ops.append((2, x, y, None))
                else:
                    ops.append((1, sorted(alive)[R(0, len(alive))], None, None))
            else:
                x = sorted(alive)[R(0, len(alive))]
                if gsets[x]:
                    sl = sorted(gsets[x])
                    y = sl[R(0, len(sl))]
                    gsets[x].discard(y)
                    z = R(0, 15)
                    gsets[x].add(y)
                    ops.append((3, x, y, z))
                else:
                    ops.append((1, x, None, None))
        inp = [f"{n} {m}", ' '.join(map(str, nums[1:]))]
        for op, x, y, z in ops:
            if op == 0: inp.append(f"0 {x} {y}")
            elif op == 1: inp.append(f"1 {x}")
            elif op == 2: inp.append(f"2 {x} {y}")
            else: inp.append(f"3 {x} {y} {z}")
        inp = '\n'.join(inp) + '\n'
        p = subprocess.run([f'{CPP}/FollowUp2.bin'], input=inp.encode(), capture_output=True)
        got = p.stdout.decode().split()
        want = list(map(str, brute_p11266(n, nums, ops)))
        if got != want:
            fails += 1
            if fails <= 2:
                print(f'P11266 MISMATCH t={t}\ngot={got}\nwant={want}\ninp={inp!r}', file=sys.stderr)
    print(f'P11266 200 轮: {fails} fails')

if __name__ == '__main__':
    buf = open(f'{CPP}/paipai154.out', 'w')
    sys.stdout = buf
    t0 = time.time()
    run_p3377()
    run_p2713()
    run_p4971()
    run_p1456()
    run_p1552()
    run_p4331()
    run_p11266()
    buf.write(f'elapsed {time.time()-t0:.1f}s\n')
    buf.close()
    sys.stdout = sys.__stdout__
    print('done, see /tmp/cpp154/paipai154.out')
