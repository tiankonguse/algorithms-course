import subprocess, sys, random

# 题4/5 规范化：解析输出为块集合列表
def norm_blocks(t):
    lines = t.strip().split('\n')
    if not lines: return ''
    cnt = int(lines[0])
    blocks = []
    idx = 1
    for _ in range(cnt):
        size = int(lines[idx]); idx += 1
        nodes = sorted(int(x) for x in lines[idx].split()); idx += 1
        blocks.append(tuple(nodes))
    blocks.sort()
    return repr(blocks)

def norm5(t):
    lines = t.strip().split('\n')
    if not lines: return ''
    cnt = int(lines[0])
    blocks = []
    for i in range(1, cnt + 1):
        blocks.append(tuple(sorted(int(x) for x in lines[i].split())))
    blocks.sort()
    return repr(blocks)

def norm1(t):
    # 数量 + 编号行（去尾空格）
    out = []
    for ln in t.strip().split('\n'):
        out.append(' '.join(ln.split()))
    return '\n'.join(out)

NORM = {1: norm1, 2: lambda t: t.strip(), 3: lambda t: t.strip(),
        4: norm_blocks, 5: norm5, 6: lambda t: t.strip(), 7: lambda t: t.strip(), 8: lambda t: t.strip()}

fails = 0
total = 0
for which in range(1, 9):
    for seed in range(1, 16):
        total += 1
        inp = subprocess.run(['python3', 'gen193.py', str(seed), str(which)],
                             capture_output=True, text=True).stdout
        if which == 6:
            course = subprocess.run(['./Code06.run'], input=inp, capture_output=True, text=True)
        else:
            course = subprocess.run([f'./Code0{which}.run', str(which)], input=inp, capture_output=True, text=True)
        brute = subprocess.run(['./brute193.run', str(which)], input=inp, capture_output=True, text=True)
        a, b = course.stdout, brute.stdout
        if NORM[which](a) != NORM[which](b):
            fails += 1
            print(f'FAIL 题{which} seed{seed}')
            print('--- 输入 ---')
            print(inp)
            print('--- 课程 ---'); print(a)
            print('--- 暴力 ---'); print(b)
print(f'总计 FAIL: {fails} / {total}')
