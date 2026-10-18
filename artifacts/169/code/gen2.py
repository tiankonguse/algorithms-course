import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 8); m = random.randint(4, 12)
print(n, m)
arr = [random.randint(1, 20) for _ in range(n)]
print(*arr)
for _ in range(m):
    if random.random() < 0.45:
        a = random.randint(1, n); b = random.randint(1, 20)
        print('C', a, b)
    else:
        x = random.randint(1, n); y = random.randint(x, n)
        v = random.randint(1, y - x + 1)
        print('Q', x, y, v)
