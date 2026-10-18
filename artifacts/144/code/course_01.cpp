#include <bits/stdc++.h>
using namespace std;
int main() {
  int n; cin >> n;
  long long tri[20][20] = {};
  for (int i = 0; i < n; i++) tri[i][0] = tri[i][i] = 1;
  for (int i = 1; i < n; i++)
    for (int j = 1; j < i; j++) tri[i][j] = tri[i-1][j] + tri[i-1][j-1];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= i; j++) cout << tri[i][j] << " ";
    cout << "\n";
  }
}
