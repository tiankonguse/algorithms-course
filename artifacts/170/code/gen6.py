import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(1, 8)
m = random.randint(1, 10)
print(n, m)
for _ in range(n):
    print(random.randint(0, 10), random.randint(0, 10))
adds = n
for _ in range(m):
    if adds < 12 and random.random() < 0.5:
        print(1, random.randint(0, 10), random.randint(0, 10)); adds += 1
    else:
        print(2, random.randint(0, 10), random.randint(0, 10))
