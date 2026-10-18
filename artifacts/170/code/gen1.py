import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(1, 8); k = random.randint(1, 10)
print(n, k)
for _ in range(n):
    print(random.randint(1, k), random.randint(1, k), random.randint(1, k))
