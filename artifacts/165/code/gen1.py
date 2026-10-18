import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 8); m = random.randint(3, 15)
print(n, m)
for _ in range(m):
    op = random.randint(1, 3)
    if op == 1:
        print(1, random.randint(1, n), random.randint(1, n))
    elif op == 2:
        print(2, random.randint(0, _))  # 拷贝 0..当前版本号
    else:
        print(3, random.randint(1, n), random.randint(1, n))
