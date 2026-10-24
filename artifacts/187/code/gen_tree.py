# 公共：随机树生成
import random, sys

def gen_tree(r, n, wmin, wmax, extra=None):
    """返回 (边列表, 邻接表, 父数组)"""
    edges, adj = [], [[] for _ in range(n + 1)]
    for i in range(2, n + 1):
        p = r.randint(1, i - 1)
        adj[i].append(p); adj[p].append(i)
        if extra is None:
            edges.append((p, i))
        else:
            edges.append((p, i, extra(r)))
    return edges, adj

def dfs_dep(adj, root):
    n = len(adj) - 1
    dep = [0] * (n + 1)
    par = [0] * (n + 1)
    st = [(root, 0)]
    while st:
        u, fa = st.pop()
        for v in adj[u]:
            if v != fa:
                par[v] = u
                dep[v] = dep[u] + 1
                st.append((v, u))
    return dep, par
