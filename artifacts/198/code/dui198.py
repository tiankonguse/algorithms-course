import subprocess, os

COURSE5 = '/tmp/cpp198/Code05_Partition_Multiset_cpp_1.run'
COURSE6 = '/tmp/cpp198/Code06_Meow_II_cpp_1.run'
BRUTE = '/tmp/cpp198/brute198.run'

ok = 0
# 题5：12 种子
for seed in range(1, 13):
    subprocess.run(['python3', '/tmp/cpp198/gen198.py', str(seed)])
    inp5 = open(f'/tmp/cpp198/in5.s{seed}').read()
    inp5c = inp5.split('\n', 1)[1]   # 课程程序无题号行
    c5 = subprocess.run([COURSE5], input=inp5c, capture_output=True, text=True).stdout.strip()
    b5 = subprocess.run([BRUTE], input=inp5, capture_output=True, text=True).stdout.strip()
    if c5 != b5:
        print(f'题5 seed{seed} MISMATCH: 课程={c5} 暴力={b5}')
        open(f'/tmp/cpp198/fail5.s{seed}', 'w').write(inp5)
    else:
        ok += 1
print(f'== 题5: {ok}/12 组通过 ==')

ok6 = 0
for seed in range(1, 13):
    inp6 = open(f'/tmp/cpp198/in6.s{seed}').read()
    inp6c = inp6.split('\n', 1)[1]   # 课程程序无题号行
    c6 = subprocess.run([COURSE6], input=inp6c, capture_output=True, text=True).stdout.strip()
    b6 = subprocess.run([BRUTE], input=inp6, capture_output=True, text=True).stdout.strip()
    lines6 = c6.split('\n')
    yes6 = lines6[0]
    bfirst = b6.split('\n')[0]
    if yes6 != bfirst:
        print(f'题6 seed{seed} 判定 MISMATCH: 课程={yes6} 暴力={bfirst}')
        open(f'/tmp/cpp198/fail6.s{seed}', 'w').write(inp6)
        continue
    if yes6 == 'Yes':
        # 验证课程构造：每条序列 4n/2 个元素且完全相等、每个值各 2 个
        s = lines6[1] if len(lines6) > 1 else ''
        v = int(inp6c.split()[0])
        if len(s) != 4 * v:
            print(f'题6 seed{seed} 构造长度不对: len={len(s)} 期望={4*v}'); continue
        arr = [int(x) for x in inp6c.split()[1:]]
        seq0 = [arr[i] for i in range(len(s)) if s[i] == '0']
        seq1 = [arr[i] for i in range(len(s)) if s[i] == '1']
        if len(seq0) != len(seq1) or seq0 != seq1:
            print(f'题6 seed{seed} 两序列不等'); continue
        # 每个值出现 4 次且各 2 个在 0/1
        from collections import Counter
        c0 = Counter(seq0); c1 = Counter(seq1)
        if any(c0[val] != 2 or c1[val] != 2 for val in range(1, v + 1)):
            print(f'题6 seed{seed} 每个值分布不对'); continue
    ok6 += 1
print(f'== 题6: {ok6}/12 组通过 ==')
