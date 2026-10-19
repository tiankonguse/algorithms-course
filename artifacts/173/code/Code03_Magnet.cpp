#include <bits/stdc++.h>

using namespace std;

struct Node {
    int x, y, m, p;
    long long range;
    long long dist;
};

bool cmp1(Node &a, Node &b) {
    return a.m < b.m;
}

bool cmp2(Node &a, Node &b) {
    return a.dist < b.dist;
}

const int MAXN = 300001;
const int MAXB = 601;
int n;
Node arr[MAXN];

int blen, bnum;
int bi[MAXN];
int bl[MAXB];
int br[MAXB];
int maxm[MAXB];

int que[MAXN];
bool vis[MAXN];

int bfs() {
    int ans = 0;
    vis[0] = true;
    int l = 1, r = 1;
    que[r++] = 0;
    while (l < r) {
        int cur = que[l++];
        for (int b = 1; b <= bnum; b++) {
            if (maxm[b] <= arr[cur].p) {
                while (bl[b] <= br[b] && arr[bl[b]].dist <= arr[cur].range) {
                    int i = bl[b];
                    if (!vis[i]) {
                        vis[i] = true;
                        que[r++] = i;
                        ans++;
                    }
                    bl[b]++;
                }
            } else {
                for (int i = bl[b]; i <= br[b]; i++) {
                    if (arr[i].dist <= arr[cur].range && arr[i].m <= arr[cur].p && !vis[i]) {
                        vis[i] = true;
                        que[r++] = i;
                        ans++;
                    }
                }
                break;
            }
        }
    }
    return ans;
}

void prepare() {
    blen = (int)sqrt(n);
    bnum = (n + blen - 1) / blen;
    for (int i = 1; i <= n; i++) {
        bi[i] = (i - 1) / blen + 1;
    }
    for (int i = 1; i <= bnum; i++) {
        bl[i] = (i - 1) * blen + 1;
        br[i] = min(i * blen, n);
    }
    sort(arr + 1, arr + n + 1, cmp1);
    for (int i = 1; i <= bnum; i++) {
        maxm[i] = arr[br[i]].m;
        sort(arr + bl[i], arr + br[i] + 1, cmp2);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int x, y, m, p, range;
    cin >> x >> y >> p >> range >> n;
    arr[0] = {x, y, 0, p, range, 0};
    for (int i = 1; i <= n; i++) {
        cin >> x >> y >> m >> p >> range;
        arr[i] = {x, y, m, p, range, 0};
    }
    long long xd, yd;
    for (int i = 0; i <= n; i++) {
        arr[i].range = arr[i].range * arr[i].range;
        xd = arr[0].x - arr[i].x;
        yd = arr[0].y - arr[i].y;
        arr[i].dist = xd * xd + yd * yd;
    }
    prepare();
    cout << bfs() << '\n';
    return 0;
}
