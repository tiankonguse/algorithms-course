import random
import subprocess
import sys

random.seed(16307)
PROG = '/tmp/cpp163/Code07_Forest2.bin'


class Forest:
    """暴力：邻接表 + 每次 L 后整树重算 fa/dep；Q 收集路径权值排序取第 k 小。"""

    def __init__(self, n, edges, arr):
        self.n = n
        self.arr = [0] + arr[:]
        self.adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            self.adj[u].append(v)
            self.adj[v].append(u)
        self.rebuild()

    def rebuild(self):
        n = self.n
        self.fa = [0] * (n + 1)
        self.dep = [0] * (n + 1)
        for s in range(1, n + 1):
            if self.fa[s] != 0:
                continue
            self.fa[s] = -1  # 树根标记（用 -1 表示根，跳祖先时停）
            st = [s]
            self.dep[s] = 1
            while st:
                u = st.pop()
                for v in self.adj[u]:
                    if self.fa[v] == 0:
                        self.fa[v] = u
                        self.dep[v] = self.dep[u] + 1
                        st.append(v)
        # 根标记统一为 0：fa[root] = 0
        for u in range(1, n + 1):
            if self.fa[u] == -1:
                self.fa[u] = 0

    def lca(self, a, b):
        while self.dep[a] > self.dep[b]:
            a = self.fa[a]
        while self.dep[b] > self.dep[a]:
            b = self.fa[b]
        while a != b:
            a = self.fa[a]
            b = self.fa[b]
        return a

    def collect_path(self, x, y):
        l = self.lca(x, y)
        vals = []
        u = x
        while True:
            vals.append(self.arr[u])
            if u == l:
                break
            u = self.fa[u]
        u = y
        while u != l:
            vals.append(self.arr[u])
            u = self.fa[u]
        return vals

    def path_kth(self, x, y, k):
        vals = self.collect_path(x, y)
        vals.sort()
        return vals[k - 1]

    def connect(self, x, y):
        self.adj[x].append(y)
        self.adj[y].append(x)
        self.rebuild()

    def root_of(self, u):
        while self.fa[u] != 0:
            u = self.fa[u]
        return u

    def same_tree(self, x, y):
        return self.root_of(x) == self.root_of(y)


def gen_case():
    n = random.randint(5, 20)
    # 初始森林：并查集防环
    par = list(range(n + 1))

    def find(a):
        while par[a] != a:
            par[a] = par[par[a]]
            a = par[a]
        return a

    m = random.randint(0, max(0, n // 2))
    edges = []
    used = set()
    while len(edges) < m:
        u = random.randint(1, n)
        v = random.randint(1, n)
        if u == v or (min(u, v), max(u, v)) in used:
            continue
        if find(u) == find(v):
            continue
        used.add((min(u, v), max(u, v)))
        par[find(u)] = find(v)
        edges.append((u, v))
    arr = [random.randint(1, 100) for _ in range(n)]

    # 操作序列（真值）
    ops = []
    t = random.randint(10, 30)
    f = Forest(n, edges, arr)
    for _ in range(t):
        if random.random() < 0.7:
            # Q：保证 x、y 在同一棵树（原题约束）
            trees = {}
            for a in range(1, n + 1):
                trees.setdefault(f.root_of(a), []).append(a)
            comp = random.choice(list(trees.values()))
            x = random.choice(comp)
            y = random.choice(comp)
            plen = len(f.collect_path(x, y))
            k = random.randint(1, plen)
            ops.append(('Q', x, y, k))
        else:
            # L：选不同树的两点
            cand = []
            for a in range(1, n + 1):
                for b in range(a + 1, n + 1):
                    if not f.same_tree(a, b):
                        cand.append((a, b))
            if cand:
                x, y = random.choice(cand)
                ops.append(('L', x, y))
                f.connect(x, y)
            else:
                x = random.randint(1, n)
                y = random.randint(1, n)
                if not f.same_tree(x, y):
                    ops.append(('L', x, y))
                    f.connect(x, y)
                else:
                    plen = len(f.collect_path(x, y))
                    k = random.randint(1, plen)
                    ops.append(('Q', x, y, k))
    return n, m, edges, arr, ops


def run_prog(n, m, edges, arr, ops):
    """交互编码：按 lastAns 逐条 xor 喂给课程程序；lastAns 与暴力真值同步。"""
    f = Forest(n, edges, arr)
    lines = ['1 %d %d %d' % (n, m, len(ops))]
    lines += [str(x) for x in arr]
    lines += ['%d %d' % (u, v) for u, v in edges]
    last = 0
    want = []
    for op in ops:
        if op[0] == 'Q':
            _, x, y, k = op
            lines.append('Q %d %d %d' % (x ^ last, y ^ last, k ^ last))
            ans = f.path_kth(x, y, k)
            want.append(ans)
            last = ans
        else:
            _, x, y = op
            lines.append('L %d %d' % (x ^ last, y ^ last))
            f.connect(x, y)
    inp = '\n'.join(lines) + '\n'
    r = subprocess.run([PROG], input=inp.encode(), capture_output=True, timeout=5)
    got = [int(v) for v in r.stdout.decode().split()]
    return got, want


def brute(n, edges, arr, ops):
    """暴力：按真值操作模拟，返回 Q 的真值答案列表。"""
    f = Forest(n, edges, arr)
    out = []
    for op in ops:
        if op[0] == 'Q':
            _, x, y, k = op
            out.append(f.path_kth(x, y, k))
        else:
            _, x, y = op
            f.connect(x, y)
    return out


def main():
    fails = 0
    for t in range(100):
        n, m, edges, arr, ops = gen_case()
        got, want = run_prog(n, m, edges, arr, ops)
        if got != want:
            fails += 1
            print('MISMATCH t=%d' % t)
            print(' n=%d m=%d ops=%d' % (n, m, len(ops)))
            print(' edges:', edges)
            print(' arr:', arr)
            print(' ops:', ops)
            print(' got:', got)
            print(' want:', want)
            if fails > 3:
                return fails
    return fails


if __name__ == '__main__':
    fails = main()
    print('RESULT: P3302 fails=%d / 100' % fails)
    sys.exit(1 if fails else 0)
