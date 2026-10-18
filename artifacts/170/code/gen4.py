import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(1, 10); m = random.randint(1, 8)
print(n, m)
for _ in range(n):
    print(random.randint(0, 10), random.randint(0, 10), random.randint(0, 20))
for _ in range(m):
    a = random.randint(0, 8); b = random.randint(0, 8)
    c = random.randint(a, 10); d = random.randint(b, 10)
    print(a, b, c, d)
