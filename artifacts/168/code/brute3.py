#!/usr/bin/env python3
"""题3 暴力：逐场雨模拟每个区域收到量，国家收集够了即停；不达标输出 NIE"""
import sys
data = list(map(int, sys.stdin.read().split()))
it = iter(data)
n = next(it); m = next(it)
own = [0] + [next(it) for _ in range(m)]
need = [0] + [next(it) for _ in range(n)]
k = next(it)
rains = []
for _ in range(k):
    l = next(it); r = next(it); num = next(it)
    if r < l:
        r += m
    rains.append((l, r, num))
ans = []
for nation in range(1, n + 1):
    got = 0
    res = None
    for t, (l, r, num) in enumerate(rains, 1):
        # 链上位置 p ∈ [l, r] 覆盖到区域 (p-1) % m + 1
        for p in range(l, r + 1):
            if own[(p - 1) % m + 1] == nation:
                got += num
        if got >= need[nation]:
            res = t
            break
    ans.append(str(res if res is not None else 'NIE'))
print('\n'.join(ans))
