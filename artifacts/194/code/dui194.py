import subprocess, random, sys
fails = 0
total = 0
for which in range(1, 11):
    for seed in range(1, 16):
        inp = subprocess.run(['python3', 'gen194.py', str(seed), str(which)], capture_output=True, text=True).stdout
        a = subprocess.run(['./Code%02d.run' % which, str(which)], input=inp, capture_output=True, text=True).stdout
        b = subprocess.run(['./brute194.run', str(which)], input=inp, capture_output=True, text=True).stdout
        total += 1
        if a != b:
            fails += 1
            print('FAIL 题%d seed%d' % (which, seed))
            print('--- 输入 ---'); print(inp)
            print('--- 课程 ---'); print(a)
            print('--- 暴力 ---'); print(b)
print('总计 FAIL: %d / %d' % (fails, total))
