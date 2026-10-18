import random, subprocess, sys
from collections import deque

random.seed(20261018)

def run_prog(binpath, inp):
    p = subprocess.run([binpath], input=inp.encode(), capture_output=True, timeout=10)
    return p.stdout.decode().split()

def path_nodes(adj, a, b):
    par = [-1] * len(adj)
    q = deque([a]); par[a] = 0
    while q:
        u = q.popleft()
        if u == b: break
        for v in adj[u]:
            if par[v] == -1:
                par[v] = u; q.append(v)
    res = []
    u = b
    while u != 0:
        res.append(u); u = par[u]
    return res

def subtree_nodes(adj, x, frm):
    res = [x]
    for v in adj[x]:
        if v != frm:
            res += subtree_nodes(adj, v, x)
    return res

def test_p3384(rounds):
    fails = 0
    for r in range(rounds):
        n = random.randint(2, 8)
        m = random.randint(5, 30)
        MOD = random.randint(2, 50)
        root = random.randint(1, n)
        arr = [0] + [random.randint(0, 30) for _ in range(n)]
        edges = []
        for v in range(2, n + 1):
            u = random.randint(1, v - 1)
            edges.append((u, v))
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v); adj[v].append(u)
        # 以 root 为根求每个点的父，子树定义以 root 为根（课程 dfs1(root,0)）
        par = [-1] * (n + 1)
        q = deque([root]); par[root] = 0
        while q:
            u = q.popleft()
            for v in adj[u]:
                if par[v] == -1:
                    par[v] = u; q.append(v)
        cur = arr[:]
        ops = []
        for _ in range(m):
            op = random.randint(1, 4)
            x = random.randint(1, n); y = random.randint(1, n)
            v = random.randint(1, 10)
            if op == 1:
                ops.append((1, x, y, v))
                for nd in path_nodes(adj, x, y):
                    cur[nd] += v
            elif op == 2:
                exp = sum(cur[nd] for nd in path_nodes(adj, x, y)) % MOD
                ops.append((2, x, y, exp))
            elif op == 3:
                ops.append((3, x, v, None))
                for nd in subtree_nodes(adj, x, par[x]):
                    cur[nd] += v
            else:
                exp = sum(cur[nd] for nd in subtree_nodes(adj, x, par[x])) % MOD
                ops.append((4, x, 0, exp))
        lines = [f"{n} {m} {root} {MOD}", ' '.join(map(str, arr[1:]))]
        for u, v in edges:
            lines.append(f"{u} {v}")
        for o in ops:
            if o[0] == 1:
                lines.append(f"1 {o[1]} {o[2]} {o[3]}")
            elif o[0] == 2:
                lines.append(f"2 {o[1]} {o[2]}")
            elif o[0] == 3:
                lines.append(f"3 {o[1]} {o[2]}")
            else:
                lines.append(f"4 {o[1]}")
        inp = '\n'.join(lines)
        out = run_prog('/tmp/cpp161/Code01_HLD2.bin', inp)
        exp_out = [str(o[3]) for o in ops if o[0] in (2, 4)]
        if out != exp_out:
            fails += 1
            print(f"P3384 MISMATCH r{r}: got {out[:4]} want {exp_out[:4]}")
            open('/tmp/cpp161/p3384_fail.in', 'w').write(inp)
            print('    ops:', [tuple(o[:3]) + (o[3],) for o in ops])
            if fails > 3: return fails
    return fails

def test_p3979(rounds):
    fails = 0
    for r in range(rounds):
        n = random.randint(2, 8)
        m = random.randint(5, 30)
        edges = []
        for v in range(2, n + 1):
            u = random.randint(1, v - 1)
            edges.append((u, v))
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v); adj[v].append(u)
        arr = [0] + [random.randint(1, 100) for _ in range(n)]
        root = random.randint(1, n)
        root0 = root
        cur = arr[:]
        ops = []
        for _ in range(m):
            op = random.randint(1, 3)
            if op == 1:
                root = random.randint(1, n)
                ops.append((1, root, None))
            elif op == 2:
                x = random.randint(1, n); y = random.randint(1, n)
                v = random.randint(1, 100)
                ops.append((2, x, y, v))
                for nd in path_nodes(adj, x, y):
                    cur[nd] = v
            else:
                x = random.randint(1, n)
                par = [-1] * (n + 1)
                q = deque([root]); par[root] = 0
                while q:
                    u = q.popleft()
                    for v in adj[u]:
                        if par[v] == -1:
                            par[v] = u; q.append(v)
                sub = subtree_nodes(adj, x, par[x])
                ops.append((3, x, 0, min(cur[nd] for nd in sub)))
        lines = [f"{n} {m}"]
        for u, v in edges:
            lines.append(f"{u} {v}")
        lines.append(' '.join(map(str, arr[1:])))
        lines.append(str(root0))
        for o in ops:
            if o[0] == 1:
                lines.append(f"1 {o[1]}")
            elif o[0] == 2:
                lines.append(f"2 {o[1]} {o[2]} {o[3]}")
            else:
                lines.append(f"3 {o[1]}")
        inp = '\n'.join(lines)
        out = run_prog('/tmp/cpp161/Code07_FarAway2.bin', inp)
        exp_out = [str(o[3]) for o in ops if o[0] == 3]
        if out != exp_out:
            fails += 1
            print(f"P3979 MISMATCH r{r}: got {out[:4]} want {exp_out[:4]}")
            open('/tmp/cpp161/p3979_fail.in', 'w').write(inp)
            print('    ops:', [(o[0], o[1], o[2] if len(o) > 2 else '', o[3] if len(o) > 3 else '') for o in ops])
            if fails > 3: return fails
    return fails

f1 = test_p3384(200)
f7 = test_p3979(100)
print(f"RESULT: P3384 fails={f1} / 200, P3979 fails={f7} / 100")
sys.exit(1 if (f1 or f7) else 0)
