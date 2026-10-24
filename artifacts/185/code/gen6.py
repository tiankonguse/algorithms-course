import random, sys
random.seed(int(sys.argv[1]))
n = random.randint(2, 10); m = random.randint(1, 10)
print(n, m)
print(' '.join(str(random.randint(1, 5)) for _ in range(n)))
for v in range(2, n+1):
    print(random.randint(1, v-1), v)
for _ in range(m):
    l = random.randint(1, n); r = random.randint(l, n); x = random.randint(l, r)
    print(l, r, x)
