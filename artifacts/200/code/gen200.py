#!/usr/bin/env python3
"""200 课对拍生成器：生成仙人掌（树 + 环替换），按题目格式输出。
首行输出 prob 题号行；课程程序读剥掉首行后的输入，暴力程序读全文。
"""
import random, sys

def gen_cactus(n, t, rng, odd_cycle=False):
    """生成仙人掌：n 个点（1..n），t 个环，每个环长 3..n 内。
    返回 (edges, cycles, is_cactus)。edges = [(u,v),...] 无向；cycles = [[u,p1,...,v],...] 环路径（不含闭合边 v-u）。
    用「树 + 环替换树边」构造，保证仙人掌。odd_cycle=True 时每个环边数（len(cycle)+1）为奇数。
    """
    # 1. 树：点 i 的父亲随机 < i（保证连通），也可允许 1..n 任意树
    par = {}
    for i in range(2, n + 1):
        par[i] = rng.randint(1, i - 1)
    tree_edges = [(i, par[i]) for i in range(2, n + 1)]
    # 2. 随机选 t 条树边做环（每条环用 k 个新点还是复用？环经过的点必须是原图点：
    #    为控制 n，环直接在树边两端之间插入已有但未用的点？简化：环只占 3 个点：u-v 保留 + 插入一个新点 w（w 从备用点里取）。
    #    备用点 = n 以内未被树用到的点不够，改为：环(u,v,k) = u - w1 - ... - w_{k-1} - v，w 从「当前环已用点」外随机取任意点（可复用其它环的点？不行，点可共享但边不能。
    #    简单实现：环的新点从 1..n 中随机取（可以取树上的点，只要边不冲突）。仙人掌允许点属于多个环（环顶/共享点）。
    #    但点 u 已经连了树边，新环边 u-w 不会与树边冲突（不同边）。w 可以任意取。
    usable = list(range(1, n + 1))
    used_edges = set()
    for u, v in tree_edges:
        used_edges.add((min(u, v), max(u, v)))
    cycles = []
    chosen = []
    for (u, v) in rng.sample(tree_edges, min(t, len(tree_edges))):
        # 环 = u-m1-...-mk-v + 闭合边 v-u。所有环边必须不与已用边冲突（否则共享边 = 非仙人掌）。
        made = False
        for _attempt in range(20):
            k = rng.randint(1, 3)          # 中间新点数
            if odd_cycle:
                k = 2 * rng.randint(0, 2)  # k 偶数 → 环长 k+1 奇数
            mids = [rng.randint(1, n) for _ in range(k)]
            path = [u] + mids + [v]
            seq = path + [path[0]]
            cand = [(min(a, b), max(a, b)) for a, b in zip(seq[:-1], seq[1:])]
            if len(set(cand)) == len(cand) and not any(e in used_edges for e in cand):
                cycles.append(path)
                chosen.append((u, v))
                for e in cand:
                    used_edges.add(e)
                made = True
                break
        # 20 次构造不出（n 太小）就跳过该环（树边保留为割边，不孤立点）
    # 3. 输出全部边：树边（未被环选中仍是割边）+ 环边
    edges = []
    for u, v in tree_edges:
        if (u, v) not in chosen and (v, u) not in chosen:
            edges.append((u, v))
    for path in cycles:
        seq = path + [path[0]]
        for a, b in zip(seq[:-1], seq[1:]):
            edges.append((a, b))
    return edges, cycles

def paths_from(edges, cycles):
    """题1/题5 的路径格式：割边成一条路径 (2,u,v)；环 = 长路径 + 闭合边路径。"""
    paths = []
    for u, v in edges:
        paths.append([u, v])
    # 环路径已经含全部环边（长路径 + 闭合边），这里把每个环输出为 1 条长路径 + 1 条闭合边
    # 但 edges 里环边是逐条列出的，会导致重复！需要从 edges 中剔除环的闭合边，用长路径表示。
    # 因此 paths_from 应该基于 cycles 构造而不是 edges。
    raise NotImplementedError

def main():
    prob = int(sys.argv[1])
    seed = int(sys.argv[2])
    rng = random.Random(seed)
    if prob in (1, 5):
        n = rng.randint(4, 8)
        t = rng.randint(1, 3)
        odd = (prob == 5)  # 题5 无奇数要求，直径任意
        edges, cycles = gen_cactus(n, t, rng, odd_cycle=False)
        # 路径格式：环 = 长路径(全部环点) + 闭合边路径(2, v, u)
        paths = []
        covered = set()
        for u, v in edges:
            covered.add((min(u, v), max(u, v)))
        # 输出所有割边（edges 里不在任何环上的）
        cycle_edge_set = set()
        for path in cycles:
            seq = path + [path[0]]
            for a, b in zip(seq[:-1], seq[1:]):
                cycle_edge_set.add((min(a, b), max(a, b)))
        for u, v in edges:
            if (min(u, v), max(u, v)) not in cycle_edge_set:
                paths.append([u, v])
        for path in cycles:
            paths.append(path)                    # 长路径 u...v（覆盖 u-w1..v）
            paths.append([path[-1], path[0]])     # 闭合边 v-u
        m = len(paths)
        print('prob %d' % prob)
        print(n, m)
        for p in paths:
            print(len(p), ' '.join(map(str, p)))
    elif prob in (2, 3, 4):
        n = rng.randint(4, 8)
        t = rng.randint(1, 3)
        edges, cycles = gen_cactus(n, t, rng, odd_cycle=False)
        m = len(edges)
        print('prob %d' % prob)
        if prob == 2:
            print('1')  # 一组数据
            print(n, m)
            for u, v in edges:
                print(u, v)
        elif prob == 3:
            print(n, m)
            for u, v in edges:
                print(u, v)
        else:
            print(n, m)
            for u, v in edges:
                print(u, v)
            if prob == 4:
                # 点权：50% 全 1（第二版），50% 随机
                if rng.random() < 0.5:
                    print(' '.join(['1'] * n))
                else:
                    print(' '.join(str(rng.randint(1, 6)) for _ in range(n)))
    elif prob == 6:
        n = rng.randint(4, 8)
        t = rng.randint(1, 3)
        edges, cycles = gen_cactus(n, t, rng, odd_cycle=False)
        m = len(edges)
        q = rng.randint(1, 5)
        print('prob %d' % prob)
        print(n, m, q)
        for u, v in edges:
            print(u, v, rng.randint(1, 9))
        for _ in range(q):
            a = rng.randint(1, n)
            b = rng.randint(1, n)
            print(a, b)
    elif prob == 7:
        n = rng.randint(4, 8)
        t = rng.randint(1, 3)
        edges, cycles = gen_cactus(n, t, rng, odd_cycle=False)
        m = len(edges)
        q = rng.randint(1, 4)
        print('prob %d' % prob)
        print(n, m)
        for u, v in edges:
            print(u, v, rng.randint(1, 9))
        print(q)
        for _ in range(q):
            k = rng.randint(1, n)
            arr = rng.sample(range(1, n + 1), k)
            print(k, ' '.join(map(str, arr)))
    elif prob == 8:
        n = rng.randint(4, 9)
        t = rng.randint(1, 3)
        edges, cycles = gen_cactus(n, t, rng, odd_cycle=True)
        m = len(edges)
        q = rng.randint(3, 8)
        print('prob %d' % prob)
        print(n, m, q)
        for u, v in edges:
            print(u, v)
        for _ in range(q):
            op = rng.randint(1, 3)
            x = rng.randint(1, n)
            print(op, x)
    else:
        raise SystemExit('bad prob')

main()
