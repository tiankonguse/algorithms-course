import subprocess, sys, random

def run(exe, inp, which):
    r = subprocess.run([exe, str(which)], input=inp, capture_output=True, text=True, timeout=20)
    return r.stdout.strip(), r.stderr

def norm(s):
    return '\n'.join(' '.join(x.split()) for x in s.splitlines() if x.strip())

def gen(seed, which):
    r = subprocess.run(['python3', '/tmp/cpp192/gen192.py', str(seed), str(which)], capture_output=True, text=True)
    return r.stdout

fails = 0
N = 15
for which in range(1, 8):
    for seed in range(1, N + 1):
        inp = gen(seed, which)
        co, ce = run('/tmp/cpp192/Code%.2d.run' % which, inp, which)
        br, be = run('/tmp/cpp192/brute192.run', inp, which)
        if norm(co) != norm(br):
            fails += 1
            fn = '/tmp/cpp192/mis192_%d_%d.txt' % (which, seed)
            open(fn, 'w').write(inp)
            print('MISMATCH 题%d seed%d' % (which, seed))
            print('  课程:', co.replace('\n', ' | '))
            print('  暴力:', br.replace('\n', ' | '))
            if ce: print('  stderr:', ce[:200])
            if be: print('  brute stderr:', be[:200])
print('总计 FAIL:', fails, '/ 105')
