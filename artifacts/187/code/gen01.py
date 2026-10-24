import random, sys
sys.path.insert(0, '/tmp/cpp187')
from gen_tree import gen_tree
r = random.Random(int(sys.argv[1]))
n = r.randint(2, 12)
arr = [r.randint(1, 20) for _ in range(n + 1)]
edges, adj = gen_tree(r, n, 1, 1)
print(n)
print(' '.join(str(x) for x in arr[1:]))
for e in edges:
    print(e[0], e[1])
