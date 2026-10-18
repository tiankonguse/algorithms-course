import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(3, 7)
print(n)
for _ in range(n):
    print(random.randint(1, n), random.randint(1, n))
perm = list(range(1, n+1)); random.shuffle(perm)
for i in range(1, n):
    u = perm[i]; v = perm[random.randint(0, i-1)]
    print(u, v)
