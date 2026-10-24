import subprocess, sys, os

BASE = '/tmp/cpp197'
def run(prob, seed):
    inp = subprocess.run([sys.executable, f'{BASE}/gen197.py', str(seed), str(prob)],
                         capture_output=True, text=True).stdout
    course = subprocess.run([f'{BASE}/Code0{prob}*.run' if False else f'{BASE}/runs/p{prob}.run', ],
                            capture_output=True, text=True)  # placeholder, replaced below
    return inp, None

# 课程程序名
COURSE = {1: 'Code01_Mafia.run', 2: 'Code02_Flags.run', 3: 'Code03_BinaryCode.run',
          4: 'Code04_Game.run', 5: 'Code05_Boolean.run'}

def run_bin(path, inp):
    return subprocess.run([path], input=inp, capture_output=True, text=True).stdout

def verify_p1(inp, out):
    lines = out.strip().split('\n')
    if lines[0] == 'No': return True, ''
    toks = lines[1].split()
    if len(toks) != 2: return False, 'ansTime/Size 行格式错: %r' % lines[1]
    ansTime, ansSize = int(toks[0]), int(toks[1])
    if ansSize > 0:
        if len(lines) < 3: return False, '有摧毁边但缺边行'
        if ansSize != len(lines[2].split()): return False, 'ansSize=%d 但边数=%d' % (ansSize, len(lines[2].split()))
    # 读输入
    it = iter(inp.strip().split('\n'))
    n, m = map(int, next(it).split())
    X, Y, C, T = [], [], [], []
    for _ in range(m):
        x, y, c, t = map(int, next(it).split())
        X.append(x); Y.append(y); C.append(c); T.append(t)
    dest = [int(v) for v in (lines[2].split() if ansSize > 0 else [])]
    if len(dest) != ansSize: return False, '边数不一致'
    # 摧毁边两两不相邻
    seen = set()
    for eid in dest:
        for v in (X[eid-1], Y[eid-1]):
            if v in seen: return False, '摧毁边在点 %d 相邻' % v
            seen.add(v)
    # 同色幸存边两两不相邻
    use = {}
    surv = [i for i in range(1, m + 1) if i not in dest]
    for eid in surv:
        for v in (X[eid-1], Y[eid-1]):
            key = (v, C[eid-1])
            if key in use: return False, '同色幸存边在点 %d 颜色 %d 相邻' % (v, C[eid-1])
            use[key] = 1
    # ansTime 正确
    mt = max(T[eid-1] for eid in dest) if dest else 0
    if mt != ansTime: return False, 'ansTime=%d 但 max t=%d' % (ansTime, mt)
    return True, ''

def verify_p3(inp, out):
    lines = out.strip().split('\n')
    if lines[0] == 'NO': return True, ''
    if len(lines) != 1 + 1:  # 课程在 YES 后打印 n 串？课程 YES 行 + n 行
        pass
    n = int(inp.strip().split('\n')[0])
    if len(lines) != n + 1: return False, 'YES 后应打印 %d 行，实际 %d 行' % (n, len(lines) - 1)
    ss = lines[1:]
    for i in range(n):
        for j in range(n):
            if i == j: continue
            a, b = ss[i], ss[j]
            if len(a) >= len(b) and a[:len(b)] == b:
                return False, '串 %s 以 %s 为前缀' % (a, b)
    return True, ''

def verify_p4(inp, out):
    o = out.strip()
    if o == '-1': return True, ''
    lines = inp.strip().split('\n')
    n, d = map(int, lines[0].split())
    g = lines[1].strip()
    m = int(lines[2])
    p1, c1, p2, c2 = [], [], [], []
    for i in range(m):
        a, ca, b, cb = lines[3 + i].split()
        p1.append(int(a)); c1.append(ca); p2.append(int(b)); c2.append(cb)
    if len(o) != n: return False, '方案长度 %d != n %d' % (len(o), n)
    for i in range(n):
        ch = o[i]
        if g[i] == 'a' and ch == 'A': return False, 'a 类地点用了 A'
        if g[i] == 'b' and ch == 'B': return False, 'b 类地点用了 B'
        if g[i] == 'c' and ch == 'C': return False, 'c 类地点用了 C'
    for i in range(m):
        if o[p1[i] - 1] == c1[i] and o[p2[i] - 1] != c2[i]:
            return False, '限制 %d 不满足' % (i + 1)
    return True, ''

def main():
    SEEDS = 12
    for prob in range(1, 6):
        ok = 0
        for seed in range(1, SEEDS + 1):
            inp = subprocess.run([sys.executable, f'{BASE}/gen197.py', str(seed), str(prob)],
                                 capture_output=True, text=True).stdout
            co = run_bin(f'{BASE}/{COURSE[prob]}', inp)
            br = run_bin(f'{BASE}/brute197.run {prob}', inp) if False else run_bin(f'{BASE}/brute197.run', inp)
            # brute197.run 需要参数 prob
            co2 = subprocess.run([f'{BASE}/brute197.run', str(prob)], input=inp, capture_output=True, text=True).stdout
            if prob == 1:
                b1 = co2.strip().split('\n')
                c1 = co.strip().split('\n')
                if (c1[0] == 'No') != (b1[0] == 'No'):
                    print(f'题1 seed{seed} 判定不一致: 课程={c1[0]} 暴力={b1[0]}'); continue
                if c1[0] == 'Yes':
                    # 课程 ansTime 是该 limit 下 2-SAT 任意解的实际耗时，应 <= 最小可行 limit
                    if int(c1[1].split()[0]) > int(b1[1]):
                        print(f'题1 seed{seed} ansTime={c1[1].split()[0]} 超过最小可行limit {b1[1]}'); continue
                    v, e = verify_p1(inp, co)
                    if not v:
                        print(f'题1 seed{seed} 方案非法: {e}'); continue
                ok += 1
            elif prob == 2:
                if co.strip() != co2.strip():
                    print(f'题2 seed{seed} 不一致: 课程={co.strip()} 暴力={co2.strip()}'); continue
                ok += 1
            elif prob == 3:
                c3 = co.strip().split('\n')[0]
                b3 = co2.strip().split('\n')[0]
                if c3 != b3:
                    print(f'题3 seed{seed} 判定不一致: 课程={c3} 暴力={b3}'); continue
                v, e = verify_p3(inp, co)
                if not v:
                    print(f'题3 seed{seed} 方案非法: {e}'); continue
                ok += 1
            elif prob == 4:
                c4 = '-1' if co.strip() == '-1' else 'YES'
                b4 = co2.strip().split('\n')[0]
                if (c4 == '-1') != (b4 == '-1'):
                    print(f'题4 seed{seed} 判定不一致: 课程={c4} 暴力={b4}'); continue
                if c4 != '-1':
                    v, e = verify_p4(inp, co)
                    if not v:
                        print(f'题4 seed{seed} 方案非法: {e}'); continue
                ok += 1
            else:
                cl = [l for l in co.strip().split('\n') if l != '']
                bl = [l for l in co2.strip().split('\n') if l != '']
                if cl != bl:
                    print(f'题5 seed{seed} 不一致: 课程={cl} 暴力={bl}'); continue
                ok += 1
        print(f'== 题{prob}: {ok}/{SEEDS} 组通过 ==')
        if ok != SEEDS:
            print(f'  题{prob} 有失败，保存输入', file=sys.stderr)

if __name__ == '__main__':
    main()
