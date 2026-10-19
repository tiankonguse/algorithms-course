import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(1, 10); m = random.randint(2, 15)
print(n, m)
for _ in range(n): print(random.randint(-20, 20), end=' ')
print()
for _ in range(m):
    op = random.randint(1, 2)
    if op == 1:
        l = random.randint(1, n); r = random.randint(l, n)
        print(1, l, r, random.randint(-10, 10))
    else:
        print(2, random.randint(1, n), random.randint(-25, 25))
