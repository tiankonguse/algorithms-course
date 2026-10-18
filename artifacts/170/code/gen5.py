import random, sys
seed = int(sys.argv[1]); random.seed(seed)
w = random.randint(2, 8)
ops = []
cnt = random.randint(1, 8)
adds = 0
for _ in range(cnt):
    if adds < 6 and random.random() < 0.6:
        x, y = random.randint(0, w-1), random.randint(0, w-1)
        v = random.randint(0, 10)
        ops.append((1, x, y, v)); adds += 1
    else:
        a = random.randint(0, w-1); b = random.randint(0, w-1)
        c = random.randint(a, w-1); d = random.randint(b, w-1)
        ops.append((2, a, b, c, d))
cnt = len(ops)
print(w, cnt)
for op in ops:
    if op[0] == 1:
        print(1, op[1], op[2], op[3])
    else:
        print(2, op[1], op[2], op[3], op[4])
print(3)
