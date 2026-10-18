import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 30)
print(n)
for _ in range(n):
    print(random.randint(1, 100), random.randint(1, 100))
