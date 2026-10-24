# 题6 保险箱 / 题7 密码串 / 题8 太鼓达人：性质校验器
import subprocess, itertools

def run(p, inp):
    return subprocess.run([p], input=inp, capture_output=True, text=True).stdout

def windows(s, L, cyclic):
    """所有长度 L 的窗口"""
    n = len(s)
    if not cyclic:
        return [s[i:i+L] for i in range(n - L + 1)]
    return [s[i:i+L] if i+L <= n else s[i:] + s[:i+L-n] for i in range(n)]

def check6(n, k):
    s = run('./Code06.run', f'{n} {k}\n').strip()
    L = k**n + n - 1
    if len(s) != L:
        return False, f'长度 {len(s)} != {L}'
    ws = set(windows(s, n, False))
    want = {''.join(t) for t in itertools.product('0123456789'[:k], repeat=n)}
    if ws != want:
        return False, f'窗口缺 {want - ws}'
    return True, 'OK'

def check7(n):
    inp = f'{n}\n0\n'
    s = run('./Code07_Password.run', inp).strip()
    L = 10**n + n - 1
    if len(s) != L:
        return False, f'长度 {len(s)} != {L}'
    ws = set(windows(s, n, False))
    want = {''.join(t) for t in itertools.product('0123456789', repeat=n)}
    if ws != want:
        return False, f'窗口缺 {len(want - ws)} 个'
    return True, 'OK'

def check8(n):
    inp = f'{n}\n0\n'
    out = run('./Code08_Taiko.run', inp).strip()
    parts = out.split(' ')
    if parts[0] != str(2**n):
        return False, f'长度声明 {parts[0]} != {2**n}'
    s = parts[1]
    L = 2**n
    if len(s) != L:
        return False, f'串长 {len(s)} != {L}'
    if not s.startswith('0' * n):
        return False, '前缀非 n 个 0'
    ws = set(windows(s, n, True))
    want = {''.join(t) for t in itertools.product('01', repeat=n)}
    if ws != want:
        return False, f'循环窗口缺 {len(want - ws)} 个'
    return True, 'OK'

print('--- 题6 保险箱 ---')
for n in range(1, 5):
    for k in [2, 3]:
        if k**n > 5000: continue
        ok, msg = check6(n, k)
        print(f'  n={n} k={k}: {"PASS" if ok else "FAIL " + msg}')
print('--- 题7 密码串 ---')
for n in range(1, 5):
    ok, msg = check7(n)
    print(f'  n={n}: {"PASS" if ok else "FAIL " + msg}')
print('--- 题8 太鼓达人 ---')
for n in range(1, 13):
    ok, msg = check8(n)
    print(f'  n={n}: {"PASS" if ok else "FAIL " + msg}')
