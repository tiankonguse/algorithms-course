import random, subprocess, sys

def gen1(rng):
    n = rng.randint(1, 6); m = rng.randint(1, 10)
    arr = [rng.randint(0, 15) for _ in range(n)]
    init_arr = arr[:]
    lines = [f"{n} {m}", ' '.join(map(str, arr))]
    for _ in range(m):
        if rng.random() < 0.4:
            x = rng.randint(0, 15); arr.append(x); n += 1
            lines.append(f"A {x}")
        else:
            l = rng.randint(1, n); r = rng.randint(l, n); x = rng.randint(0, 15)
            lines.append(f"Q {l} {r} {x}")
    return '\n'.join(lines) + '\n', init_arr

def brute1(arr_snapshot, ops):
    arr = arr_snapshot[:]
    out = []
    n = len(arr)
    for op in ops:
        if op[0] == 'A':
            arr.append(op[1]); n += 1
        else:
            l, r, x = op[1], op[2], op[3]
            best = -1
            for p in range(l, r + 1):
                v = 0
                for q in range(p - 1, n):
                    v ^= arr[q]
                v ^= x
                best = max(best, v)
            out.append(best)
    return out

def run1():
    rng = random.Random(20261018)
    fails = 0
    for t in range(200):
        inp, arr = gen1(rng)
        ops = []
        arr2 = arr[:]
        n0 = len(arr)
        for ln in inp.split('\n')[2:]:
            if not ln: continue
            parts = ln.split()
            if parts[0] == 'A': ops.append(('A', int(parts[1])))
            else: ops.append(('Q', int(parts[1]), int(parts[2]), int(parts[3])))
        # brute 需从头模拟
        arrv = arr[:]
        exp = []
        for op in ops:
            if op[0] == 'A': arrv.append(op[1])
            else:
                l, r, x = op[1], op[2], op[3]
                nn = len(arrv)
                best = -1
                for p in range(l, r + 1):
                    v = 0
                    for q in range(p - 1, nn): v ^= arrv[q]
                    v ^= x
                    best = max(best, v)
                exp.append(best)
        got = subprocess.run(['/tmp/cpp159/code1.bin'], input=inp.encode(), capture_output=True).stdout.decode().split()
        exp = [str(e) for e in exp]
        if got != exp:
            fails += 1
            print('MISMATCH 题1 轮', t); print('in:', inp); print('got:', got); print('exp:', exp)
            if fails > 2: break
    print('题1 P4735: 200 轮', '全过' if fails == 0 else f'{fails} FAIL')

def gen7(rng):
    n = rng.randint(2, 8)
    k = rng.randint(0, n * (n - 1) // 2)
    arr = [rng.randint(0, 31) for _ in range(n)]
    return f"{n} {k}\n{' '.join(map(str, arr))}\n", arr, k

def brute7(arr, k):
    xs = []
    for i in range(len(arr)):
        for j in range(i + 1, len(arr)):
            xs.append(arr[i] ^ arr[j])
    xs.sort(reverse=True)
    return sum(xs[:k]) % 1000000007

def run7():
    rng = random.Random(20261018)
    fails = 0
    for t in range(200):
        inp, arr, k = gen7(rng)
        exp = brute7(arr, k)
        got = subprocess.run(['/tmp/cpp159/code7.bin'], input=inp.encode(), capture_output=True).stdout.decode().strip()
        if got != str(exp):
            fails += 1
            print('MISMATCH 题7 轮', t, 'in:', inp.strip().replace('\n',';'), 'got:', got, 'exp:', exp)
            if fails > 2: break
    print('题7 CF241B: 200 轮', '全过' if fails == 0 else f'{fails} FAIL')

run1()
run7()
