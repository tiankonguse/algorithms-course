import sys
it = iter(sys.stdin.read().split())
n = int(next(it)); m = int(next(it))
pts = []
for _ in range(n):
    x, y, v = int(next(it)), int(next(it)), int(next(it)); pts.append((x, y, v))
out = []
for _ in range(m):
    a, b, c, d = int(next(it)), int(next(it)), int(next(it)), int(next(it))
    out.append(str(sum(v for x, y, v in pts if a <= x <= c and b <= y <= d)))
print('\n'.join(out))
