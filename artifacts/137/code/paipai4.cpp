// 题4 路径最大异或和：课程（DFS树 + 环线性基 + query(path[n])） vs 暴力（独立DFS树 + 枚举环组合）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int MAXN = 50001, MAXM = 200002, BIT4 = 60;

// ---- 课程（照 Code04_MaximumXorOfPath2 迭代版）----
int head4[MAXN], next4[MAXM], to4[MAXM], cnt4;
ll weight4[MAXM];
ll basis4[BIT4 + 1];
bool visited4[MAXN];
ll path4[MAXN];
int ufe4[MAXN][3], stackSize4, u4, f4, e4;
ll pstack4[MAXN], p4;
bool insert4(ll num) {
  for (int i = BIT4; i >= 0; i--)
    if (num >> i == 1) {
      if (basis4[i] == 0) { basis4[i] = num; return true; }
      num ^= basis4[i];
    }
  return false;
}
void push4(int u, int f, int e, ll p) { ufe4[stackSize4][0] = u; ufe4[stackSize4][1] = f; ufe4[stackSize4][2] = e; pstack4[stackSize4] = p; stackSize4++; }
void pop4() { --stackSize4; u4 = ufe4[stackSize4][0]; f4 = ufe4[stackSize4][1]; e4 = ufe4[stackSize4][2]; p4 = pstack4[stackSize4]; }
void dfs4(int root, int n) {
  stackSize4 = 0;
  push4(root, 0, -1, 0);
  while (stackSize4 > 0) {
    pop4();
    if (e4 == -1) {
      path4[u4] = p4;
      visited4[u4] = true;
      e4 = head4[u4];
    } else {
      e4 = next4[e4];
    }
    if (e4 != 0) {
      push4(u4, f4, e4, p4);
      int v = to4[e4];
      ll xr = p4 ^ weight4[e4];
      if (visited4[v]) insert4(xr ^ path4[v]);
      else push4(v, u4, -1, xr);
    }
  }
}
ll query4(ll init) {
  for (int i = BIT4; i >= 0; i--) init = max(init, init ^ basis4[i]);
  return init;
}
void prepare4(int n) {
  cnt4 = 1;
  memset(head4, 0, sizeof(head4));
  memset(basis4, 0, sizeof(basis4));
  memset(visited4, 0, sizeof(visited4));
  memset(path4, 0, sizeof(path4));
}
void addEdge4(int u, int v, ll w) {
  next4[cnt4] = head4[u]; to4[cnt4] = v; weight4[cnt4] = w; head4[u] = cnt4++;
}

// ---- 暴力：独立 DFS 树 + 枚举环组合 ----
struct EdgeB { int v; ll w; };
vector<vector<EdgeB>> gb;
vector<ll> pathB;
vector<int> visB;
vector<ll> cycles;   // 环异或值
void dfsB(int u, int fa, ll p) {
  pathB[u] = p;
  visB[u] = 1;
  for (auto& e : gb[u]) {
    if (e.v == fa) continue;
    if (!visB[e.v]) dfsB(e.v, u, p ^ e.w);
    else cycles.push_back(p ^ e.w ^ pathB[e.v]);   // 任意已访问节点都构成环（含旁支子树）
  }
  visB[u] = 2;
}
ll bruteAns(int n) {
  cycles.clear();
  pathB.assign(n + 1, 0);
  visB.assign(n + 1, 0);
  dfsB(1, 0, 0);
  ll best = pathB[n];
  int k = cycles.size();
  for (int mask = 0; mask < (1 << k); mask++) {
    ll x = 0;
    for (int i = 0; i < k; i++) if (mask >> i & 1) x ^= cycles[i];
    best = max(best, pathB[n] ^ x);
  }
  return best;
}

int main() {
  mt19937 rng(20261017);
  int fails = 0;
  for (int T = 0; T < 3000; T++) {
    int n = 2 + rng() % 7;               // 2..8 个点
    // 先生成随机树保证连通
    gb.assign(n + 1, {});
    vector<pair<int,int>> edges;
    for (int i = 2; i <= n; i++) {
      int p = 1 + rng() % (i - 1);
      edges.push_back({p, i});
    }
    // 再加 0..4 条随机边（可能重复）
    int extra = rng() % 5;
    for (int i = 0; i < extra; i++) {
      int u = 1 + rng() % n, v = 1 + rng() % n;
      if (u == v) continue;
      edges.push_back({u, v});
    }
    prepare4(n);
    for (auto& e : edges) {
      ll w = rng() % 256;
      int u = e.first, v = e.second;
      gb[u].push_back({v, w}); gb[v].push_back({u, w});
      addEdge4(u, v, w); addEdge4(v, u, w);
    }
    dfs4(1, n);
    ll c = query4(path4[n]);
    ll b = bruteAns(n);
    if (c != b) {
      fails++;
      printf("MISMATCH T=%d 课程=%lld 暴力=%lld n=%d edges=%zu\n", T, c, b, n, edges.size());
      if (fails > 3) return 1;
    }
  }
  if (fails) return 1;
  printf("3000 组 ALL OK\n");
  return 0;
}
