# 题3：生成真实操作序列（不加密），加密由驱动脚本逐条做
import random, sys
sys.path.insert(0, '/tmp/cpp187')
from gen_tree import gen_tree
r = random.Random(int(sys.argv[1]))
n = r.randint(2, 12)
q = r.randint(5, 25)
arr = list(range(1, n + 1))
r.shuffle(arr)
edges, adj = gen_tree(r, n, 1, 10, extra=lambda r: r.randint(1, 10))
ops = []
for _ in range(q):
    if r.random() < 0.6:
        x = r.randint(1, n)
        y = r.randint(x, n)
        z = r.randint(1, n)
        ops.append((1, x, y, z))
    else:
        x = r.randint(1, n - 1)
        ops.append((2, x))
with open(f'/tmp/cpp187/q03_real_{sys.argv[1]}.txt', 'w') as f:
    for op in ops:
        f.write(' '.join(map(str, op)) + '\n')
# 固定部分写到临时（n/q/arr/边由驱动每次重写完整输入）
print(n, q)
print(' '.join(map(str, arr)))
for u, v, w in edges:
    print(u, v, w)
