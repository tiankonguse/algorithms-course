# 题3 交互式对拍：q 改为当前条数，逐条加密 + 重跑两程序 + 比较输出
import subprocess

mask = (1 << 30) - 1
for seed in [1, 2, 3, 4, 5]:
    out = subprocess.run(['python3', 'gen03.py', str(seed)], capture_output=True, text=True)
    fixed = out.stdout.rstrip('\n')
    n, q = map(int, fixed.split('\n')[0].split())
    arr_line = fixed.split('\n')[1]
    edges = fixed.split('\n')[2:]
    ops = []
    with open(f'q03_real_{seed}.txt') as f:
        for line in f:
            ops.append(tuple(map(int, line.split())))
    la = 0
    enc_all = []
    ok, msg = True, ''
    for idx, op in enumerate(ops):
        lines = [f'{n} {idx + 1}', arr_line] + edges
        for j in range(idx):
            lines.append(' '.join(map(str, enc_all[j])))
        enc = [op[0]]
        for v in op[1:]:
            enc.append(v ^ la)
        lines.append(' '.join(map(str, enc)))
        inp = '\n'.join(lines) + '\n'
        r1 = subprocess.run(['./Code03_PersistentEdgeDecompositionTree.run'], input=inp, capture_output=True, text=True)
        r2 = subprocess.run(['./brute03.run'], input=inp, capture_output=True, text=True)
        o1 = r1.stdout.strip().split('\n')
        o2 = r2.stdout.strip().split('\n')
        if o1 != o2:
            ok, msg = False, f'操作{idx+1} {op}: 课程={o1} 暴力={o2}'
            break
        if op[0] == 1:
            la = int(o1[-1]) & mask
        enc_all.append(enc)
    print(f'题3 seed={seed}: {"全过" if ok else "MISMATCH " + msg}')
