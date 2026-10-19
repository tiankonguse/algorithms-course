#!/usr/bin/env python3
"""175 课 7 题对拍生成器：统一生成多组输入到 /tmp/cpp175/in/"""
import os, random

OUT = '/tmp/cpp175/in'
os.makedirs(OUT, exist_ok=True)
SEED_BASE = 20261019

def gen1(seed, n, m):
    rnd = random.Random(seed)
    lines = [f"{n} {m}"]
    lines.append(' '.join(str(rnd.randint(1, 50)) for _ in range(n)))
    for _ in range(m):
        op = rnd.choice('AC')
        x = rnd.randint(1, n)
        y = rnd.randint(1, n) if op == 'C' else rnd.randint(0, x - 1)
        if op == 'A':
            # y 是余数，0 <= y < x
            lines.append(f"A {x} {y}")
        else:
            lines.append(f"C {x} {y}")
    return lines

def gen2(seed, n, q):
    rnd = random.Random(seed)
    lines = [str(n)]
    lines.append(' '.join(str(rnd.randint(1, n)) for _ in range(n)))
    lines.append(str(q))
    for _ in range(q):
        p = rnd.randint(1, n)
        k = rnd.randint(1, n)
        lines.append(f"{p} {k}")
    return lines

def gen3(seed, t, n, q):
    rnd = random.Random(seed)
    lines = [str(t)]
    for _ in range(t):
        lines.append(f"{n} {q}")
        lines.append(' '.join(str(rnd.randint(1, 100)) for _ in range(n)))
        for _ in range(q):
            s = rnd.randint(1, n)
            d = rnd.randint(1, n)
            # k 让 s+(k-1)*d <= n 尽量满足，也偶尔越界
            maxk = (n - s) // d + 1
            k = rnd.randint(1, max(1, maxk)) if rnd.random() < 0.8 else rnd.randint(1, max(1, maxk) + 3)
            lines.append(f"{s} {d} {k}")
    return lines

def gen4(seed, n, m):
    rnd = random.Random(seed)
    lines = [f"{n} {m}"]
    lines.append(' '.join(str(rnd.randint(0, 100)) for _ in range(n)))
    for _ in range(m):
        op = rnd.randint(1, 2)
        if op == 1:
            x = rnd.randint(1, n)
            y = rnd.randint(1, x)   # 保证 y <= x
            z = rnd.randint(1, 100)
            lines.append(f"1 {x} {y} {z}")
        else:
            x = rnd.randint(1, n)
            y = rnd.randint(x, n)
            lines.append(f"2 {x} {y}")
    return lines

def gen5(seed, n, m):
    rnd = random.Random(seed)
    s = rnd.randint(0, n - 1)
    t = rnd.randint(0, n - 1)
    sjump = rnd.randint(1, n)
    tjump = rnd.randint(1, n)
    lines = [f"{n} {m}", f"{s} {sjump} {t} {tjump}"]
    for _ in range(m):
        idx = rnd.randint(0, n - 1)
        jump = rnd.randint(1, n)
        lines.append(f"{idx} {jump}")
    return lines

def gen6(seed, n):
    rnd = random.Random(seed)
    lines = [str(n)]
    lines.append(' '.join(str(rnd.randint(1, n)) for _ in range(n)))
    return lines

def gen7(seed, n):
    rnd = random.Random(seed)
    lines = [str(n)]
    used = set()
    # 随机树：每个点连到更小的点
    for v in range(2, n + 1):
        u = rnd.randint(1, v - 1)
        lines.append(f"{u} {v}")
    return lines

cases = {
    1: [(15, 30), (50, 100), (80, 150), (100, 200)],
    2: [(20, 30), (50, 80), (100, 150), (200, 300)],
    3: [(3, 15, 20), (1, 30, 40), (4, 20, 30), (2, 40, 60)],
    4: [(20, 30), (50, 80), (100, 150), (150, 200)],
    5: [(6, 4), (10, 8), (15, 12), (20, 15)],
    6: [(10,), (20,), (30,), (40,)],
    7: [(6,), (10,), (15,), (20,)],
}

with open(OUT + '/meta.txt', 'w') as meta:
    for ti, (prob, params) in enumerate(cases.items(), 1):
        for ci in range(len(params)):
            seed = SEED_BASE * 100 + prob * 10 + ci
            fn = f'in{prob}_{ci}.txt'
            if prob == 1: lines = gen1(seed, *params[ci])
            elif prob == 2: lines = gen2(seed, *params[ci])
            elif prob == 3: lines = gen3(seed, *params[ci])
            elif prob == 4: lines = gen4(seed, *params[ci])
            elif prob == 5: lines = gen5(seed, *params[ci])
            elif prob == 6: lines = gen6(seed, *params[ci])
            else: lines = gen7(seed, *params[ci])
            with open(f'{OUT}/{fn}', 'w') as f:
                f.write('\n'.join(lines) + '\n')
            meta.write(f'{prob} {fn}\n')
print('生成完毕:', len(open(OUT + '/meta.txt').readlines()), '组')
