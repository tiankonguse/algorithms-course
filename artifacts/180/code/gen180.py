#!/usr/bin/env python3
"""180 课 6 题对拍数据生成器：in_{1..6}_{0..3}.txt（每种子一棵小树 + 各题查询）。"""
import random, os

OUT = '/tmp/cpp180'
os.makedirs(OUT, exist_ok=True)


def gen_tree(rng, n, weighted):
    """返回 (parent, edge_weight)，parent[i] 是 i 的父亲（1 号根），i>=2。"""
    par = [0] * (n + 1)
    w = [0] * (n + 1)
    for i in range(2, n + 1):
        par[i] = rng.randint(1, i - 1)
        w[i] = rng.randint(1, 9) if weighted else 1
    return par, w


def emit(f, lines):
    with open(f, 'w') as fp:
        fp.write('\n'.join(lines) + '\n')


def main():
    for t in range(4):
        rng = random.Random(18000 + t * 137)
        n = rng.randint(2, 12)

        # 题1：n 点树 + q 查询，每条 k 个重要点（不含 1 也允许）
        par, w = gen_tree(rng, n, False)
        q = rng.randint(1, 5)
        L = [f'{n}']
        for i in range(2, n + 1):
            L.append(f'{i} {par[i]}')
        L.append(f'{q}')
        for _ in range(q):
            k = rng.randint(1, max(1, n // 2))
            pts = rng.sample(range(1, n + 1), k)
            L.append(f'{k} ' + ' '.join(map(str, pts)))
        emit(f'{OUT}/in_1_{t}.txt', L)

        # 题2：同树无向 + q 查询 k 个点
        par, w = gen_tree(rng, n, False)
        q = rng.randint(1, 5)
        L = [f'{n}']
        for i in range(2, n + 1):
            L.append(f'{i} {par[i]}')
        L.append(f'{q}')
        for _ in range(q):
            k = rng.randint(2, min(n, 6))
            pts = rng.sample(range(1, n + 1), k)
            L.append(f'{k} ' + ' '.join(map(str, pts)))
        emit(f'{OUT}/in_2_{t}.txt', L)

        # 题3：带权树 + q 查询 k 个关键点（不含 1）
        par, w = gen_tree(rng, n, True)
        q = rng.randint(1, 5)
        L = [f'{n}']
        for i in range(2, n + 1):
            L.append(f'{i} {par[i]} {w[i]}')
        L.append(f'{q}')
        for _ in range(q):
            k = rng.randint(1, max(1, n - 1))
            pts = rng.sample(range(2, n + 1), k)
            L.append(f'{k} ' + ' '.join(map(str, pts)))
        emit(f'{OUT}/in_3_{t}.txt', L)

        # 题4：n 点树 + q 查询 k 个管理点
        par, w = gen_tree(rng, n, False)
        q = rng.randint(1, 5)
        L = [f'{n}']
        for i in range(2, n + 1):
            L.append(f'{i} {par[i]}')
        L.append(f'{q}')
        for _ in range(q):
            k = rng.randint(1, min(n, 4))
            pts = rng.sample(range(1, n + 1), k)
            L.append(f'{k} ' + ' '.join(map(str, pts)))
        emit(f'{OUT}/in_4_{t}.txt', L)

        # 题5：n 点树 + q 查询：k 病毒(start,speed) + m 关键城市
        par, w = gen_tree(rng, n, False)
        q = rng.randint(1, 4)
        L = [f'{n}']
        for i in range(2, n + 1):
            L.append(f'{i} {par[i]}')
        L.append(f'{q}')
        for _ in range(q):
            k = rng.randint(1, 3)
            m = rng.randint(1, min(n, 4))
            starts = [rng.randint(1, n) for _ in range(k)]
            speeds = [rng.randint(1, 4) for _ in range(k)]
            L.append(f'{k} {m}')
            for s_idx in range(k):
                L.append(f'{starts[s_idx]} {speeds[s_idx]}')
            cities = rng.sample(range(1, n + 1), m)
            L.append(' '.join(map(str, cities)))
        emit(f'{OUT}/in_5_{t}.txt', L)

        # 题6：带权树 + m 条操作（翻转）
        par, w = gen_tree(rng, n, True)
        m = rng.randint(2, 8)
        L = [f'{n} {m}']
        for i in range(2, n + 1):
            L.append(f'{i} {par[i]} {w[i]}')
        ops = [rng.randint(1, n) for _ in range(m)]
        L.extend(map(str, ops))
        emit(f'{OUT}/in_6_{t}.txt', L)

    print('gen 180 done: 6 x 4 = 24 files')


if __name__ == '__main__':
    main()
