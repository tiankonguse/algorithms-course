import sys
it = iter(sys.stdin.read().split())
n = int(next(it)); k = int(next(it))
arr = []
for i in range(n):
    a, b, c = int(next(it)), int(next(it)), int(next(it))
    arr.append((a, b, c, i))
f = [0]*n
for i in range(n):
    for j in range(n):
        if i != j and arr[j][0] <= arr[i][0] and arr[j][1] <= arr[i][1] and arr[j][2] <= arr[i][2]:
            f[i] += 1
ans = [0]*n
for v in f: ans[v] += 1
print('\n'.join(map(str, ans)))
