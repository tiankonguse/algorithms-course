import random, sys
sys.path.insert(0, '/tmp/cpp187')
from gen_tree import gen_tree
r = random.Random(int(sys.argv[1]))
n = r.randint(2, 10)
edges, adj = gen_tree(r, n, 1, 1000000006, extra=lambda r: r.randint(1, 1000000006))
print(n)
for u, v, w in edges:
    c = r.randint(0, 1)
    print(u, v, w, c)
