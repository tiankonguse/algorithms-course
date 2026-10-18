import random, sys
seed = int(sys.argv[1])
random.seed(seed)
q = random.randint(20, 60)
print(q)
s = []  # 当前集合
for i in range(q):
    if len(s) < 2:
        op = 1
    else:
        op = random.choice([1, 2, 3])
    if op == 1:
        x = random.randint(0, 63)
        s.append(x)
        print('1', x)
    elif op == 2:
        x = random.choice(s)
        s.remove(x)
        print('2', x)
    else:
        print('3')
