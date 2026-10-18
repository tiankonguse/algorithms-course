import random, sys
seed = int(sys.argv[1])
random.seed(seed)
n = random.randint(1, 5)
k = random.randint(1, 8)
q = random.randint(20, 40)
print(n, k)
alive = []
for i in range(1, n + 1):
    v = random.randint(1, 8); w = random.randint(1, 8)
    alive.append(i)
    print(v, w)
print(q)
nxt_id = n + 1
for i in range(q):
    if not alive:
        op = 1
    else:
        op = random.choice([1, 2, 3])
    if op == 1:
        v = random.randint(1, 8); w = random.randint(1, 8)
        alive.append(nxt_id)
        print(1, v, w)
        nxt_id += 1
    elif op == 2:
        x = random.choice(alive)
        alive.remove(x)
        print(2, x)
    else:
        print(3)
