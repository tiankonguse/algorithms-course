import random, subprocess, sys

B3 = '/tmp/cpp160/Code03_SegmentWithBalanced2.bin'
B7 = '/tmp/cpp160/Code07_NetworkManagement2.bin'

def run(binp, inp):
    p = subprocess.run([binp], input=inp, capture_output=True, text=True)
    return p.stdout.strip().split('\n') if p.stdout.strip() else []

# ---------- 题3 P3380：线段树套平衡树 ----------
def gen3(seed):
    rnd = random.Random(seed)
    n = rnd.randint(1, 20)
    m = rnd.randint(1, 60)
    arr = [rnd.randint(0, 30) for _ in range(n)]
    ops = []
    for _ in range(m):
        op = rnd.randint(1, 5)
        x = rnd.randint(1, n); y = rnd.randint(1, n)
        if x > y: x, y = y, x
        if op == 3:
            ops.append((op, x, rnd.randint(0, 30), 0))
        elif op == 2:
            ops.append((op, x, y, rnd.randint(1, y - x + 1)))
        else:
            z = rnd.randint(0, 30)
            ops.append((op, x, y, z))
    lines = [f'{n} {m}', ' '.join(map(str, arr))]
    for op, x, y, z in ops:
        if op == 3:
            lines.append(f'{op} {x} {y}')
        else:
            lines.append(f'{op} {x} {y} {z}')
    return '\n'.join(lines) + '\n', arr, ops

def brute3(arr, ops):
    a = arr[:]
    out = []
    for op, x, y, z in ops:
        if op == 1:
            seg = a[x-1:y]
            out.append(str(sum(1 for v in seg if v < z) + 1))
        elif op == 2:
            seg = sorted(a[x-1:y])
            out.append(str(seg[z-1]))
        elif op == 3:
            a[x-1] = y
        elif op == 4:
            seg = a[x-1:y]
            pre = -2147483647
            for v in seg:
                if v < z and v > pre: pre = v
            out.append(str(pre))
        else:
            seg = a[x-1:y]
            post = 2147483647
            for v in seg:
                if v > z and v < post: post = v
            out.append(str(post))
    return out

# ---------- 题7 P4175：网络管理 ----------
def gen7(seed):
    rnd = random.Random(seed)
    n = rnd.randint(1, 30)
    m = rnd.randint(1, 40)
    arr = [rnd.randint(1, 50) for _ in range(n)]
    edges = []
    for v in range(2, n+1):
        u = rnd.randint(1, v-1)
        edges.append((u, v))
    ops = []
    for _ in range(m):
        if rnd.random() < 0.3:
            ops.append((0, rnd.randint(1, n), rnd.randint(1, 50)))
        else:
            k = rnd.randint(1, n)
            ops.append((k, rnd.randint(1, n), rnd.randint(1, n)))
    lines = [f'{n} {m}', ' '.join(map(str, arr))]
    for u, v in edges: lines.append(f'{u} {v}')
    for op, x, y in ops: lines.append(f'{op} {x} {y}')
    return '\n'.join(lines) + '\n', n, arr, edges, ops

def brute7(n, arr, edges, ops):
    adj = [[] for _ in range(n+1)]
    for u, v in edges:
        adj[u].append(v); adj[v].append(u)
    a = arr[:]
    out = []
    # 每轮 BFS 求路径
    for op, x, y in ops:
        if op == 0:
            a[x-1] = y
            continue
        # BFS x -> y
        par = {x: 0}
        q = [x]; seen = {x}
        while q:
            u = q.pop(0)
            if u == y: break
            for v in adj[u]:
                if v not in seen:
                    seen.add(v); par[v] = u; q.append(v)
        path = []
        cur = y
        while cur != 0:
            path.append(a[cur-1]); cur = par.get(cur, 0)
        if op > len(path):
            out.append('invalid request!')
        else:
            path.sort(reverse=True)
            out.append(str(path[op-1]))
    return out

# ---------- 主循环 ----------
def main():
    seed = 20261018
    fails3 = 0
    # 题3
    for i in range(200):
        inp, arr, ops = gen3(seed + i)
        got = run(B3, inp)
        exp = brute3(arr, ops)
        if got != exp:
            fails3 += 1
            print('MISMATCH 题3 轮', i)
            print('inp:', inp)
            print('got:', got[:8])
            print('exp:', exp[:8])
            break
    print('题3 P3380: 200 轮', '全过' if fails3 == 0 else 'FAIL')
    fails7 = 0
    # 题7
    for i in range(100):
        inp, n, arr, edges, ops = gen7(seed + 1000 + i)
        got = run(B7, inp)
        exp = brute7(n, arr, edges, ops)
        if got != exp:
            fails7 += 1
            print('MISMATCH 题7 轮', i)
            print('inp:', inp)
            print('got:', got[:8])
            print('exp:', exp[:8])
            break
    print('题7 P4175: 100 轮', '全过' if fails7 == 0 else 'FAIL')
    sys.exit(1 if (fails3 or fails7) else 0)

main()
