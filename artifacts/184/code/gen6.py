import random, sys
random.seed(int(sys.argv[1]))
n = random.randint(2, 12)
print(n)
for v in range(2, n+1):
    u = random.randint(1, v-1)
    print(u, v, random.randint(1, 10))
m = random.randint(1, 10)
print(m)
for _ in range(m):
    x = random.randint(1, n); y = random.randint(x, n)
    print(x, y)
