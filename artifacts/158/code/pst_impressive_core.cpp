int add(int jobi, long long jobv, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    sum[rt] = sum[i] + jobv;
    if (l < r) {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = add(jobi, jobv, l, mid, ls[rt]);
        } else {
            rs[rt] = add(jobi, jobv, mid + 1, r, rs[rt]);
        }
    }
    return rt;
}

long long query(int jobl, int jobr, int l, int r, int u, int v) {
void dfs2(int u, int f) {
    root[dfn[u]] = add(dep[u], (long long)siz[u] - 1, 1, depth, root[dfn[u] - 1]);
    for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
        if (to[ei] != f) {
            dfs2(to[ei], u);
        }
    }
}

void prepare() {
    depth = 0;
    dfs1(1, 0);
    root[0] = build(1, depth);
    dfs2(1, 0);
}

long long compute(int a, int k) {
    long long ans = (long long)(siz[a] - 1) * min(k, dep[a] - 1);
    ans += query(dep[a] + 1, dep[a] + k, 1, depth, root[dfn[a] - 1], root[dfn[a] + siz[a] - 1]);
    return ans;
}
