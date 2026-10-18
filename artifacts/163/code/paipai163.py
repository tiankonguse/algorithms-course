import random, subprocess, sys

def gen(n):
    edges = []
    for v in range(2, n + 1):
        u = random.randint(1, v - 1)
        edges.append((u, v))
    adj = [[] for _ in range(n + 1)]
    for u, v in edges:
        adj[u].append(v); adj[v].append(u)
    fa = [0] * (n + 1)
    st = [1]; order = []
    while st:
        u = st.pop(); order.append(u)
        for v in adj[u]:
            if v != fa[u]:
                fa[v] = u; st.append(v)
    arr = [0] + [random.randint(1, n) for _ in range(n)]
    # 暴力：每棵子树集合
    sub = [set() for _ in range(n + 1)]
    for u in range(n, 0, -1):   # order 是前序，按逆序保证子先于父
        pass
    # 直接对每个查询点做子树遍历
    def subtree_colors(x):
        seen = set()
        st = [x]; par = fa[x]
        while st:
            u = st.pop()
            seen.add(arr[u])
            for v in adj[u]:
                if v != par:
                    st.append(v)
        return len(seen)
    queries = [random.randint(1, n) for _ in range(10)]
    want = [subtree_colors(q) for q in queries]
    lines = [str(n)]
    lines += [f'{u} {v}' for u, v in edges]
    lines += [str(x) for x in arr[1:]]
    lines += [str(len(queries))] + [str(q) for q in queries]
    return '\n'.join(lines) + '\n', want

def run(prog, inp):
    r = subprocess.run([prog], input=inp.encode(), capture_output=True, timeout=30)
    return r.stdout.decode().strip().split()

def main():
    prog = '/tmp/cpp163/Code01_DsuOnTree3.bin'
    fails = 0
    for t in range(300):
        n = random.randint(2, 12)
        inp, want = gen(n)
        got = run(prog, inp)
        w = [str(x) for x in want]
        if got != w:
            fails += 1
            print(f'MISMATCH t={t}')
            print(inp)
            print('got', got)
            print('want', w)
            if fails > 3:
                open('/tmp/cpp163/u41492_fail.in', 'w').write(inp)
                return fails
    return fails

if __name__ == '__main__':
    fails = main()
    print(f'RESULT: U41492 fails={fails} / 300')
    sys.exit(1 if fails else 0)
