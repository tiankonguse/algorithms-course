import subprocess, sys

RUN = {
    1: './Code01_Legacy.run', 2: './Code02_Journeys.run', 3: './Code03_Road.run',
    4: './Code04_Desert.run', 5: './Code05_Bomb.run', 6: './Code06_BeautifulTreeHLD.run',
    7: './Code07_BeautifulTreeST.run', 8: './Code08_Forest.run', 9: './Code09_Jail.run',
}

def run(cmd, data):
    return subprocess.run(cmd, input=data, capture_output=True, text=True, timeout=120).stdout

def verify4(data, out):
    lines = out.splitlines()
    if not lines or lines[0] != 'TAK':
        return True, ''
    vals = [int(x) for x in lines[1].split()]
    tok = data.split()
    it = iter(tok)
    n, s, m = int(next(it)), int(next(it)), int(next(it))
    fixed = {}
    for _ in range(s):
        x, v = int(next(it)), int(next(it))
        fixed[x] = v
    for x, v in fixed.items():
        if vals[x-1] != v:
            return False, f'定值不符: x={x} 期望{v} 得{vals[x-1]}'
    if any(v < 1 or v > 10**9 for v in vals):
        return False, '值域越界'
    for _ in range(m):
        l, r, k = int(next(it)), int(next(it)), int(next(it))
        xs = [int(next(it)) for _ in range(k)]
        for x in xs:
            for p in range(l, r+1):
                if p not in xs:
                    if not (vals[x-1] > vals[p-1]):
                        return False, f'关系(l={l},r={r}) 特殊{x} 应> {p}: {vals[x-1]} vs {vals[p-1]}'
    return True, ''

def verify67(data, out):
    lines = out.splitlines()
    if lines[0].strip() == '-1':
        return True, ''
    vals = [int(x) for x in lines[0].split()]
    n = vals  # 长度
    NN = len(vals)
    if sorted(vals) != list(range(1, NN+1)):
        return False, '不是排列'
    tok = data.split()
    it = iter(tok)
    n, m = int(next(it)), int(next(it))
    adj = [[] for _ in range(n+1)]
    for _ in range(n-1):
        u, v = int(next(it)), int(next(it))
        adj[u].append(v); adj[v].append(u)
    def path(a, b):
        par = [0]*(n+1); par[a] = -1
        qu = [a]
        for u in qu:
            for v in adj[u]:
                if not par[v]: par[v] = u; qu.append(v)
        p = []; x = b
        while x != -1:
            p.append(x)
            if x == a: break
            x = par[x]
        return p[::-1]
    for _ in range(m):
        op, a, b, c = int(next(it)), int(next(it)), int(next(it)), int(next(it))
        p = path(a, b)
        mn, mx = min(vals[u-1] for u in p), max(vals[u-1] for u in p)
        if op == 1 and mn != vals[c-1]:
            return False, f'op1 路径({a},{b}) 最小应 {vals[c-1]} 得 {mn}'
        if op == 2 and mx != vals[c-1]:
            return False, f'op2 路径({a},{b}) 最大应 {vals[c-1]} 得 {mx}'
    return True, ''

fail = 0
total = 0
for tid in range(1, 10):
    for seed in range(1, 13):
        total += 1
        data = subprocess.run(['python3', 'gen195.py', str(seed), str(tid)], capture_output=True, text=True).stdout
        o1 = run([RUN[tid]], data)
        o2 = run(['./brute195.run', str(tid)], data)
        ok = True
        if tid in (4,):
            same = o1.splitlines()[0] == o2.splitlines()[0]
            v, msg = verify4(data, o1)
            if not (same and v):
                ok = False
                print(f'FAIL 题{tid} seed{seed}: same={same} {msg}')
                print('  课程:', repr(o1[:200]))
                print('  暴力:', repr(o2[:200]))
        elif tid in (6, 7):
            same = (o1.splitlines()[0].strip() == '-1') == (o2.splitlines()[0].strip() == '-1')
            v, msg = verify67(data, o1)
            if not (same and v):
                ok = False
                print(f'FAIL 题{tid} seed{seed}: same={same} {msg}')
                print('  课程:', repr(o1[:200]))
                print('  暴力:', repr(o2[:200]))
        else:
            if o1.strip() != o2.strip():
                ok = False
                print(f'FAIL 题{tid} seed{seed}')
                print('  课程:', repr(o1[:300]))
                print('  暴力:', repr(o2[:300]))
        if not ok:
            fail += 1
            if fail > 8:
                print('失败过多，中止')
                sys.exit(1)
print(f'总计 FAIL: {fail} / {total}')
