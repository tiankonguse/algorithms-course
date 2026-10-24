#include <bits/stdc++.h>

using namespace std;

struct Edge {
   int u, v, id;
};

bool EdgeCmp(Edge e1, Edge e2) {
   if (e1.u != e2.u) {
       return e1.u < e2.u;
   }
   return e1.v < e2.v;
}

const int MAXN = 501;
const int MAXM = 2001;
int n = 500, m, k;
Edge edgeArr[MAXM << 1];

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int eid[MAXM << 1];
int cntg;

int cur[MAXN];
int deg[MAXN];
bool vis[MAXM];

int path[MAXM];
int cntp;

void addEdge(int u, int v, int id) {
   nxt[++cntg] = head[u];
   to[cntg] = v;
   eid[cntg] = id;
   head[u] = cntg;
}

void connect() {
   sort(edgeArr + 1, edgeArr + k + 1, EdgeCmp);
   for (int l = 1, r = 1; l <= k; l = ++r) {
       while (r + 1 <= k && edgeArr[l].u == edgeArr[r + 1].u) {
           r++;
       }
       for (int i = r; i >= l; i--) {
           deg[edgeArr[i].u]++;
           addEdge(edgeArr[i].u, edgeArr[i].v, edgeArr[i].id);
       }
   }
   for (int i = 1; i <= n; i++) {
       cur[i] = head[i];
   }
}

int undirectedStart() {
   int odd = 0;
   for (int i = 1; i <= n; i++) {
       if ((deg[i] & 1) == 1) {
           odd++;
       }
   }
   if (odd != 0 && odd != 2) {
       return -1;
   }
   for (int i = 1; i <= n; i++) {
       if (odd == 0 && deg[i] > 0) {
           return i;
       }
       if (odd == 2 && (deg[i] & 1) == 1) {
           return i;
       }
   }
   return -1;
}

void euler(int u) {
   for (int e = cur[u]; e > 0; e = cur[u]) {
       cur[u] = nxt[e];
        cerr << "try e=" << e << " u->v " << (e%2==0?2:1) << "->" << to[e] << " eid=" << eid[e] << " vis=" << (int)vis[eid[e]] << "\n";
       if (!vis[eid[e]]) {
           vis[eid[e]] = true;
           euler(to[e]);
       }
   }
        cerr << "push " << u << "\n";
   path[++cntp] = u;
}

int main() {
   ios::sync_with_stdio(false);
   cin.tie(nullptr);
   cin >> m;
   k = 0;
   for (int i = 1, u, v; i <= m; i++) {
       cin >> u >> v;
       edgeArr[++k] = { u, v, i };
       edgeArr[++k] = { v, u, i };
   }
   connect();
   int start = undirectedStart();
   if (start == -1) {
       cout << "No\n";
   } else {
       euler(start);
       if (cntp != m + 1) {
           cout << "No\n";
       } else {
           for (int i = cntp; i >= 1; i--) {
               cout << path[i] << "\n";
           }
       }
   }
   return 0;
}