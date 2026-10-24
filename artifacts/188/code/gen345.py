# 题3 生成器：构造必有解的随机链数对；题4/5 共用
import random, sys
seed = int(sys.argv[1])
r = random.Random(seed)
kind = sys.argv[2] if len(sys.argv) > 2 else '3'
if kind == '3':
    npairs = r.randint(2, 6)
    nodes = list(range(1, npairs * 2 + 1))
    r.shuffle(nodes)
    perm = nodes[:npairs + 1]
    pairs = []
    for i in range(npairs):
        pairs.append((perm[i], perm[i+1]))
    r.shuffle(pairs)
    print(npairs)
    for a, b in pairs:
        print(a, b)
elif kind == '4':
    n = r.randint(2, 6)
    words = []
    for i in range(n):
        L = r.randint(1, 3)
        w = ''.join(r.choice('abcde') for _ in range(L))
        words.append(w)
    print(n)
    for w in words:
        print(w)
elif kind == '5':
    n = r.randint(2, 7)
    chars = list('ABCDEabcde')
    r.shuffle(chars)
    cs = chars[:max(2, min(6, n + 1))]
    seen = set()
    pairs = []
    while len(pairs) < n:
        a = r.choice(cs); b = r.choice(cs)
        if a == b: continue
        key = tuple(sorted((a, b)))
        if key in seen: continue
        seen.add(key)
        pairs.append((a, b))
    print(n)
    for a, b in pairs:
        print(a, b)
