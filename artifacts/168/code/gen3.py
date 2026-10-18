#!/usr/bin/env python3
"""题3 P3527 陨石雨：n<=4, m<=6, k<=6, need<=20, num<=5"""
import random, sys
seed = int(sys.argv[1])
random.seed(seed)
n = random.randint(1, 4)
m = random.randint(1, 6)
k = random.randint(1, 6)
print(n, m)
print(' '.join(str(random.randint(1, n)) for _ in range(m)))
print(' '.join(str(random.randint(1, 20)) for _ in range(n)))
print(k)
for _ in range(k):
    l = random.randint(1, m)
    r = random.randint(1, m)
    num = random.randint(1, 5)
    print(l, r, num)
