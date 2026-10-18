#!/usr/bin/env python3
"""题2 P1527 矩阵内第 k 小：n<=4, q<=8, 值域 [0,9]（造重复值）"""
import random, sys
seed = int(sys.argv[1])
random.seed(seed)
n = random.randint(1, 4)
q = random.randint(1, 8)
print(n, q)
for _ in range(n):
    print(' '.join(str(random.randint(0, 9)) for _ in range(n)))
for _ in range(q):
    a = random.randint(1, n); c = random.randint(a, n)
    b = random.randint(1, n); d = random.randint(b, n)
    k = random.randint(1, (c - a + 1) * (d - b + 1))
    print(a, b, c, d, k)
