#!/usr/bin/env python3
# 191 课对拍生成器：python3 gen191.py <seed> <题号>
# 无向图允许重边与自环（191 题面明确保证重边/自环下答案正确）
import random, sys

seed, which = int(sys.argv[1]), int(sys.argv[2])
rng = random.Random(seed)

def gen_edges(n, connected, allow_self=True, allow_multi=True):
    """返回无向边列表（1-based）。默认允许自环/重边；题4 禁两者（题面无此保证）"""
    edges = []
    if connected and n >= 2:
        # 树保证连通
        perm = list(range(2, n + 1))
        rng.shuffle(perm)
        for v in perm:
            p = rng.randint(1, v - 1)
            edges.append((p, v))
    # 附加随机边
    if allow_self and allow_multi:
        maxextra = n * n
        extra = rng.randint(0, maxextra)
        for _ in range(extra):
            u = rng.randint(1, n)
            v = rng.randint(1, n)
            edges.append((u, v))
    else:
        # 无自环无重边：从候选对里抽
        cand = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1)]
        rng.shuffle(cand)
        extra = rng.randint(0, len(cand))
        edges += cand[:extra]
    rng.shuffle(edges)
    return edges

def out_edges(edges, zero_based=False):
    lines = []
    for u, v in edges:
        if zero_based:
            lines.append(f"{u-1} {v-1}")
        else:
            lines.append(f"{u} {v}")
    return lines

n = rng.randint(2, 8)

if which == 1:  # 割边模版题1：多连通区，1-based，输出割边数量+序号
    edges = gen_edges(n, connected=False)
    lines = [f"{n} {len(edges)}"] + out_edges(edges)
    sys.stdout.write("\n".join(lines) + "\n")

elif which == 2:  # 割边模版题2：连通，0-based
    edges = gen_edges(n, connected=True)
    lines = [f"{n} {len(edges)}"] + out_edges(edges, zero_based=True)
    sys.stdout.write("\n".join(lines) + "\n")

elif which == 3:  # 关键网络线路：连通，n m k l + k A + l B + 边
    edges = gen_edges(n, connected=True)
    k = rng.randint(1, n)
    l = rng.randint(1, n)
    A = sorted(rng.sample(range(1, n + 1), k))
    B = sorted(rng.sample(range(1, n + 1), l))
    lines = [f"{n} {len(edges)} {k} {l}"]
    lines += [str(x) for x in A]
    lines += [str(x) for x in B]
    lines += out_edges(edges)
    sys.stdout.write("\n".join(lines) + "\n")

elif which == 4:  # 贝尔敦道路：连通
    edges = gen_edges(n, connected=True, allow_self=False, allow_multi=False)
    lines = [f"{n} {len(edges)}"] + out_edges(edges)
    sys.stdout.write("\n".join(lines) + "\n")

elif which == 5:  # 边双模版题：多连通区
    edges = gen_edges(n, connected=False)
    lines = [f"{n} {len(edges)}"] + out_edges(edges)
    sys.stdout.write("\n".join(lines) + "\n")

elif which == 6:  # 网络：连通，多组（1 组 + 0 0），q 次加边
    edges = gen_edges(n, connected=True)
    q = rng.randint(1, 8)
    lines = [f"{n} {len(edges)}"] + out_edges(edges)
    lines.append(str(q))
    for _ in range(q):
        x = rng.randint(1, n)
        y = rng.randint(1, n)
        lines.append(f"{x} {y}")
    lines.append("0 0")
    sys.stdout.write("\n".join(lines) + "\n")

elif which == 7:  # 冗余路径：连通
    edges = gen_edges(n, connected=True)
    lines = [f"{n} {len(edges)}"] + out_edges(edges)
    sys.stdout.write("\n".join(lines) + "\n")
