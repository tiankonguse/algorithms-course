#!/usr/bin/env python3
"""181 课 5 题随机用例生成器：题1 P3605 / 题2 P3224 / 题3 P4556 / 题4 P1600 / 题5 P3521。
n 取小值（≤12），5 题 × 固定种子 0..3，输出 in_{i}_{seed}.txt。"""
import random, sys

def w(t):  # 写入文件
    open('/tmp/cpp181/in_%d_%d.txt' % (t, seed), 'w').write(txt)

seed = int(sys.argv[1]) if len(sys.argv) > 1 else 0
rnd = random.Random(seed * 999 + 7)

# 题1 P3605 晋升者计数：n, arr, fa[2..n]
n = rnd.randint(3, 12)
arr = [rnd.randint(1, 30) for _ in range(n + 1)]
txt = '%d\n' % n
txt += ' '.join(str(arr[i]) for i in range(1, n + 1)) + '\n'
txt += ' '.join(str(rnd.randint(1, i - 1)) for i in range(2, n + 1)) + '\n'
w(1)

# 题2 P3224 永无乡：n m, num[1..n] 排列, m 座桥, q, 操作
n = rnd.randint(3, 10)
m = rnd.randint(0, n - 1)
num = list(range(1, n + 1)); rnd.shuffle(num)
txt = '%d %d\n' % (n, m)
txt += ' '.join(map(str, num)) + '\n'
pairs = set()
for _ in range(m):
    while True:
        a = rnd.randint(1, n); b = rnd.randint(1, n)
        if a != b and (min(a, b), max(a, b)) not in pairs:
            pairs.add((min(a, b), max(a, b))); break
    txt += '%d %d\n' % (a, b)
q = rnd.randint(1, 15)
txt += '%d\n' % q
for _ in range(q):
    if rnd.random() < 0.5:
        txt += 'B %d %d\n' % (rnd.randint(1, n), rnd.randint(1, n))
    else:
        txt += 'Q %d %d\n' % (rnd.randint(1, n), rnd.randint(1, n))
w(2)

# 题3 P4556 雨天的尾巴：n m, n-1 边, m 行 x y v
n = rnd.randint(3, 12)
txt = '%d %d\n' % (n, m3 := rnd.randint(1, 8))
for i in range(2, n + 1):
    txt += '%d %d\n' % (rnd.randint(1, i - 1), i)
for _ in range(m3):
    txt += '%d %d %d\n' % (rnd.randint(1, n), rnd.randint(1, n), rnd.randint(1, 6))
w(3)

# 题4 P1600 天天爱跑步：n m, n-1 边, w[1..n], m 行 x y
n = rnd.randint(3, 12)
txt = '%d %d\n' % (n, m4 := rnd.randint(1, 8))
for i in range(2, n + 1):
    txt += '%d %d\n' % (rnd.randint(1, i - 1), i)
txt += ' '.join(str(rnd.randint(0, n - 1)) for _ in range(n)) + '\n'
for _ in range(m4):
    txt += '%d %d\n' % (rnd.randint(1, n), rnd.randint(1, n))
w(4)

# 题5 P3521 最小化逆序对：n 叶（1..8），先序建树序列
nleaf = rnd.randint(2, 8)
def gen_structure(nl):
    if nl == 1:
        return ['L']
    k = rnd.randint(1, nl - 1)
    return ['0'] + gen_structure(k) + gen_structure(nl - k)
st = gen_structure(nleaf)
vals = list(range(1, nleaf + 1)); rnd.shuffle(vals)
vi = [0]
def fill(s):
    out = []
    for c in s:
        if c == '0':
            out.append('0')
        else:
            out.append(str(vals[vi[0]])); vi[0] += 1
    return out
seq = fill(st)
txt = '%d\n' % nleaf
txt += ' '.join(seq) + '\n'
w(5)
print('in_%d_%d.txt ok' % (1, seed))
