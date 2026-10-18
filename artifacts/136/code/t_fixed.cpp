// ShowDetails 4 例：普通消元（例1 12,9,14,11 / 例2 2,5,11,6）与高斯消元（例3 6,37,35,33 / 例4 7,10,4）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 101;
const int BIT = 60;
ll arr[MAXN];
int n;
ll basis1[BIT + 1];
bool zero1;
ll basis2[MAXN];
int len;
bool zero2;

bool insert(ll num) {
  for (int i = BIT; i >= 0; i--) {
    if (num >> i == 1) {
      if (basis1[i] == 0) { basis1[i] = num; return true; }
      num ^= basis1[i];
    }
  }
  return false;
}

void compute1() {
  zero1 = false;
  for (int i = 1; i <= n; i++) {
    if (!insert(arr[i])) zero1 = true;
  }
}

void compute2() {
  len = 1;
  for (ll i = BIT; i >= 0; i--) {
    for (int j = len; j <= n; j++) {
      if ((basis2[j] & (1LL << i)) != 0) { swap(basis2[j], basis2[len]); break; }
    }
    if ((basis2[len] & (1LL << i)) != 0) {
      for (int j = 1; j <= n; j++) {
        if (j != len && (basis2[j] & (1LL << i)) != 0) basis2[j] ^= basis2[len];
      }
      len++;
    }
  }
  len--;
  zero2 = len != n;
}

set<ll> collect;
void dfs(ll* nums, int nn, int i, bool pick, ll path) {
  if (i > nn) { if (pick) collect.insert(path); }
  else {
    dfs(nums, nn, i + 1, pick, path);
    dfs(nums, nn, i + 1, true, path ^ nums[i]);
  }
}
void printXor(ll* nums, int nn) {
  collect.clear();
  dfs(nums, nn, 1, false, 0);
  cout << "至少选择一个数字所有可能的异或和:";
  for (ll s : collect) cout << " " << s;
  cout << endl;
}

int main() {
  cout << "例子1" << endl;
  memset(basis1, 0, sizeof(basis1));
  arr[1] = 12; arr[2] = 9; arr[3] = 14; arr[4] = 11; n = 4;
  cout << "原始数组得到的异或结果如下" << endl;
  printXor(arr, n);
  cout << "普通消元得到的线性基 : ";
  compute1();
  ll b1[MAXN]; int s1 = 0;
  for (int i = BIT; i >= 0; i--) if (basis1[i] != 0) { cout << basis1[i] << " "; b1[++s1] = basis1[i]; }
  cout << endl;
  cout << "是否能异或出0 : " << zero1 << endl;
  cout << "普通消元得到的异或结果如下" << endl;
  printXor(b1, s1);

  cout << "例子2" << endl;
  memset(basis1, 0, sizeof(basis1));
  arr[1] = 2; arr[2] = 5; arr[3] = 11; arr[4] = 6; n = 4;
  cout << "原始数组得到的异或结果如下" << endl;
  printXor(arr, n);
  cout << "普通消元得到的线性基 : ";
  compute1();
  ll b2[MAXN]; int s2 = 0;
  for (int i = BIT; i >= 0; i--) if (basis1[i] != 0) { cout << basis1[i] << " "; b2[++s2] = basis1[i]; }
  cout << endl;
  cout << "是否能异或出0 : " << zero1 << endl;
  cout << "普通消元得到的异或结果如下" << endl;
  printXor(b2, s2);

  cout << "例子3" << endl;
  memset(basis2, 0, sizeof(basis2));
  arr[1] = basis2[1] = 6; arr[2] = basis2[2] = 37; arr[3] = basis2[3] = 35; arr[4] = basis2[4] = 33; n = 4;
  cout << "原始数组得到的异或结果如下" << endl;
  printXor(arr, n);
  cout << "高斯消元得到的线性基 : ";
  compute2();
  for (int i = 1; i <= len; i++) cout << basis2[i] << " ";
  cout << endl;
  cout << "是否能异或出0 : " << zero2 << endl;
  cout << "高斯消元得到的异或结果如下" << endl;
  printXor(basis2, len);

  cout << "例子4" << endl;
  memset(basis1, 0, sizeof(basis1));
  memset(basis2, 0, sizeof(basis2));
  arr[1] = basis2[1] = 7; arr[2] = basis2[2] = 10; arr[3] = basis2[3] = 4; n = 3;
  cout << "原始数组得到的异或结果如下" << endl;
  printXor(arr, n);
  cout << "普通消元" << endl;
  compute1();
  for (int i = BIT; i >= 0; i--) if (basis1[i] != 0) cout << basis1[i] << " ";
  cout << endl;
  cout << "是否能异或出0 : " << zero1 << endl;
  cout << "高斯消元" << endl;
  compute2();
  for (int i = 1; i <= len; i++) cout << basis2[i] << " ";
  cout << endl;
  cout << "是否能异或出0 : " << zero2 << endl;
  return 0;
}
