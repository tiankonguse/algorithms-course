#!/usr/bin/env python3
"""题2 暴力：每条查询取矩形内全部数排序取第 k 小"""
import sys
data = list(map(int, sys.stdin.read().split()))
it = iter(data)
n = next(it); q = next(it)
mat = [[0] * (n + 1)]
for _ in range(n):
    mat.append([0] + [next(it) for _ in range(n)])
out = []
for _ in range(q):
    a = next(it); b = next(it); c = next(it); d = next(it); k = next(it)
    vals = [mat[x][y] for x in range(a, c + 1) for y in range(b, d + 1)]
    vals.sort()
    out.append(str(vals[k - 1]))
print('\n'.join(out))
