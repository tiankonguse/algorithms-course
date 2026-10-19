import random, os

def w(name, s):
    open('in/' + name, 'w').write(s)

random.seed(20261019)

# 题1：重要度 max(x*c)，n,m<=1e5 但小数据；arr 小值域让重复出现
for t in range(4):
    n = random.randint(5, 30)
    m = random.randint(3, 15)
    arr = [random.randint(1, 12) for _ in range(n)]
    qs = []
    for _ in range(m):
        l = random.randint(1, n); r = random.randint(l, n)
        qs.append((l, r))
    s = f"{n} {m}\n" + ' '.join(map(str, arr)) + "\n" + '\n'.join(f"{l} {r}" for l, r in qs) + "\n"
    w(f'in1_{t}.txt', s)

# 题2：达到阈值最小众数（0-based l r k）
for t in range(4):
    n = random.randint(5, 30)
    m = random.randint(3, 15)
    arr = [random.randint(1, 10) for _ in range(n)]
    qs = []
    for _ in range(m):
        l = random.randint(0, n-1); r = random.randint(l, n-1)
        k = random.randint(1, 5)
        qs.append((l, r, k))
    s = f"{n} {m}\n" + ' '.join(map(str, arr)) + "\n" + '\n'.join(f"{l} {r} {k}" for l, r, k in qs) + "\n"
    w(f'in2_{t}.txt', s)

# 题3：相同数最远距离
for t in range(4):
    n = random.randint(5, 30)
    m = random.randint(3, 15)
    arr = [random.randint(1, 12) for _ in range(n)]
    qs = []
    for _ in range(m):
        l = random.randint(1, n); r = random.randint(l, n)
        qs.append((l, r))
    s = f"{n}\n" + ' '.join(map(str, arr)) + f"\n{m}\n" + '\n'.join(f"{l} {r}" for l, r in qs) + "\n"
    w(f'in3_{t}.txt', s)

# 题4：累加和为0最长子数组（arr 1/-1）
for t in range(4):
    n = random.randint(5, 30)
    m = random.randint(3, 15)
    arr = [random.choice([1, -1]) for _ in range(n)]
    qs = []
    for _ in range(m):
        l = random.randint(1, n); r = random.randint(l, n)
        qs.append((l, r))
    s = f"{n} {m}\n" + ' '.join(map(str, arr)) + "\n" + '\n'.join(f"{l} {r}" for l, r in qs) + "\n"
    w(f'in4_{t}.txt', s)

# 题5：mex
for t in range(4):
    n = random.randint(5, 30)
    m = random.randint(3, 15)
    arr = [random.randint(0, 15) for _ in range(n)]
    qs = []
    for _ in range(m):
        l = random.randint(1, n); r = random.randint(l, n)
        qs.append((l, r))
    s = f"{n} {m}\n" + ' '.join(map(str, arr)) + "\n" + '\n'.join(f"{l} {r}" for l, r in qs) + "\n"
    w(f'in5_{t}.txt', s)

# 题6：秃子酋长（arr 为 1~n 排列）
for t in range(4):
    n = random.randint(5, 30)
    m = random.randint(3, 15)
    arr = list(range(1, n+1)); random.shuffle(arr)
    qs = []
    for _ in range(m):
        l = random.randint(1, n); r = random.randint(l, n)
        qs.append((l, r))
    s = f"{n} {m}\n" + ' '.join(map(str, arr)) + "\n" + '\n'.join(f"{l} {r}" for l, r in qs) + "\n"
    w(f'in6_{t}.txt', s)

# 题7：树上莫队（n 节点颜色 + n-1 边 + m 查询）
for t in range(4):
    n = random.randint(4, 15)
    m = random.randint(3, 12)
    col = [random.randint(1, 10) for _ in range(n)]
    edges = []
    for i in range(2, n+1):
        edges.append((random.randint(1, i-1), i))
    qs = []
    for _ in range(m):
        u = random.randint(1, n); v = random.randint(1, n)
        qs.append((u, v))
    s = f"{n} {m}\n" + ' '.join(map(str, col)) + "\n" + '\n'.join(f"{u} {v}" for u, v in edges) + "\n" + '\n'.join(f"{u} {v}" for u, v in qs) + "\n"
    w(f'in7_{t}.txt', s)

# 题8：糖果公园（n m q / v / w / 边 / c / 操作）
for t in range(4):
    n = random.randint(4, 12)
    mm = random.randint(2, 8)
    q = random.randint(4, 15)
    v = [random.randint(1, 10) for _ in range(mm)]
    ww = [random.randint(1, 10) for _ in range(n)]
    edges = []
    for i in range(2, n+1):
        edges.append((random.randint(1, i-1), i))
    c = [random.randint(1, mm) for _ in range(n)]
    ops = []
    for _ in range(q):
        op = random.randint(0, 1)
        if op == 0:
            x = random.randint(1, n); y = random.randint(1, mm)
            ops.append((0, x, y))
        else:
            x = random.randint(1, n); y = random.randint(1, n)
            ops.append((1, x, y))
    s = f"{n} {mm} {q}\n" + ' '.join(map(str, v)) + "\n" + ' '.join(map(str, ww)) + "\n"
    s += '\n'.join(f"{u} {vv}" for u, vv in edges) + "\n" + ' '.join(map(str, c)) + "\n"
    s += '\n'.join(f"{op} {x} {y}" for op, x, y in ops) + "\n"
    w(f'in8_{t}.txt', s)

print('32 组生成完成')
