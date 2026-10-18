import sys
it = iter(sys.stdin.read().split())
n = int(next(it)); m = int(next(it))
pts = []
for _ in range(n):
    x, y = int(next(it)), int(next(it)); pts.append((x, y))
out = []
for _ in range(m):
    a, b, c, d = int(next(it)), int(next(it)), int(next(it)), int(next(it))
    out.append(str(sum(1 for x, y in pts if a <= x <= c and b <= y <= d)))
print('\n'.join(out))
