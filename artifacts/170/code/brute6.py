import sys
it = iter(sys.stdin.read().split())
n = int(next(it)); m = int(next(it))
pts = []
for _ in range(n):
    x, y = int(next(it)), int(next(it)); pts.append((x, y))
out = []
for _ in range(m):
    op = int(next(it)); x, y = int(next(it)), int(next(it))
    if op == 1:
        pts.append((x, y))
    else:
        out.append(str(min(abs(x-px)+abs(y-py) for px, py in pts)))
if out:
    sys.stdout.write('\n'.join(out) + '\n')
