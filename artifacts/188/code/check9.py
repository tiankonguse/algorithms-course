# 题9 校验器：方案合法性
import subprocess, sys
def run(p, inp):
    return subprocess.run([p], input=inp, capture_output=True, text=True).stdout
def check(inp):
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    edges = []
    edge_id = {}
    for i in range(1, m + 1):
        u, v, s, t = map(int, lines[i].split())
        key = tuple(sorted((u, v)))
        edges.append((key, s, t))
        edge_id[key] = edge_id.get(key, 0) + 1
    out = run('./Code09_GarbageTruck.run', inp).strip()
    if out == 'NIE':
        return True, 'NIE'
    ol = out.strip().split('\n')
    k = int(ol[0])
    idx = 1
    used = {}
    for _ in range(k):
        if idx >= len(ol): return False, '车行不够'
        nums = list(map(int, ol[idx].split())); idx += 1
        if len(nums) < 1: return False, '边数缺失'
        c = nums[0]
        pts = nums[1:]
        if len(pts) != c + 1: return False, f'点数 {len(pts)} != 边数 {c}+1'
        if pts[0] != pts[-1]: return False, '起点 != 终点'
        if len(set(pts[:-1])) != c: return False, '中途重复经过点'
        for j in range(c):
            key = tuple(sorted((pts[j], pts[j+1])))
            used[key] = used.get(key, 0) + 1
    # 每条边经过次数奇偶性 = s^t（s=t 可 0 次）
    for key, s0, t0 in edges:
        if used.get(key, 0) % 2 != (s0 ^ t0):
            return False, f'边 {key} 使用 {used.get(key, 0)} 次, 需要 {(s0 ^ t0)}'
    # 翻转后状态达成
    st = {}
    for key, s0, t0 in edges:
        st[key] = s0
    for _ in range(k):
        pass
    # 重放所有车的所有边
    ol2 = out.strip().split('\n')
    idx = 1
    for _ in range(int(ol2[0])):
        nums = list(map(int, ol2[idx].split())); idx += 1
        c = nums[0]
        pts = nums[1:]
        for j in range(c):
            key = tuple(sorted((pts[j], pts[j+1])))
            st[key] ^= 1
    for key, s0, t0 in edges:
        if st[key] != t0:
            return False, f'边 {key} 终态 {st[key]} != {t0}'
    return True, f'OK k={k}'
allok = True
for sd in range(1, 16):
    inp = subprocess.run(['python3', 'gen9.py', str(sd)], capture_output=True, text=True).stdout
    ok, msg = check(inp)
    print(f'题9 seed{sd}: {"PASS " + msg if ok else "FAIL " + msg}')
    allok &= ok
print('题9', '全部 PASS' if allok else '有 FAIL')
