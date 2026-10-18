#!/usr/bin/env python3
"""题1 暴力：每条查询对 [l..r] 排序取第 k 小"""
import sys
data = list(map(int, sys.stdin.read().split()))
it = iter(data)
n = next(it); m = next(it)
arr = [0] + [next(it) for _ in range(n)]
out = []
for _ in range(m):
    l = next(it); r = next(it); k = next(it)
    seg = sorted(arr[l:r + 1])
    out.append(str(seg[k - 1]))
print('\n'.join(out))
