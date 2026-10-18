import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 6); m = random.randint(3, 8)
print(n, m)
for _ in range(n):
    d = random.randint(1, 10); p = random.randint(1, 10); l = random.randint(1, 5)
    print(d, p, l)
for _ in range(m):
    money = random.randint(1, 50); least = random.randint(1, 20)
    print(money, least)
