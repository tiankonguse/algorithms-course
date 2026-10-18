#include <bits/stdc++.h>
using namespace std;
// 135 课题2 POJ2947 Widget Factory：工具工厂（mod 7 完整版，保留主元-自由元关系）
int MOD = 7;
int MAXN = 302;
int mat[302][302];
int inv[7];
string days[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
int n, m, s;

void invs() {
  inv[1] = 1;
  for (int i = 2; i < MOD; i++) {
    inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD);
  }
}
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
void prepare() {
  for (int i = 1; i <= s; i++)
    for (int j = 1; j <= s + 1; j++) mat[i][j] = 0;
}
int day(string str) {
  for (int i = 0; i < 7; i++) if (str == days[i]) return i;
  return -1;
}
void gauss(int nn) {
  for (int i = 1; i <= nn; i++) {
    for (int j = 1; j <= nn; j++) {
      if (j < i && mat[j][j] != 0) continue;
      if (mat[j][i] != 0) { swap(mat[i], mat[j]); break; }
    }
    if (mat[i][i] != 0) {
      for (int j = 1; j <= nn; j++) {
        if (i != j && mat[j][i] != 0) {
          int g = gcd(mat[j][i], mat[i][i]);
          int a = mat[i][i] / g;
          int b = mat[j][i] / g;
          if (j < i && mat[j][j] != 0) {
            for (int k = j; k < i; k++) {
              mat[j][k] = (mat[j][k] * a) % MOD;
            }
          }
          for (int k = i; k <= nn + 1; k++) {
            mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
          }
        }
      }
    }
  }
  for (int i = 1; i <= nn; i++) {
    if (mat[i][i] != 0) {
      bool flag = false;
      for (int j = i + 1; j <= nn; j++) {
        if (mat[i][j] != 0) { flag = true; break; }
      }
      if (!flag) {
        mat[i][nn + 1] = (mat[i][nn + 1] * inv[mat[i][i]]) % MOD;
        mat[i][i] = 1;
      }
    }
  }
}
int main() {
  invs();
  ios::sync_with_stdio(false);
  cin.tie(0);
  while (cin >> n >> m && (n != 0 && m != 0)) {
    s = max(n, m);
    prepare();
    for (int i = 1; i <= m; i++) {
      int k; string st, et;
      cin >> k >> st >> et;
      for (int j = 1; j <= k; j++) {
        int tool; cin >> tool;
        mat[i][tool] = (mat[i][tool] + 1) % MOD;
      }
      mat[i][s + 1] = ((day(et) - day(st) + 1) % MOD + MOD) % MOD;
    }
    gauss(s);
    int sign = 1;
    for (int i = 1; i <= s; i++) {
      if (mat[i][i] == 0 && mat[i][s + 1] != 0) { sign = -1; break; }
      if (i <= n && mat[i][i] == 0) { sign = 0; }
    }
    if (sign == -1) {
      cout << "Inconsistent data." << endl;
    } else if (sign == 0) {
      cout << "Multiple solutions." << endl;
    } else {
      for (int i = 1; i <= n; i++) {
        if (mat[i][s + 1] < 3) mat[i][s + 1] += 7;
      }
      for (int i = 1; i < n; i++) cout << mat[i][s + 1] << " ";
      cout << mat[n][s + 1] << endl;
    }
  }
  return 0;
}
