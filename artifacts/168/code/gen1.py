#!/usr/bin/env python3
"""题1 P3834 区间第 k 小：n<=8, m<=10, 值域 [1,20]（造重复值）"""
import random, sys
seed = int(sys.argv[1])
random.seed(seed)
n = random.randint(1, 8)
m = random.randint(1, 10)
print(n, m)
print(' '.join(str(random.randint(1, 20)) for _ in range(n)))
for _ in range(m):
    l = random.randint(1, n)
    r = random.randint(l, n)
    k = random.randint(1, r - l + 1)
    print(l, r, k)
