// followup2 P6136 强制在线对拍：初始 n 个 + m 个操作（x^=lastAns），输出 ans 异或
#include <bits/stdc++.h>
using namespace std;

// ===== 课程代码（followup2.cpp 核心，去 main）=====
const int MAXN = 2000001;
int cnt = 0, head = 0;
int key[MAXN], key_count[MAXN], ls[MAXN], rs[MAXN], siz[MAXN];
double priority[MAXN];
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i]; }
int leftRotate(int i) { int r = rs[i]; rs[i] = ls[r]; ls[r] = i; up(i); up(r); return r; }
int rightRotate(int i) { int l = ls[i]; ls[i] = rs[l]; rs[l] = i; up(i); up(l); return l; }
int add(int i, int num) {
    if (i == 0) { key[++cnt] = num; key_count[cnt] = siz[cnt] = 1; priority[cnt] = (double)rand() / RAND_MAX; return cnt; }
    if (key[i] == num) key_count[i]++;
    else if (key[i] > num) ls[i] = add(ls[i], num);
    else rs[i] = add(rs[i], num);
    up(i);
    if (ls[i] != 0 && priority[ls[i]] > priority[i]) return rightRotate(i);
    if (rs[i] != 0 && priority[rs[i]] > priority[i]) return leftRotate(i);
    return i;
}
void add(int num) { head = add(head, num); }
int small(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) return small(ls[i], num);
    else return siz[ls[i]] + key_count[i] + small(rs[i], num);
}
int getRank(int num) { return small(head, num) + 1; }
int index(int i, int x) {
    if (siz[ls[i]] >= x) return index(ls[i], x);
    else if (siz[ls[i]] + key_count[i] < x) return index(rs[i], x - siz[ls[i]] - key_count[i]);
    return key[i];
}
int index(int x) { return index(head, x); }
int pre(int i, int num) {
    if (i == 0) return INT_MIN;
    if (key[i] >= num) return pre(ls[i], num);
    else return max(key[i], pre(rs[i], num));
}
int pre(int num) { return pre(head, num); }
int post(int i, int num) {
    if (i == 0) return INT_MAX;
    if (key[i] <= num) return post(rs[i], num);
    else return min(key[i], post(ls[i], num));
}
int post(int num) { return post(head, num); }
int remove_(int i, int num) {
    if (key[i] < num) rs[i] = remove_(rs[i], num);
    else if (key[i] > num) ls[i] = remove_(ls[i], num);
    else {
        if (key_count[i] > 1) key_count[i]--;
        else {
            if (ls[i] == 0 && rs[i] == 0) return 0;
            else if (ls[i] != 0 && rs[i] == 0) i = ls[i];
            else if (ls[i] == 0 && rs[i] != 0) i = rs[i];
            else {
                if (priority[ls[i]] >= priority[rs[i]]) { i = rightRotate(i); rs[i] = remove_(rs[i], num); }
                else { i = leftRotate(i); ls[i] = remove_(ls[i], num); }
            }
        }
    }
    up(i);
    return i;
}
void remove_(int num) { if (getRank(num) != getRank(num + 1)) head = remove_(head, num); }
void clear() {
    memset(key + 1, 0, cnt * sizeof(int));
    memset(key_count + 1, 0, cnt * sizeof(int));
    memset(ls + 1, 0, cnt * sizeof(int));
    memset(rs + 1, 0, cnt * sizeof(int));
    memset(siz + 1, 0, cnt * sizeof(int));
    memset(priority + 1, 0, cnt * sizeof(int));
    cnt = 0; head = 0;
}
// ===== 课程代码结束 =====

int main() {
    srand(20261019);
    int total = 0, fails = 0;
    for (int round = 0; round < 300; round++) {
        clear();
        int n = 50 + rand() % 100;
        int m = 200;
        vector<int> init;
        for (int i = 0; i < n; i++) init.push_back(rand() % 201 - 100);
        vector<pair<int,int>> ops; // 明文 (op, x)
        for (int i = 0; i < m; i++) {
            int op = 1 + rand() % 6;
            int x = rand() % 201 - 100;
            ops.push_back({op, x});
        }
        // 课程（交互编码）与暴力各维护自己的 lastAns / ans
        for (int v : init) add(v);
        multiset<int> ms;
        for (int v : init) ms.insert(v);
        long long curLast = 0, curAns = 0;
        long long brLast = 0, brAns = 0;
        for (auto& o : ops) {
            int op = o.first;
            int xc = o.second ^ curLast; // 课程端解码
            int xb = o.second ^ brLast;  // 暴力端解码（应为同一值）
            if (op == 1) { add(xc); ms.insert(xb); }
            else if (op == 2) {
                remove_(xc);
                auto it = ms.find(xb);
                if (it != ms.end()) ms.erase(it);
            } else if (op == 3) {
                curLast = getRank(xc); curAns ^= curLast;
                brLast = distance(ms.begin(), ms.lower_bound(xb)) + 1; brAns ^= brLast;
            } else if (op == 4) {
                if (xc < 1 || xc > (int)ms.size()) { curLast = 0; }
                else curLast = index(xc);
                curAns ^= curLast;
                if (xb < 1 || xb > (int)ms.size()) brLast = 0;
                else { auto it = ms.begin(); advance(it, xb - 1); brLast = *it; }
                brAns ^= brLast;
            } else if (op == 5) {
                curLast = pre(xc); curAns ^= curLast;
                auto it = ms.lower_bound(xb);
                if (it == ms.begin()) brLast = INT_MIN;
                else { --it; brLast = *it; }
                brAns ^= brLast;
            } else {
                curLast = post(xc); curAns ^= curLast;
                auto it = ms.upper_bound(xb);
                if (it == ms.end()) brLast = INT_MAX;
                else brLast = *it;
                brAns ^= brLast;
            }
        }
        total++;
        if (curAns != brAns) {
            fails++;
            if (fails <= 3) printf("FAIL round=%d cur=%lld brute=%lld\n", round, curAns, brAns);
        }
    }
    printf("== followup2: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
