import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 12); m = random.randint(1, 15)
print(n, m)
for i in range(2, n+1): print(random.randint(1, i-1), end=' ')
print()
for _ in range(m):
    op = random.randint(1, 2)
    if op == 1:
        x = random.randint(1, n); y = random.randint(x, n)
        print(1, x, y, random.randint(1, 5))
    else:
        print(2, random.randint(1, n), random.randint(1, n))
