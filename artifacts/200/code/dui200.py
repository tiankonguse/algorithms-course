#!/usr/bin/env python3
"""200 课对拍驱动：python3 dui200.py <prob> <seeds...>"""
import subprocess, sys, os

CP = '/tmp/cpp200'
COURSE = {1:'Code01_CactusDeleteEdge2.run', 2:'Code02_CactusAddEdge2.run', 3:'Code03_CactusLongestRoute2.run',
          4:'Code04_CactusIndependentSet2.run', 5:'Code05_CactusDiameter2.run', 6:'Code06_CactusShortestPaths2.run',
          7:'Code07_MaximumOfShortestPath2.run', 8:'Code08_CactusPathFlip2.run'}
BRUTE = {i: 'brute%d.run' % i for i in range(1, 9)}

def run(exe, data):
    p = subprocess.run([os.path.join(CP, exe)], input=data, capture_output=True, text=True, timeout=60)
    if p.returncode != 0:
        return 'EXIT' + str(p.returncode)
    return p.stdout.strip()

def main():
    prob = int(sys.argv[1])
    seeds = [int(x) for x in sys.argv[2:]]
    bad = 0
    for seed in seeds:
        gen = subprocess.run([sys.executable, os.path.join(CP, 'gen200.py'), str(prob), str(seed)],
                             capture_output=True, text=True, timeout=60)
        data = gen.stdout
        if gen.returncode != 0:
            print('seed %d gen FAIL: %s' % (seed, gen.stderr.strip()[:120])); bad += 1; continue
        course_data = data.split('\n', 1)[1] + '\n' if '\n' in data else ''
        c = run(COURSE[prob], course_data)
        b = run(BRUTE[prob], data)
        if c != b:
            bad += 1
            print('seed %d MISMATCH  course=%s  brute=%s' % (seed, c[:60], b[:60]))
            open('/tmp/cpp200/fail%d_%d.in' % (prob, seed), 'w').write(data)
        else:
            print('seed %d OK  ans=%s' % (seed, c[:40].replace('\n', '/')))
    print('prob %d: %d/%d 过, %d 组 MISMATCH' % (prob, len(seeds) - bad, len(seeds), bad))

main()
