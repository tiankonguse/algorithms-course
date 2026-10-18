import sys
it = iter(sys.stdin.read().split())
w = int(next(it))
k = int(next(it))
grid = {}
out = []
for _ in range(k):
    op = int(next(it))
    if op == 1:
        x, y, v = int(next(it)), int(next(it)), int(next(it))
        grid[(x, y)] = grid.get((x, y), 0) + v
    else:
        a, b, c, d = int(next(it)), int(next(it)), int(next(it)), int(next(it))
        s = 0
        for (x, y), v in grid.items():
            if a <= x <= c and b <= y <= d: s += v
        out.append(str(s))
if out:
    sys.stdout.write('\n'.join(out) + '\n')
