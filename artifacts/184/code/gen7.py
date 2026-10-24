import random, sys
random.seed(int(sys.argv[1]))
t = random.randint(1, 3)
print(t)
for _ in range(t):
    n = random.randint(2, 12); m = random.randint(1, n)
    print(n, m)
    print(' '.join(random.choice('ABC') for _ in range(n)))
    for v in range(2, n+1):
        print(random.randint(1, v-1), v)
    print(' '.join(random.choice('ABC') for _ in range(m)))
