import random, sys

def w(name, text):
    open(f'/tmp/cpp179/in_{name}.txt', 'w').write(text)

# 题1 美好的每一天: n m + 字符串 + m组l r
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 12); m = random.randint(1, 12)
    s = ''.join(random.choice('abcdef') for _ in range(n))
    qs = '\n'.join(f"{random.randint(1,n)} {random.randint(1,n)}" for _ in range(m))
    w(f'1_{seed}', f"{n} {m}\n{s}\n{qs}\n")

# 题2 简单的询问: n + arr + q + q组l1 r1 l2 r2
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 12); q = random.randint(1, 12)
    arr = ' '.join(str(random.randint(1, n)) for _ in range(n))
    qs = '\n'.join(f"{random.randint(1,n)} {random.randint(1,n)} {random.randint(1,n)} {random.randint(1,n)}" for _ in range(q))
    w(f'2_{seed}', f"{n}\n{arr}\n{q}\n{qs}\n")

# 题3 区间lcp达标对: n m k + 字符串 + m组l r
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 12); m = random.randint(1, 12); k = random.randint(1, min(4, n))
    s = ''.join(random.choice('fzout sy') for _ in range(n))
    qs = '\n'.join(f"{random.randint(1,n)} {random.randint(1,n)}" for _ in range(m))
    w(f'3_{seed}', f"{n} {m} {k}\n{s}\n{qs}\n")

# 题4 区间最大匹配: n m z + a + b + q + q组l r
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 10); m = random.randint(1, 10); z = random.randint(1, 30)
    a = ' '.join(str(random.randint(1, z)) for _ in range(n))
    b = ' '.join(str(random.randint(1, z)) for _ in range(m))
    q = random.randint(1, 10)
    qs = '\n'.join(f"{random.randint(1,m)} {random.randint(1,m)}" for _ in range(q))
    w(f'4_{seed}', f"{n} {m} {z}\n{a}\n{b}\n{q}\n{qs}\n")

# 题5 作业: n m + arr + m组l r a b
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 12); m = random.randint(1, 12)
    arr = ' '.join(str(random.randint(1, 20)) for _ in range(n))
    qs = '\n'.join(f"{random.randint(1,n)} {random.randint(1,n)} {random.randint(1,20)} {random.randint(1,20)}" for _ in range(m))
    w(f'5_{seed}', f"{n} {m}\n{arr}\n{qs}\n")

# 题6 盼君勿忘: n m + arr + m组l r p
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 10); m = random.randint(1, 10)
    arr = ' '.join(str(random.randint(1, 8)) for _ in range(n))
    qs = '\n'.join(f"{random.randint(1,n)} {random.randint(1,n)} {random.randint(1,20)}" for _ in range(m))
    w(f'6_{seed}', f"{n} {m}\n{arr}\n{qs}\n")

# 题7 掉进兔子洞: n m + arr + 3*m 组区间
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 12); m = random.randint(1, 6)
    arr = ' '.join(str(random.randint(1, 6)) for _ in range(n))
    qs = []
    for _ in range(m):
        for _ in range(3):
            qs.append(f"{random.randint(1,n)} {random.randint(1,n)}")
    w(f'7_{seed}', f"{n} {m}\n{arr}\n{chr(10).join(qs)}\n")

# 题8 由乃的玉米田: n m + arr + m组op l r x
for seed in range(4):
    random.seed(20261020 + seed)
    n = random.randint(1, 12); m = random.randint(1, 12)
    arr = ' '.join(str(random.randint(1, 12)) for _ in range(n))
    qs = '\n'.join(f"{random.randint(1,4)} {random.randint(1,n)} {random.randint(1,n)} {random.randint(1,12)}" for _ in range(m))
    w(f'8_{seed}', f"{n} {m}\n{arr}\n{qs}\n")

print("gen done")
