import random, subprocess, sys, os

def gen1(n, m):
    edges = []
    for v in range(2, n + 1):
        u = random.randint(1, v - 1)
        edges.append((u, v))
    adj = [[] for _ in range(n + 1)]
    for u, v in edges:
        adj[u].append(v); adj[v].append(u)
    fa = [0] * (n + 1); dep = [0] * (n + 1)
    st = [1]; fa[1] = 0
    order = []
    while st:
        u = st.pop(); order.append(u)
        for v in adj[u]:
            if v != fa[u]:
                fa[v] = u; dep[v] = dep[u] + 1; st.append(v)
    ops = []
    want = []
    val = [0] * (n + 1)  # val[u] = edge (u, fa[u])
    def lca(u, v):
        while dep[u] > dep[v]: u = fa[u]
        while dep[v] > dep[u]: v = fa[v]
        while u != v: u = fa[u]; v = fa[v]
        return u
    for _ in range(m):
        if random.random() < 0.6:
            x = random.randint(1, n); y = random.randint(1, n)
            ops.append(('P', x, y))
            l = lca(x, y)
            u = x
            while u != l:
                val[u] += 1; u = fa[u]
            u = y
            while u != l:
                val[u] += 1; u = fa[u]
        else:
            # Q 保证 x、y 直接相连
            a, b = random.choice(edges)
            if random.random() < 0.5: x, y = a, b
            else: x, y = b, a
            ops.append(('Q', x, y))
            w = x if fa[x] == y else y
            want.append(val[w])
    lines = [f'{n} {m}']
    lines += [f'{u} {v}' for u, v in edges]
    lines += [f'{o} {x} {y}' for o, x, y in ops]
    return '\n'.join(lines) + '\n', want

def run(prog, inp):
    r = subprocess.run([prog], input=inp.encode(), capture_output=True, timeout=30)
    return r.stdout.decode().strip().split()

def main():
    prog = '/tmp/cpp162/Code01_GrassPlanting2.bin'
    fails = 0
    for t in range(300):
        n = random.randint(2, 9); m = random.randint(1, 40)
        inp, want = gen1(n, m)
        got = run(prog, inp)
        w = [str(x) for x in want]
        if got != w:
            fails += 1
            print(f'MISMATCH t={t}')
            print(inp)
            print('got', got)
            print('want', w)
            if fails > 3:
                open('/tmp/cpp162/p3038_fail.in', 'w').write(inp)
                return fails
    return fails

if __name__ == '__main__':
    fails = main()
    print(f'RESULT: P3038 fails={fails} / 300')
    sys.exit(1 if fails else 0)
