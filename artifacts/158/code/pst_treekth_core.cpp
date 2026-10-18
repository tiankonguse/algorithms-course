int insert(int jobi, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    siz[rt] = siz[i] + 1;
    if (l < r) {
        int mid = (l + r) / 2;
        if (jobi <= mid) {
            ls[rt] = insert(jobi, l, mid, ls[rt]);
        } else {
            rs[rt] = insert(jobi, mid + 1, r, rs[rt]);
        }
    }
    return rt;
}

int query(int jobk, int l, int r, int u, int v, int lca, int lcafa) {
    if (l == r) {
        return l;
    }
    int lsiz = siz[ls[u]] + siz[ls[v]] - siz[ls[lca]] - siz[ls[lcafa]];
    int mid = (l + r) / 2;
    if (lsiz >= jobk) {
        return query(jobk, l, mid, ls[u], ls[v], ls[lca], ls[lcafa]);
    } else {
        return query(jobk - lsiz, mid + 1, r, rs[u], rs[v], rs[lca], rs[lcafa]);
    }
}

void dfs(int u, int f) {
    root[u] = insert(kth(arr[u]), 1, s, root[f]);
    deep[u] = deep[f] + 1;
    stjump[u][0] = f;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
        if (to[ei] != f) {
            dfs(to[ei], u);
        }
    }
}

int lca(int a, int b) {
    if (deep[a] < deep[b]) {
        swap(a, b);
    }
    for (int p = MAXP - 1; p >= 0; p--) {
        if (deep[stjump[a][p]] >= deep[b]) {
            a = stjump[a][p];
        }
    }
    if (a == b) {
        return a;
    }
    for (int p = MAXP - 1; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {
            a = stjump[a][p];
            b = stjump[b][p];
        }
    }
    return stjump[a][0];
}

int kth(int u, int v, int k) {
    int lcaNode = lca(u, v);
    int i = query(k, 1, s, root[u], root[v], root[lcaNode], root[stjump[lcaNode][0]]);
    return sorted[i];
}
