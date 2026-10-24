#!/usr/bin/env python3
"""gen196.py <题号> <seed> → stdout。小数据随机生成器，8 题统一入口。"""
import random, sys

def p1(rng):
    n = rng.randint(2, 6); m = rng.randint(1, 12)
    print(n, m)
    for _ in range(m):
        x1 = rng.randint(1, n); v1 = rng.randint(0, 1)
        x2 = rng.randint(1, n); v2 = rng.randint(0, 1)
        print(x1, v1, x2, v2)

def p2(rng):
    n = rng.randint(2, 6); m = rng.randint(0, 10)
    print(n, m)
    used = set()
    for _ in range(m):
        while True:
            u = rng.randint(1, 2 * n); v = rng.randint(1, 2 * n)
            if u == v or (u, v) in used or (v, u) in used: continue
            used.add((u, v)); break
        print(u, v)

def p3(rng):
    n = rng.randint(2, 7); m = rng.randint(1, 10)
    print(n, m)
    for i in range(1, n + 1): print(rng.randint(10, 60), end=' ')
    print()
    for _ in range(m):
        print(rng.randint(1, n), rng.randint(1, n))
    print(0, 0)

def p4(rng):
    t = rng.randint(1, 2)
    print(t)
    for _ in range(t):
        n = rng.randint(1, 4); m = rng.randint(0, 8); k = rng.randint(2, 4)
        print(n, m, k)
        for _ in range(m):
            op = rng.randint(1, 3)
            if op == 1:
                print(op, rng.randint(1, n), rng.randint(1, k))
            else:
                i = rng.randint(1, n); j = rng.randint(1, n)
                x = rng.randint(2, 2 * k)
                print(op, i, j, x)

def p5(rng):
    n = rng.randint(2, 7)
    print(n)
    # 生成对称认识图
    mat = [[False] * (n + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(i + 1, n + 1):
            if rng.random() < 0.5:
                mat[i][j] = mat[j][i] = True
    for i in range(1, n + 1):
        lst = [j for j in range(1, n + 1) if mat[i][j] and j != i]
        print(len(lst), *lst)

def p6(rng):
    n = rng.randint(3, 8); m = rng.randint(1, 10); k = rng.randint(1, 4)
    # 每个阵营至少一个点：先各分 1 点，其余随机
    k = min(k, n)
    camp = list(range(1, k + 1))
    for i in range(k + 1, n + 1):
        camp.append(rng.randint(1, k))
    rng.shuffle(camp)
    print(n, m, k)
    used = set()
    for _ in range(m):
        while True:
            u = rng.randint(1, n); v = rng.randint(1, n)
            if u == v or (u, v) in used: continue
            used.add((u, v)); break
        print(u, v)
    groups = [[] for _ in range(k + 1)]
    for i in range(1, n + 1): groups[camp[i - 1]].append(i)
    for g in groups[1:]:
        print(len(g), *g)

def p7(rng):
    n = rng.randint(2, 6)
    print(n)
    for _ in range(n):
        a = rng.randint(2, 20); b = rng.randint(2, 20)
        print(a, b)

def p8(rng):
    n = rng.randint(2, 3); p = rng.randint(2, 6); f = rng.randint(2, 6); m = rng.randint(2, 4)
    print(n, p, f, m)
    for _ in range(n):
        print(rng.randint(1, p), rng.randint(1, p))
    for i in range(1, p + 1):
        l = rng.randint(1, f); r = rng.randint(l, f)
        print(l, r)
    for _ in range(m):
        print(rng.randint(1, p), rng.randint(1, p))

def main():
    no, seed = int(sys.argv[1]), int(sys.argv[2])
    rng = random.Random(seed)
    fn = {1: p1, 2: p2, 3: p3, 4: p4, 5: p5, 6: p6, 7: p7, 8: p8}[no]
    fn(rng)

if __name__ == '__main__':
    main()
