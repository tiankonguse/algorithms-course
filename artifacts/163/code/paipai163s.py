import random
import subprocess
import sys

random.seed(2026)
PROG = '/tmp/cpp163/Code01_DsuOnTree3.bin'


def gen(n):
    edges = []
    for v in range(2, n + 1):
        u = random.randint(1, v - 1)
        edges.append((u, v))
    adj = [[] for _ in range(n + 1)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)
    fa = [0] * (n + 1)
    st = [1]
    while st:
        u = st.pop()
        for v in adj[u]:
            if v != fa[u]:
                fa[v] = u
                st.append(v)
    arr = [0] + [random.randint(1, n) for _ in range(n)]

    def subtree_colors(x):
        seen = set()
        stack = [x]
        par = fa[x]
        while stack:
            u = stack.pop()
            seen.add(arr[u])
            for v in adj[u]:
                if v != par:
                    stack.append(v)
        return len(seen)

    queries = [random.randint(1, n) for _ in range(10)]
    want = [subtree_colors(q) for q in queries]
    lines = [str(n)]
    lines += ['%d %d' % (u, v) for u, v in edges]
    lines += [str(x) for x in arr[1:]]
    lines += [str(len(queries))] + [str(q) for q in queries]
    return '\n'.join(lines) + '\n', want


fails = 0
for t in range(10):
    n = random.randint(2, 14)
    inp, want = gen(n)
    r = subprocess.run([PROG], input=inp.encode(), capture_output=True, timeout=5)
    got = r.stdout.decode().strip().split()
    w = [str(x) for x in want]
    if got != w:
        fails += 1
        print('MISMATCH t=%d n=%d' % (t, n))
        print(inp)
        print('got', got)
        print('want', w)
print('RESULT: U41492 fails=%d / 10' % fails)
sys.exit(1 if fails else 0)
