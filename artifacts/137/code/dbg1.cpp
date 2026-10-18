#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int rankRows(vector<vector<ll>> rows, int m) {
  int r = 0;
  for (int j = 0; j < m; j++) {
    int piv = -1;
    for (int i = r; i < (int)rows.size(); i++) if (rows[i][j] != 0) { piv = i; break; }
    if (piv == -1) continue;
    swap(rows[r], rows[piv]);
    for (int i = 0; i < (int)rows.size(); i++) {
      if (i != r && rows[i][j] != 0) {
        ll a = rows[r][j], b = rows[i][j];
        ll g = std::gcd(a < 0 ? -a : a, b < 0 ? -b : b);
        a /= g; b /= g;
        for (int k = 0; k < m; k++) rows[i][k] = rows[i][k] * a - rows[r][k] * b;
      }
    }
    r++;
  }
  return r;
}
int main() {
  vector<vector<ll>> rows = {
    {0,2,1,2}, {1,0,1,2}, {3,3,1,0}, {1,3,2,3}
  };
  printf("rank=%d\n", rankRows(rows, 4));
  // 只取物品2,1,3（前三个）
  vector<vector<ll>> rows2 = {{1,0,1,2},{0,2,1,2},{3,3,1,0}};
  printf("rank3=%d\n", rankRows(rows2, 4));
  return 0;
}
