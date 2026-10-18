#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
struct Course {
  double sml = 1e-5;
  vector<vector<double>> mat;
  vector<int> basis;
  int n, m, cnt, cost;
  bool insert(int i) {
    for (int j = 1; j <= m; j++) {
      if (fabs(mat[i][j]) >= sml) {
        if (basis[j] == 0) { basis[j] = i; return true; }
        double rate = mat[i][j] / mat[basis[j]][j];
        for (int k = j; k <= m; k++) mat[i][k] -= rate * mat[basis[j]][k];
      }
    }
    return false;
  }
  void compute() {
    cnt = cost = 0;
    sort(mat.begin() + 1, mat.end(), [&](const vector<double>& a, const vector<double>& b) { return a[m + 1] < b[m + 1]; });
    basis.assign(n + 1, 0);
    for (int i = 1; i <= n; i++) if (insert(i)) { cnt++; cost += (int)mat[i][m + 1]; }
  }
  pair<int,int> run(int nn, int mm, const vector<vector<int>>& attr, const vector<int>& price) {
    n = nn; m = mm;
    mat.assign(n + 1, vector<double>(m + 2, 0));
    for (int i = 1; i <= n; i++) { for (int j = 1; j <= m; j++) mat[i][j] = attr[i][j]; mat[i][m + 1] = price[i]; }
    compute();
    return {cnt, cost};
  }
};
int main() {
  int n = 4, m = 4;
  vector<vector<int>> attr(n + 1, vector<int>(m + 1));
  attr[1] = {0, 0, 2, 1, 2};
  attr[2] = {0, 1, 0, 1, 2};
  attr[3] = {0, 3, 3, 1, 0};
  attr[4] = {0, 1, 3, 2, 3};
  vector<int> price = {0, 3, 1, 5, 8};
  auto c = Course().run(n, m, attr, price);
  printf("课程=(%d,%d)\n", c.first, c.second);
  return 0;
}
