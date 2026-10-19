#!/usr/bin/env python3
# 176 课 8 题对拍生成器：每种子一组，写 in/in{1..8}_{seed}.txt
import random, os

os.makedirs('/tmp/cpp176/in', exist_ok=True)

def gen1(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); q = rng.randint(3, 15)
    arr = [rng.randint(1, 6) for _ in range(n)]
    lines = [f"{n}", ' '.join(map(str, arr)), f"{q}"]
    for _ in range(q):
        l = rng.randint(1, n); r = rng.randint(l, n)
        lines.append(f"{l} {r}")
    return '\n'.join(lines) + '\n'

def gen2(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); m = rng.randint(3, 15); k = rng.randint(1, 6)
    arr = [rng.randint(1, k) for _ in range(n)]
    lines = [f"{n} {m} {k}", ' '.join(map(str, arr))]
    for _ in range(m):
        l = rng.randint(1, n); r = rng.randint(l, n)
        lines.append(f"{l} {r}")
    return '\n'.join(lines) + '\n'

def gen3(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); m = rng.randint(3, 15)
    arr = [rng.randint(1, 6) for _ in range(n)]
    lines = [f"{n} {m}", ' '.join(map(str, arr))]
    for _ in range(m):
        l = rng.randint(1, n); r = rng.randint(l, n)
        lines.append(f"{l} {r}")
    return '\n'.join(lines) + '\n'

def gen4(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); m = rng.randint(3, 15)
    arr = [rng.randint(1, 5) for _ in range(n)]
    lines = [f"{n} {m}", ' '.join(map(str, arr))]
    for _ in range(m):
        l = rng.randint(1, n); r = rng.randint(l, n)
        lines.append(f"{l} {r}")
    return '\n'.join(lines) + '\n'

def gen5(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); m = rng.randint(3, 15); k = rng.randint(0, 7)
    arr = [rng.randint(0, 7) for _ in range(n)]
    lines = [f"{n} {m} {k}", ' '.join(map(str, arr))]
    for _ in range(m):
        l = rng.randint(1, n); r = rng.randint(l, n)
        lines.append(f"{l} {r}")
    return '\n'.join(lines) + '\n'

def gen6(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); m = rng.randint(8, 20)
    arr = [rng.randint(1, 6) for _ in range(n)]
    lines = [f"{n} {m}", ' '.join(map(str, arr))]
    for _ in range(m):
        if rng.random() < 0.5:
            l = rng.randint(1, n); r = rng.randint(l, n)
            lines.append(f"Q {l} {r}")
        else:
            pos = rng.randint(1, n); val = rng.randint(1, 6)
            lines.append(f"R {pos} {val}")
    return '\n'.join(lines) + '\n'

def gen7(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); m = rng.randint(8, 20)
    arr = [rng.randint(1, 6) for _ in range(n)]
    lines = [f"{n} {m}", ' '.join(map(str, arr))]
    for _ in range(m):
        if rng.random() < 0.5:
            pos = rng.randint(0, n - 1); val = rng.randint(1, 6)
            lines.append(f"1 {pos} {val}")
        else:
            l = rng.randint(0, n - 1); r = rng.randint(l, n - 1)
            lines.append(f"2 {l} {r}")
    return '\n'.join(lines) + '\n'

def gen8(seed):
    rng = random.Random(seed)
    n = rng.randint(5, 30); m = rng.randint(8, 20)
    arr = [rng.randint(1, 6) for _ in range(n)]
    lines = [f"{n} {m}", ' '.join(map(str, arr))]
    for _ in range(m):
        if rng.random() < 0.5:
            l = rng.randint(1, n); r = rng.randint(l, n)
            lines.append(f"1 {l} {r}")
        else:
            pos = rng.randint(1, n); val = rng.randint(1, 6)
            lines.append(f"2 {pos} {val}")
    return '\n'.join(lines) + '\n'

GENS = {1: gen1, 2: gen2, 3: gen3, 4: gen4, 5: gen5, 6: gen6, 7: gen7, 8: gen8}

for t in range(1, 9):
    for s in range(4):
        with open(f'/tmp/cpp176/in/in{t}_{s}.txt', 'w') as f:
            f.write(GENS[t](1000 + t * 100 + s))
print('generated 32 files')
