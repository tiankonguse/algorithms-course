import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 6); k = random.randint(3, 8); m = random.randint(3, 12)
print(n, m, k)
for _ in range(m):
    x = random.randint(1, n); y = random.randint(1, n)
    while x == y: y = random.randint(1, n)
    l = random.randint(0, k-2)
    r = random.randint(l+1, k)   # 保证 l < r
    print(x, y, l, r)
