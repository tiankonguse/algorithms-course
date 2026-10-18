import sys
it = iter(sys.stdin.read().split())
n = int(next(it)); m = int(next(it))
perm = [int(next(it)) for _ in range(n)]
dels = [int(next(it)) for _ in range(m)]
out = []
for t in range(m):
    inv = sum(1 for i in range(len(perm)) for j in range(i+1, len(perm)) if perm[i] > perm[j])
    out.append(str(inv))
    perm.remove(dels[t])
print('\n'.join(out))
