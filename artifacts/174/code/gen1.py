import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(1, 60); m = random.randint(1, 60)
print(n, m)
print(' '.join(str(random.randint(1, 30)) for _ in range(n)))
for _ in range(m):
    l = random.randint(1, n); r = random.randint(l, n)
    if random.randint(1, 2) == 1:
        print(1, l, r, random.randint(1, 30), random.randint(1, 30))
    else:
        print(2, l, r, random.randint(1, r - l + 1))
