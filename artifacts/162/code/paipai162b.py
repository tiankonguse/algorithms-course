import random, subprocess, sys
from collections import deque

def gen(n):
    edges = []
    for v in range(2, n + 1):
        u = random.randint(1, v - 1)
        edges.append((u, v))
    adj = [[] for _ in range(n + 1)]
    for u, v in edges:
        adj[u].append(v); adj[v].append(u)
    # all-pairs BFS
    dist = [[0] * (n + 1) for _ in range(n + 1)]
    for s in range(1, n + 1):
        d = dist[s]
        q = deque([s]); seen = {s}
        while q:
            u = q.popleft()
            for v in adj[u]:
                if v not in seen:
                    seen.add(v); d[v] = d[u] + 1; q.append(v)
    ans = 0
    for a in range(1, n + 1):
        for b in range(a + 1, n + 1):
            for c in range(b + 1, n + 1):
                if dist[a][b] == dist[b][c] == dist[c][a]:
                    ans += 1
    lines = [str(n)]
    lines += [f'{u} {v}' for u, v in edges]
    return '\n'.join(lines) + '\n', ans

def run(prog, inp):
    r = subprocess.run([prog], input=inp.encode(), capture_output=True, timeout=30)
    return r.stdout.decode().strip()

def main():
    prog = '/tmp/cpp162/Code07_HotHotels2.bin'
    fails = 0
    for t in range(400):
        n = random.randint(3, 12)
        inp, want = gen(n)
        got = run(prog, inp)
        if got != str(want):
            fails += 1
            print(f'MISMATCH t={t} want={want} got={got}')
            print(inp)
            if fails > 3:
                open('/tmp/cpp162/p5904_fail.in', 'w').write(inp)
                return fails
    return fails

if __name__ == '__main__':
    fails = main()
    print(f'RESULT: P5904 fails={fails} / 400')
    sys.exit(1 if fails else 0)
