import random, sys
random.seed(int(sys.argv[1]))
n = random.randint(2, 12); m = random.randint(1, 10)
print(n, m)
for v in range(2, n+1):
    u = random.randint(1, v-1)
    print(u, v, random.randint(1, 10))
for _ in range(m):
    print(random.randint(1, n), random.randint(1, n))
