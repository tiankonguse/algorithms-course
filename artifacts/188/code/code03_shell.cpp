#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
   static const int MAXN = 200001;
   int n, m;
   vector<vector<int>> *pair;
   int sortv[MAXN];

   int head[MAXN];
   int nxt[MAXN];
   int to[MAXN];
   int cntg;

   int cur[MAXN];
   int outDeg[MAXN];
   int inDeg[MAXN];

   int path[MAXN];
   int cntp;

   void addEdge(int u, int v) {
       nxt[++cntg] = head[u];
       to[cntg] = v;
       head[u] = cntg;
   }

   int kth(int num) {
       int l = 1, r = n, ans = 0;
       while (l <= r) {
           int mid = (l + r) >> 1;
           if (sortv[mid] >= num) {
               ans = mid;
               r = mid - 1;
           } else {
               l = mid + 1;
           }
       }
       return ans;
   }

   void prepare() {
       int len = 0;
       for (int i = 0; i < m; i++) {
           sortv[++len] = (*pair)[i][0];
           sortv[++len] = (*pair)[i][1];
       }
       sort(sortv + 1, sortv + len + 1);
       n = 1;
       for (int i = 2; i <= len; i++) {
           if (sortv[n] != sortv[i]) {
               sortv[++n] = sortv[i];
           }
       }
       cntg = cntp = 0;
       for (int i = 1; i <= n; i++) {
           head[i] = outDeg[i] = inDeg[i] = 0;
       }
   }

   void connect() {
       for (int i = 0, u, v; i < m; i++) {
           u = kth((*pair)[i][0]);
           v = kth((*pair)[i][1]);
           outDeg[u]++;
           inDeg[v]++;
           addEdge(u, v);
       }
       for (int i = 1; i <= n; i++) {
           cur[i] = head[i];
       }
   }

   int directedStart() {
       int start = -1, end = -1;
       for (int i = 1; i <= n; i++) {
           int v = outDeg[i] - inDeg[i];
           if (v < -1 || v > 1 || (v == 1 && start != -1) || (v == -1 && end != -1)) {
               return -1;
           }
           if (v == 1) {
               start = i;
           }
           if (v == -1) {
               end = i;
           }
       }
       if ((start == -1) ^ (end == -1)) {
           return -1;
       }
       if (start != -1) {
           return start;
       }
       for (int i = 1; i <= n; i++) {
           if (outDeg[i] > 0) {
               return i;
           }
       }
       return -1;
   }

   void euler(int u) {
       for (int e = cur[u]; e > 0; e = cur[u]) {
           cur[u] = nxt[e];
           euler(to[e]);
       }
       path[++cntp] = u;
   }

   vector<vector<int>> validArrangement(vector<vector<int>>& pairs) {
       m = (int)pairs.size();
       pair = &pairs;
       prepare();
       connect();
       int start = directedStart();
       if (start == -1) {
           return {};
       }
       euler(start);
       if (cntp != m + 1) {
           return {};
       }
       vector<vector<int>> ans(m, vector<int>(2));
       for (int i = 0, j = cntp; i < m; i++, j--) {
           ans[i][0] = sortv[path[j]];
           ans[i][1] = sortv[path[j - 1]];
       }
       return ans;
   }

};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<int>> pairs(n);
    for (int i = 0; i < n; i++) {
        int a, b; cin >> a >> b;
        pairs[i] = {a, b};
    }
    Solution sol;
    vector<vector<int>> ans = sol.validArrangement(pairs);
    if (ans.empty()) {
        cout << "EMPTY\n";
    } else {
        for (auto& p : ans) cout << p[0] << " " << p[1] << "\n";
    }
    return 0;
}
