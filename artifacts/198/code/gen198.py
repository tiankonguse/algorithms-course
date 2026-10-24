import random, sys

def gen5(rng):
    n = rng.randint(1, 10)
    arr = [rng.randint(1, 15) for _ in range(n)]
    m = rng.randint(0, min(8, n * (n - 1) // 2))
    pairs = set()
    while len(pairs) < m:
        x = rng.randint(1, n); y = rng.randint(1, n)
        if x == y: continue
        if x > y: x, y = y, x
        pairs.add((x, y))
    lines = [f'5', f'{n} {m}', ' '.join(map(str, arr))]
    for x, y in sorted(pairs): lines.append(f'{x} {y}')
    return '\n'.join(lines) + '\n'

def gen6(rng):
    v = rng.randint(1, 5)
    arr = []
    for val in range(1, v + 1):
        arr += [val] * 4
    rng.shuffle(arr)
    return f'6\n{v}\n' + ' '.join(map(str, arr)) + '\n'

def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)
    with open(f'/tmp/cpp198/in5.s{seed}', 'w') as f: f.write(gen5(rng))
    with open(f'/tmp/cpp198/in6.s{seed}', 'w') as f: f.write(gen6(rng))

main()
