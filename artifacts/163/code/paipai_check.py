import subprocess
import sys

PROG = '/tmp/cpp163/Code01_DsuOnTree3.bin'
fails = 0
for t in range(100):
    inp = open('/tmp/cpp163/case%d.in' % t).read()
    want = open('/tmp/cpp163/case%d.want' % t).read().strip().split()
    r = subprocess.run([PROG], input=inp.encode(), capture_output=True, timeout=5)
    got = r.stdout.decode().strip().split()
    if got != want:
        fails += 1
        print('MISMATCH t=%d' % t)
        print(' in:', inp.replace('\n', ' | '))
        print(' got:', got)
        print(' want:', want)
        if fails > 3:
            sys.exit(fails)
print('RESULT: U41492 fails=%d / 100' % fails)
sys.exit(1 if fails else 0)
