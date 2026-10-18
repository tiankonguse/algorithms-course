#!/usr/bin/env python3
"""157 课 5 题对拍：P3919 / P3834 / SP11470(TTM) / P3372 / HDU4348
课程代码为 /tmp/cpp157/*.bin，暴力按版本数组快照 / 切片排序模拟。
"""
import random, subprocess, sys

random.seed(20261018)
BIN = '/tmp/cpp157'
ROUNDS = 300
fails = 0

def run(binname, inp):
    p = subprocess.run([f'{BIN}/{binname}.bin'], input=inp.encode(),
                       capture_output=True, timeout=10)
    if p.returncode != 0:
        return 'RUNTIME_ERROR: ' + p.stderr.decode(errors='replace')[:200]
    return p.stdout.decode().strip()

def cmp(a, b):
    a = a.strip().split('\n') if a.strip() else []
    b = b.strip().split('\n') if b.strip() else []
    if len(a) != len(b):
        return f'len {len(a)} vs {len(b)}'
    for i, (x, y) in enumerate(zip(a, b)):
        if x != y:
            return f'line {i}: {x!r} vs {y!r}'
    return None

# ---- 题1 P3919 单点改+单点查，版本=操作计数 ----
def gen1():
    n = random.randint(1, 25)
    m = random.randint(1, 40)
    arr = [random.randint(-9, 9) for _ in range(n)]
    ops = []
    for i in range(m):
        v = random.randint(0, i)  # 基于已有版本（前 i 个操作产生 0..i-1 版本，i 操作后新版本=i+1）
        op = random.choice([1, 2])
        x = random.randint(1, n)
        if op == 1:
            y = random.randint(-9, 9)
            ops.append(f'{v} 1 {x} {y}')
        else:
            ops.append(f'{v} 2 {x}')
    return f'{n} {m}\n' + ' '.join(map(str, arr)) + '\n' + '\n'.join(ops)

def brute1(inp):
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    arr = list(map(int, lines[1].split()))
    versions = [arr[:]]
    out = []
    for i, line in enumerate(lines[2:], 1):
        p = line.split()
        v = int(p[0]); op = int(p[1]); x = int(p[2])
        if op == 1:
            y = int(p[3])
            nv = versions[v][:]
            nv[x - 1] = y
            versions.append(nv)
        else:
            versions.append(versions[v][:])
            out.append(str(versions[i][x - 1]))
    return '\n'.join(out)

# ---- 题2 P3834 区间第 k 小 ----
def gen2():
    n = random.randint(1, 40)
    m = random.randint(1, 40)
    arr = [random.randint(0, 50) for _ in range(n)]
    ops = []
    for _ in range(m):
        l = random.randint(1, n)
        r = random.randint(l, n)
        k = random.randint(1, r - l + 1)
        ops.append(f'{l} {r} {k}')
    return f'{n} {m}\n' + ' '.join(map(str, arr)) + '\n' + '\n'.join(ops)

def brute2(inp):
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    arr = list(map(int, lines[1].split()))
    out = []
    for line in lines[2:]:
        l, r, k = map(int, line.split())
        seg = sorted(arr[l - 1:r])
        out.append(str(seg[k - 1]))
    return '\n'.join(out)

# ---- 题3 SP11470/TTM 范围改可持久化经典（C/Q/H/B）----
def gen3():
    n = random.randint(1, 20)
    m = random.randint(1, 40)
    arr = [random.randint(-9, 9) for _ in range(n)]
    ops = []
    t = 0
    for _ in range(m):
        op = random.choice(['C', 'Q', 'H', 'B'])
        if op == 'C':
            x = random.randint(1, n); y = random.randint(x, n)
            z = random.randint(-5, 5)
            ops.append(f'C {x} {y} {z}')
            t += 1
        elif op == 'Q':
            x = random.randint(1, n); y = random.randint(x, n)
            ops.append(f'Q {x} {y}')
        elif op == 'H':
            x = random.randint(1, n); y = random.randint(x, n)
            z = random.randint(0, t)
            ops.append(f'H {x} {y} {z}')
        else:
            x = random.randint(0, t)
            ops.append(f'B {x}')
            t = x
    return f'{n} {m}\n' + ' '.join(map(str, arr)) + '\n' + '\n'.join(ops)

def brute35(inp):
    """题3/题5 共用：C/Q/H/B 版本数组快照。
    课程语义：root[t+1] 覆盖式——B 回溯 t 后 C 会覆盖旧版本号。"""
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    arr = list(map(int, lines[1].split()))
    versions = [arr[:]]  # versions[0] 固定；C 时覆盖 versions[t+1]
    t = 0
    out = []
    for line in lines[2:]:
        p = line.split()
        if p[0] == 'C':
            x, y, z = int(p[1]), int(p[2]), int(p[3])
            nv = versions[t][:]
            for i in range(x - 1, y):
                nv[i] += z
            while len(versions) <= t + 1:
                versions.append(None)
            versions[t + 1] = nv
            t += 1
        elif p[0] == 'Q':
            x, y = int(p[1]), int(p[2])
            out.append(str(sum(versions[t][x - 1:y])))
        elif p[0] == 'H':
            x, y, z = int(p[1]), int(p[2]), int(p[3])
            out.append(str(sum(versions[z][x - 1:y])))
        else:  # B
            t = int(p[1])
    return '\n'.join(out)

# ---- 题4 P3372 普通线段树标记永久化（范围加 + 区间和）----
def gen4():
    n = random.randint(1, 40)
    m = random.randint(1, 40)
    arr = [random.randint(-9, 9) for _ in range(n)]
    ops = []
    for _ in range(m):
        op = random.choice([1, 2])
        x = random.randint(1, n); y = random.randint(x, n)
        if op == 1:
            k = random.randint(-5, 5)
            ops.append(f'1 {x} {y} {k}')
        else:
            ops.append(f'2 {x} {y}')
    return f'{n} {m}\n' + ' '.join(map(str, arr)) + '\n' + '\n'.join(ops)

def brute4(inp):
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    arr = list(map(int, lines[1].split()))
    out = []
    for line in lines[2:]:
        p = line.split()
        if p[0] == '1':
            x, y, k = int(p[1]), int(p[2]), int(p[3])
            for i in range(x - 1, y):
                arr[i] += k
        else:
            x, y = int(p[1]), int(p[2])
            out.append(str(sum(arr[x - 1:y])))
    return '\n'.join(out)

# ---- 主循环 ----
JOBS = [
    ('题1 P3919 单点可持久化', 'Code01_PointPersistent2', gen1, brute1),
    ('题2 P3834 区间第k小', 'Code02_PointPersistent2', gen2, brute2),
    ('题3 SP11470 范围改经典', 'Code03_RangePersistentClassic2', gen3, brute35),
    ('题4 P3372 标记永久化', 'Code04_TagPermanentization2', gen4, brute4),
    ('题5 HDU4348 永久化可持久', 'Code05_RangePersistentLessSpace2', gen3, brute35),
]
global_fails = 0
for name, binname, gen, brute in JOBS:
    ok = 0
    for r in range(ROUNDS):
        inp = gen()
        a = run(binname, inp)
        b = brute(inp)
        d = cmp(a, b)
        if d:
            global_fails += 1
            print(f'[{name}] 第{r}轮 MISMATCH: {d}')
            if global_fails <= 2:
                print('  input:\n' + inp)
                print('  course:\n' + a)
                print('  brute:\n' + b)
            break
        ok += 1
    print(f'{name}: {ok}/{ROUNDS} 轮 OK' if ok == ROUNDS else f'{name}: FAIL（第{ok}轮）')
print('=== 全局 fails:', global_fails, '===')
sys.exit(1 if global_fails else 0)
