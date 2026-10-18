import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(2, 7); m = random.randint(10, 25)
print(n, m)
# 维护每条边的存在状态，生成合法操作序列
active = set()
lines = []
for _ in range(m):
    if active and random.random() < 0.4:
        x, y = random.choice(list(active))
        lines.append(f"1 {x} {y}"); active.discard((x, y))
    elif random.random() < 0.6 or not active:
        x = random.randint(1, n); y = random.randint(1, n)
        while x == y: y = random.randint(1, n)
        e = (min(x, y), max(x, y))
        if e in active:
            lines.append(f"2 {x} {y}")
        else:
            lines.append(f"0 {x} {y}"); active.add(e)
    else:
        x = random.randint(1, n); y = random.randint(1, n)
        while x == y: y = random.randint(1, n)
        lines.append(f"2 {x} {y}")
print("\n".join(lines))
