#!/usr/bin/env python3
"""183 课对拍生成器：7 题（P2634/P3806/P4149/P4178/CF293E/P5351/P6626）× 4 种子。"""
import random, sys

def gen_tree(rng, n, wmin, wmax, zero_based=False):
    """随机树：i 的父在 1..i-1 随机。返回边列表 [(u,v,w)]（1-based 内部），zero_based 时外部编号从 0。"""
    edges = []
    for i in range(2, n + 1):
        p = rng.randint(1, i - 1)
        w = rng.randint(wmin, wmax)
        if zero_based:
            edges.append((i - 1, p - 1, w))
        else:
            edges.append((i, p, w))
    return edges

seed = int(sys.argv[1])
rng = random.Random(seed)

# 题1 P2634 聪聪可可：n ≤ 10，边权 1..10，输出 a/b
def q1():
    n = rng.randint(2, 10)
    lines = [str(n)]
    for u, v, w in gen_tree(rng, n, 1, 10):
        lines.append(f"{u} {v} {w}")
    return '\n'.join(lines) + '\n'

# 题2 P3806 距离为k的点对：n ≤ 10，m ≤ 6，边权 1..10，k 1..30
def q2():
    n = rng.randint(2, 10)
    m = rng.randint(1, 6)
    lines = [f"{n} {m}"]
    for u, v, w in gen_tree(rng, n, 1, 10):
        lines.append(f"{u} {v} {w}")
    ks = sorted(rng.randint(1, 30) for _ in range(m))
    for k in ks:
        lines.append(str(k))
    return '\n'.join(lines) + '\n'

# 题3 P4149 最少边数：n ≤ 12，k 0..40，边权 0..10，编号 0-based
def q3():
    n = rng.randint(2, 12)
    k = rng.randint(0, 40)
    lines = [f"{n} {k}"]
    for u, v, w in gen_tree(rng, n, 0, 10, zero_based=True):
        lines.append(f"{u} {v} {w}")
    return '\n'.join(lines) + '\n'

# 题4 P4178 距离≤k：n ≤ 12，k 0..40，边权 0..10
def q4():
    n = rng.randint(2, 12)
    k = rng.randint(0, 40)
    lines = [str(n)]
    for u, v, w in gen_tree(rng, n, 0, 10):
        lines.append(f"{u} {v} {w}")
    lines.append(str(k))
    return '\n'.join(lines) + '\n'

# 题5 CF293E 相近点对：n ≤ 12，limitl 0..n-1，limitw 0..40，边权 0..10，输入 fa w（2..n）
def q5():
    n = rng.randint(2, 12)
    limitl = rng.randint(0, n - 1)
    limitw = rng.randint(0, 40)
    lines = [f"{n} {limitl} {limitw}"]
    for i in range(2, n + 1):
        fa = rng.randint(1, i - 1)
        w = rng.randint(0, 10)
        lines.append(f"{fa} {w}")
    return '\n'.join(lines) + '\n'

# 题6 P5351 魔力值：n ≤ 12，l r 1..n（l ≤ r），边权 0..10
def q6():
    n = rng.randint(2, 12)
    l = rng.randint(1, n)
    r = rng.randint(l, n)
    lines = [f"{n} {l} {r}"]
    for u, v, w in gen_tree(rng, n, 0, 10):
        lines.append(f"{u} {v} {w}")
    return '\n'.join(lines) + '\n'

# 题7 P6626 消息传递：t=2 组，每组 n ≤ 10，m ≤ 6，边权 1，k 0..n-1
def q7():
    t = 2
    lines = [str(t)]
    for _ in range(t):
        n = rng.randint(2, 10)
        m = rng.randint(1, 6)
        lines.append(f"{n} {m}")
        for i in range(2, n + 1):
            p = rng.randint(1, i - 1)
            lines.append(f"{p} {i}")
        for _ in range(m):
            x = rng.randint(1, n)
            k = rng.randint(0, n - 1)
            lines.append(f"{x} {k}")
    return '\n'.join(lines) + '\n'

qs = {'1': q1, '2': q2, '3': q3, '4': q4, '5': q5, '6': q6, '7': q7}
if __name__ == '__main__':
    q = sys.argv[2]
    sys.stdout.write(qs[q]())
