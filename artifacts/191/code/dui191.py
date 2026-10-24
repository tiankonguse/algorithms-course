import subprocess, sys
from collections import Counter

def run(argv, inp):
    p = subprocess.run(argv, input=inp, capture_output=True, text=True, timeout=10)
    return p.stdout

def norm(s):
    return [l for l in s.strip().split('\n') if l.strip()]

def check4(inp, cout):
    """题4 验证器：无桥→输出 m 行合法强连通定向；有桥→输出 0"""
    lines = inp.strip().split('\n')
    n, m = map(int, lines[0].split())
    edges = []
    for l in lines[1:1+m]:
        u, v = map(int, l.split())
        edges.append((u, v))
    # 邻接表（含边 id）
    adj = [[] for _ in range(n+1)]
    for i, (u, v) in enumerate(edges):
        adj[u].append((v, i)); adj[v].append((u, i))
    def conn_without(eid):
        s, t = edges[eid]
        if s == t: return True
        vis = [0]*(n+1); vis[s] = 1; q = [s]
        for u in q:
            for v, i in adj[u]:
                if i == eid: continue
                if not vis[v]: vis[v] = 1; q.append(v)
        return vis[t] == 1
    bridges = [i for i in range(m) if not conn_without(i)]
    cout_lines = norm(cout)
    if bridges:
        return cout_lines == ['0']
    # 无桥：验证 m 行定向
    if len(cout_lines) != m: return False
    used = [False]*m
    directed = [[] for _ in range(n+1)]
    for line in cout_lines:
        u, v = map(int, line.split())
        matched = -1
        for i in range(m):
            if used[i]: continue
            x, y = edges[i]
            if (u == x and v == y) or (u == y and v == x):
                matched = i; break
        if matched < 0: return False
        used[matched] = True
        directed[u].append(v)
    # 强连通
    for s in range(1, n+1):
        vis = [0]*(n+1); vis[s] = 1; q = [s]
        for u in q:
            for v in directed[u]:
                if not vis[v]: vis[v] = 1; q.append(v)
        if sum(vis) != n: return False
    return True

def compare(which, inp, cout, bout):
    if which == 4:
        return check4(inp, cout)
    if which == 3:
        cl = norm(cout); bl = norm(bout)
        if not cl or not bl: return cl == bl
        cntc, cntb = int(cl[0]), int(bl[0])
        if cntc != cntb: return False
        setc = Counter(tuple(map(int, l.split())) for l in cl[1:])
        setb = Counter(tuple(map(int, l.split())) for l in bl[1:])
        return setc == setb
    if which == 5:
        cl = norm(cout); bl = norm(bout)
        # cl[0]=分量数；cl[1:] 每分量一行 siz + 成员
        def parse(lines):
            if not lines: return None
            total = int(lines[0])
            comps = []
            i = 1
            for _ in range(total):
                if i >= len(lines): return None
                siz = int(lines[i]); i += 1
                members = []
                while len(members) < siz:
                    if i >= len(lines): return None
                    members += list(map(int, lines[i].split()))
                    i += 1
                comps.append((siz, frozenset(members[:siz])))
            return (total, Counter(comps))
        a, b = parse(cl), parse(bl)
        return a is not None and a == b
    # 题1/2/6/7：字符串比较（规范化）
    return norm(cout) == norm(bout)

bad = {}
for which in range(1, 8):
    for sd in range(1, 16):
        inp = subprocess.run(['python3','gen191.py', str(sd), str(which)], capture_output=True, text=True, timeout=10).stdout
        cout = run([f'./Code{which:02d}.run'], inp)
        bout = run(['./brute191.run', str(which)], inp)
        if not compare(which, inp, cout, bout):
            bad.setdefault(which, []).append((sd, cout, bout))
            open(f'/tmp/mis191_{which}_{sd}.txt','w').write(inp)
res = []
for which in range(1, 8):
    n = len(bad.get(which, []))
    res.append(f'题{which}: {"PASS" if n==0 else "FAIL"} ({15-n}/15)')
    for sd, c, b in bad.get(which, [])[:2]:
        res.append(f'  seed{sd}: 课程[{c.strip()[:120]}] 暴力[{b.strip()[:120]}]')
res.append('总判定: ' + ('PASS' if not bad else 'FAIL'))
open('/tmp/dui191.out','w').write('\n'.join(res))
print('done')
