// treap2 P3369 单进程对拍：课程实现内嵌 vs multiset，400 轮 × 300 步
#include <bits/stdc++.h>
using namespace std;

// ===== 课程代码（treap2.cpp 核心，去 main）=====
const int MAXN = 100001;
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
// ===== 课程代码结束 =====

int main() {
    srand(20261018);
    int total = 0, fails = 0;
    for (int round = 0; round < 400; round++) {
        cnt = 0; head = 0;
        memset(key, 0, sizeof(key)); memset(key_count, 0, sizeof(key_count));
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs)); memset(siz, 0, sizeof(siz));
        multiset<int> ms;
        for (int step = 0; step < 300; step++) {
            int op = 1 + rand() % 6;
            int x;
            if (op == 4 && ms.empty()) op = 1; // 空集合时 op4 非法，换成插入
            if (op == 4) x = 1 + rand() % (int)ms.size();
            else x = rand() % 101 - 50;
            if (op == 1) { add(x); ms.insert(x); }
            else if (op == 2) {
                remove_(x);
                auto it = ms.find(x);
                if (it != ms.end()) ms.erase(it);
            } else if (op == 3) {
                long long g = getRank(x);
                long long e = distance(ms.begin(), ms.lower_bound(x)) + 1;
                if (g != e) { fails++; if (fails <= 3) printf("FAIL round=%d step=%d op3 x=%d got=%lld exp=%lld\n", round, step, x, g, e); }
            } else if (op == 4) {
                long long g = index(x);
                auto it = ms.begin(); advance(it, x - 1);
                if (g != *it) { fails++; if (fails <= 3) printf("FAIL round=%d step=%d op4 x=%d got=%lld exp=%lld\n", round, step, x, g, (long long)*it); }
            } else if (op == 5) {
                long long g = pre(x);
                auto it = ms.lower_bound(x);
                long long e = (it == ms.begin()) ? INT_MIN : (long long)(*prev(it));
                if (g != e) { fails++; if (fails <= 3) printf("FAIL round=%d step=%d op5 x=%d got=%lld exp=%lld\n", round, step, x, g, e); }
            } else {
                long long g = post(x);
                auto it = ms.upper_bound(x);
                long long e = (it == ms.end()) ? INT_MAX : (long long)*it;
                if (g != e) { fails++; if (fails <= 3) printf("FAIL round=%d step=%d op6 x=%d got=%lld exp=%lld\n", round, step, x, g, e); }
            }
            total++;
        }
    }
    printf("== treap2 inline: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
