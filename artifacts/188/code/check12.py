# 题2 无向校验器：验证课程输出是合法欧拉路径 + 与暴力字典序最小对比
import sys, subprocess

def run(prog, inp):
    return subprocess.run([prog], input=inp, capture_output=True, text=True).stdout

def valid_path(pts, edges):
    """pts: 点序列；edges: 无向边列表；返回 (是否合法, 说明)"""
    if len(pts) != len(edges) + 1:
        return False, f'点数{len(pts)} ≠ 边数+1({len(edges)+1})'
    cnt = {}
    eset = set()
    for u, v in edges:
        eset.add((u, v)); eset.add((v, u))
    used = {}
    for i in range(len(pts) - 1):
        a, b = pts[i], pts[i+1]
        e = (a, b) if (a, b) in eset else (b, a)
        if e not in eset:
            return False, f'连续点对({a},{b}) 不是图的边'
        key = tuple(sorted((a, b)))
        used[key] = used.get(key, 0) + 1
    if any(v > 1 for v in used.values()):
        return False, f'边被重复使用: {[k for k,v in used.items() if v>1]}'
    if len(used) != len(edges):
        return False, f'只覆盖 {len(used)}/{len(edges)} 条边'
    return True, '合法'

for seed in range(1, 13):
    inp = subprocess.run(['python3', 'gen12.py', str(seed), 'u'], capture_output=True, text=True).stdout
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    edges = [tuple(map(int, l.split())) for l in lines[1:]]
    out = run('./Code02_UndirectedEuler.run', inp)
    if out.strip() == 'No':
        # 无解：暴力也应无解
        b = run('./brute12.run u', inp)
        if b.strip() != 'No':
            print(f'u{seed}: 课程 No 但暴力有解 {b.strip()!r}')
        else:
            print(f'u{seed}: 都 No OK')
        continue
    pts = [int(x) for x in out.strip().split('\n')]
    ok, msg = valid_path(pts, edges)
    if not ok:
        print(f'u{seed}: 课程路径非法 - {msg} | {pts}')
    else:
        b = run('./brute12.run u', inp).strip()
        bpts = [int(x) for x in b.split()]
        if bpts == pts:
            print(f'u{seed}: 合法且与暴力最小一致 OK')
        else:
            # 比较字典序
            s1 = ' '.join(map(str, pts)); s2 = ' '.join(map(str, bpts))
            print(f'u{seed}: 合法但字典序 课程[{s1}] vs 暴力最小[{s2}]')
