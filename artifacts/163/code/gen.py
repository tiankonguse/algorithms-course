import random

random.seed(777)


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
        while stack:
            u = stack.pop()
            seen.add(arr[u])
            for v in adj[u]:
                if v != fa[u]:
                    stack.append(v)
        return len(seen)

    queries = [random.randint(1, n) for _ in range(10)]
    want = [subtree_colors(q) for q in queries]
    lines = [str(n)]
    lines += ['%d %d' % (u, v) for u, v in edges]
    lines += [str(x) for x in arr[1:]]
    lines += [str(len(queries))] + [str(q) for q in queries]
    return '\n'.join(lines) + '\n', want


for t in range(100):
    n = random.randint(2, 14)
    inp, want = gen(n)
    open('/tmp/cpp163/case%d.in' % t, 'w').write(inp)
    open('/tmp/cpp163/case%d.want' % t, 'w').write('\n'.join(map(str, want)) + '\n')
print('100 cases generated')
