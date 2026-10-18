#include <bits/stdc++.h>
using namespace std;

// ===== 课程版：Code02_MinimumOperations 改写 =====
int mat[37][37];
int op[37];
int n, ans;

void prepare() {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) mat[i][j] = 0;
    mat[i][i] = 1;
    mat[i][n+1] = 1;
    op[i] = 0;
  }
}

void gauss(int gn) {
  for (int i = 1; i <= gn; i++) {
    for (int j = 1; j <= gn; j++) {
      if (j < i && mat[j][j] == 1) continue;
      if (mat[j][i] == 1) {
        for (int k = 1; k <= gn+1; k++) swap(mat[i][k], mat[j][k]);
        break;
      }
    }
    if (mat[i][i] == 1) {
      for (int j = 1; j <= gn; j++) {
        if (i != j && mat[j][i] == 1) {
          for (int k = i; k <= gn+1; k++) mat[j][k] ^= mat[i][k];
        }
      }
    }
  }
}

void dfs(int i, int num) {
  if (num >= ans) return;
  if (i == 0) { ans = num; }
  else {
    if (mat[i][i] == 0) {
      op[i] = 0; dfs(i-1, num);
      op[i] = 1; dfs(i-1, num+1);
    } else {
      int cur = mat[i][n+1];
      for (int j = i+1; j <= n; j++) if (mat[i][j] == 1) cur ^= op[j];
      dfs(i-1, num + cur);
    }
  }
}

int course_solve(int adj[][37]) {
  prepare();
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++) mat[i][j] = adj[i][j];
  for (int i = 1; i <= n; i++) { mat[i][i] = 1; mat[i][n+1] = 1; }
  gauss(n);
  int sign = 1;
  for (int i = 1; i <= n; i++) if (mat[i][i] == 0) { sign = 0; break; }
  if (sign == 1) {
    ans = 0;
    for (int i = 1; i <= n; i++) if (mat[i][n+1] == 1) ans++;
  } else {
    ans = n;
    dfs(n, 0);
  }
  return ans;
}

// ===== 暴力版：枚举 2^n 操作子集 =====
int brute_solve(int adj[][37]) {
  int best = n + 1;
  for (int mask = 0; mask < (1 << n); mask++) {
    int ok = 1;
    for (int j = 1; j <= n; j++) {
      int cntb = 0;
      if (mask >> (j-1) & 1) cntb++;
      for (int k = 1; k <= n; k++)
        if (adj[j][k] && (mask >> (k-1) & 1)) cntb++;
      if ((cntb & 1) == 0) { ok = 0; break; }  // 目标全 1：每个点被翻转奇数次
    }
    if (ok) best = min(best, __builtin_popcount(mask));
  }
  return best;
}

mt19937 rng(20261016);

int main() {
  int adj[37][37];
  int tested = 0;
  while (tested < 3000) {
    n = 1 + rng() % 8;
    memset(adj, 0, sizeof(adj));
    int m = rng() % 12;
    for (int t = 0; t < m; t++) {
      int u = 1 + rng() % n, v = 1 + rng() % n;
      if (u == v) continue;
      adj[u][v] = adj[v][u] = 1;
    }
    int b = brute_solve(adj);
    if (b > n) continue;  // 无解，重生成（题目保证有解）
    int c = course_solve(adj);
    if (c != b) {
      printf("MISMATCH n=%d course=%d brute=%d\n", n, c, b);
      for (int i = 1; i <= n; i++) { for (int j = 1; j <= n; j++) printf("%d", adj[i][j]); printf(" "); }
      printf("\n");
      return 1;
    }
    tested++;
  }
  printf("t2 OK 3000 groups\n");
  // 固定例 1: n=3 无边: 操作全部 3 个 -> 3
  n = 3; memset(adj, 0, sizeof(adj));
  printf("fix1 无边 n=3 course=%d brute=%d (期望 3)\n", course_solve(adj), brute_solve(adj));
  // 固定例 2: n=2 边(1,2): A=[[1,1],[1,1]], x1^x2=1 -> 1
  n = 2; memset(adj, 0, sizeof(adj)); adj[1][2]=adj[2][1]=1;
  printf("fix2 单边 n=2 course=%d brute=%d (期望 1)\n", course_solve(adj), brute_solve(adj));
  // 固定例 3: n=4 链 1-2-3-4: 操作 2 和 3 -> 2
  n = 4; memset(adj, 0, sizeof(adj));
  adj[1][2]=adj[2][1]=1; adj[2][3]=adj[3][2]=1; adj[3][4]=adj[4][3]=1;
  printf("fix3 链 n=4 course=%d brute=%d (期望 2)\n", course_solve(adj), brute_solve(adj));
  return 0;
}
