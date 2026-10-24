import random, sys
sys.path.insert(0, '/tmp/cpp187')
from gen_tree import gen_tree
r = random.Random(int(sys.argv[1]))
n = r.randint(2, 10)
e1, a1 = gen_tree(r, n, -5, 10, extra=lambda r: r.randint(-5, 10))
e2, a2 = gen_tree(r, n, -5, 10, extra=lambda r: r.randint(-5, 10))
print(n)
for u, v, w in e1: print(u, v, w)
for u, v, w in e2: print(u, v, w)
