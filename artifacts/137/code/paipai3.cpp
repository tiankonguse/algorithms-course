// 题3 幸运数字：课程（树上倍增 + 带深度替换线性基） vs 暴力（路径点集线性基最大异或）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int MAXN = 20002, LIMIT = 16, BIT3 = 60;

// ---- 课程（照 Code03_LuckyNumber2 迭代版）----
ll arr3[MAXN];
int head3[MAXN], next3[MAXN << 1], to3[MAXN << 1], cnt3;
int deep3[MAXN], stjump3[MAXN][LIMIT], power3;
ll bases3[MAXN][BIT3 + 1];
int levels3[MAXN][BIT3 + 1];
int ufe[MAXN][3], stackSize, u3, f3, e3;

void addEdge3(int u, int v) {
  next3[cnt3] = head3[u]; to3[cnt3] = v; head3[u] = cnt3++;
}
int log2(int n) { int ans = 0; while ((1 << ans) <= (n >> 1)) ans++; return ans; }
void push3(int u, int f, int e) { ufe[stackSize][0] = u; ufe[stackSize][1] = f; ufe[stackSize][2] = e; stackSize++; }
void pop3() { --stackSize; u3 = ufe[stackSize][0]; f3 = ufe[stackSize][1]; e3 = ufe[stackSize][2]; }
bool insertReplace3(ll curv, int curl, ll* basis, int* level) {
  for (int i = BIT3; i >= 0; i--) {
    if (curv >> i == 1) {
      if (basis[i] == 0) { basis[i] = curv; level[i] = curl; return true; }
      if (curl > level[i]) {
        ll tmp1 = curv; curv = basis[i]; basis[i] = tmp1;
        int tmp2 = level[i]; level[i] = curl; curl = tmp2;
      }
      curv ^= basis[i];
    }
  }
  return false;
}
void dfs3(int root, int n) {
  stackSize = 0;
  push3(root, 0, -1);
  while (stackSize > 0) {
    pop3();
    if (e3 == -1) {
      deep3[u3] = deep3[f3] + 1;
      stjump3[u3][0] = f3;
      for (int p = 1; p <= power3; p++) stjump3[u3][p] = stjump3[stjump3[u3][p - 1]][p - 1];
      for (int i = 0; i <= BIT3; i++) { bases3[u3][i] = bases3[f3][i]; levels3[u3][i] = levels3[f3][i]; }
      insertReplace3(arr3[u3], deep3[u3], bases3[u3], levels3[u3]);
      e3 = head3[u3];
    } else {
      e3 = next3[e3];
    }
    if (e3 != 0) {
      push3(u3, f3, e3);
      if (to3[e3] != f3) push3(to3[e3], u3, -1);
    }
  }
}
int lca3(int x, int y) {
  if (deep3[x] < deep3[y]) { int t = x; x = y; y = t; }
  for (int p = power3; p >= 0; p--) if (deep3[stjump3[x][p]] >= deep3[y]) x = stjump3[x][p];
  if (x == y) return x;
  for (int p = power3; p >= 0; p--) if (stjump3[x][p] != stjump3[y][p]) { x = stjump3[x][p]; y = stjump3[y][p]; }
  return stjump3[x][0];
}
ll basis3[BIT3 + 1];
ll query3(int x, int y) {
  int lc = lca3(x, y);
  ll* basisx = bases3[x]; int* levelx = levels3[x];
  ll* basisy = bases3[y]; int* levely = levels3[y];
  memset(basis3, 0, sizeof(basis3));
  for (int i = BIT3; i >= 0; i--) { ll num = basisx[i]; if (levelx[i] >= deep3[lc] && num != 0) basis3[i] = num; }
  for (int i = BIT3; i >= 0; i--) {
    ll num = basisy[i];
    if (levely[i] >= deep3[lc] && num != 0) {
      for (int j = i; j >= 0; j--) {
        if (num >> j == 1) {
          if (basis3[j] == 0) { basis3[j] = num; break; }
          num ^= basis3[j];
        }
      }
    }
  }
  ll ans = 0;
  for (int i = BIT3; i >= 0; i--) ans = max(ans, ans ^ basis3[i]);
  return ans;
}

// ---- 暴力：路径点集线性基 ----
vector<vector<int>> g;   // 1-based
vector<int> par, dep;
void dfsBuild(int u, int fa) {
  par[u] = fa;
  for (int v : g[u]) if (v != fa) { dep[v] = dep[u] + 1; dfsBuild(v, u); }
}
ll bruteMax(const vector<ll>& nums) {
  ll basis[BIT3 + 1]; memset(basis, 0, sizeof(basis));
  for (ll v : nums) {
    for (int i = BIT3; i >= 0; i--) {
      if (v >> i == 1) {
        if (basis[i] == 0) { basis[i] = v; break; }
        v ^= basis[i];
      }
    }
  }
  ll ans = 0;
  for (int i = BIT3; i >= 0; i--) ans = max(ans, ans ^ basis[i]);
  return ans;
}
ll brutePath(int x, int y, const vector<ll>& arr) {
  vector<ll> nums;
  vector<int> px, py;
  int a = x, b = y;
  while (dep[a] > dep[b]) { px.push_back(a); a = par[a]; }
  while (dep[b] > dep[a]) { py.push_back(b); b = par[b]; }
  while (a != b) { px.push_back(a); py.push_back(b); a = par[a]; b = par[b]; }
  px.push_back(a);
  for (int v : px) nums.push_back(arr[v]);
  for (int v : py) nums.push_back(arr[v]);
  return bruteMax(nums);
}

int main() {
  mt19937 rng(555123);
  int fails = 0;
  for (int T = 0; T < 3000; T++) {
    int n = 2 + rng() % 10;
    // 随机树
    g.assign(n + 1, {});
    par.assign(n + 1, 0); dep.assign(n + 1, 0);
    vector<ll> arr(n + 1);
    cnt3 = 1;
    memset(head3, 0, sizeof(head3));
    power3 = log2(n);
    for (int i = 1; i <= n; i++) arr[i] = rng() % 4096;   // 12 位小值域
    for (int i = 2; i <= n; i++) {
      int p = 1 + rng() % (i - 1);
      g[p].push_back(i); g[i].push_back(p);
      addEdge3(p, i); addEdge3(i, p);
    }
    for (int i = 1; i <= n; i++) arr3[i] = arr[i];
    dfsBuild(1, 0);
    dfs3(1, n);
    int q = 5 + rng() % 10;
    for (int qi = 0; qi < q; qi++) {
      int x = 1 + rng() % n, y = 1 + rng() % n;
      ll c = query3(x, y);
      ll b = brutePath(x, y, arr);
      if (c != b) {
        fails++;
        printf("MISMATCH T=%d q=%d x=%d y=%d 课程=%lld 暴力=%lld\n", T, qi, x, y, c, b);
        if (fails > 3) return 1;
      }
    }
  }
  if (fails) return 1;
  printf("3000 组 ALL OK\n");
  return 0;
}
