import random, sys

def mulberry32(seed):
    def rnd():
        nonlocal_vars = {'t': seed + 0x6D2B79F5}
        # use a closure via generator trick instead
        return 0
    return rnd

# 简单线性同余生成器（可复现）
class Rng:
    def __init__(self, seed):
        self.s = seed & 0xFFFFFFFF
    def next(self):
        self.s = (self.s * 1664525 + 1013904223) & 0xFFFFFFFF
        return self.s
    def randint(self, a, b):
        if a > b: return a
        return a + (self.next() % (b - a + 1))
    def random(self):
        return (self.next() & 0xFFFF) / 65536.0
    def choice(self, arr):
        return arr[self.randint(0, len(arr) - 1)]
    def shuffle(self, arr):
        for i in range(len(arr) - 1, 0, -1):
            j = self.randint(0, i)
            arr[i], arr[j] = arr[j], arr[i]
        return arr

def p1(rng):
    # 黑手党：n<=6 m<=7，边 x y c t
    n = rng.randint(2, 6); m = rng.randint(1, 7)
    print(n, m)
    for _ in range(m):
        while True:
            x = rng.randint(1, n); y = rng.randint(1, n)
            if x != y: break
        c = rng.randint(1, 3); t = rng.randint(1, 8)
        print(x, y, c, t)

def p2(rng):
    # 旗帜：n<=6，位置 1..12（保证有冲突可测）
    n = rng.randint(2, 6)
    print(n)
    for _ in range(n):
        a = rng.randint(1, 12); b = rng.randint(1, 12)
        print(a, b)

def p3(rng):
    # 编码：n<=6，每串长度 1..4，最多一个 ?
    n = rng.randint(1, 6)
    print(n)
    for _ in range(n):
        L = rng.randint(1, 4)
        chars = [rng.choice('01') for _ in range(L)]
        if rng.random() < 0.6:
            chars[rng.randint(0, L - 1)] = '?'
        print(''.join(chars))

def p4(rng):
    # 游戏：n<=6, d<=2, m<=8
    n = rng.randint(3, 6)
    d = rng.randint(0, 2)
    gtypes = ['a', 'b', 'c']
    # 选 d 个位置做 x
    positions = list(range(1, n + 1))
    rng.shuffle(positions)
    gt = [''] * (n + 1)
    for i in range(1, n + 1):
        gt[i] = 'a'
    for i in range(d):
        gt[positions[i]] = 'x'
    print(n, d)
    print(''.join(gt[1:]))
    m = rng.randint(1, 8)
    print(m)
    for _ in range(m):
        p1_ = rng.randint(1, n); c1 = rng.choice('ABC')
        p2_ = rng.randint(1, n); c2 = rng.choice('ABC')
        print(p1_, c1, p2_, c2)

def p5(rng):
    # 布尔：n<=5, m<=8, q<=6
    n = rng.randint(2, 5); m = rng.randint(1, 8); q = rng.randint(1, 6)
    print(n, m, q)
    for _ in range(m):
        u = rng.randint(1, n); x = rng.randint(0, 1)
        v = rng.randint(1, n); y = rng.randint(0, 1)
        print(u, x, v, y)
    for _ in range(q):
        l = rng.randint(1, m); r = rng.randint(l, m)
        print(l, r)

if __name__ == '__main__':
    seed = int(sys.argv[1])
    prob = int(sys.argv[2])
    rng = Rng(seed)
    if prob == 1: p1(rng)
    elif prob == 2: p2(rng)
    elif prob == 3: p3(rng)
    elif prob == 4: p4(rng)
    else: p5(rng)
