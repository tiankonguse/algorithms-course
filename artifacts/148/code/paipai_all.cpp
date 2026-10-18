// 148 课对拍：题1 AVL 实现（P3369）、题2 重建队列（LeetCode）、题3 FollowUp P6136 加强在线
#include <bits/stdc++.h>
using namespace std;

// ==================== 题1/题3 课程 AVL（照 avl1.cpp）====================
const int MAXN = 100001;
int cnt = 0, head = 0;
int key[MAXN], height[MAXN], ls[MAXN], rs[MAXN], key_count[MAXN], siz[MAXN];

void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i]; height[i] = max(height[ls[i]], height[rs[i]]) + 1; }
int leftRotate(int i) { int r = rs[i]; rs[i] = ls[r]; ls[r] = i; up(i); up(r); return r; }
int rightRotate(int i) { int l = ls[i]; ls[i] = rs[l]; rs[l] = i; up(i); up(l); return l; }
int maintain(int i) {
    int lh = height[ls[i]], rh = height[rs[i]];
    if (lh - rh > 1) {
        if (height[ls[ls[i]]] >= height[rs[ls[i]]]) i = rightRotate(i);
        else { ls[i] = leftRotate(ls[i]); i = rightRotate(i); }
    } else if (rh - lh > 1) {
        if (height[rs[rs[i]]] >= height[ls[rs[i]]]) i = leftRotate(i);
        else { rs[i] = rightRotate(rs[i]); i = leftRotate(i); }
    }
    return i;
}
int add(int i, int num) {
    if (i == 0) { key[++cnt] = num; key_count[cnt] = siz[cnt] = height[cnt] = 1; return cnt; }
    if (key[i] == num) key_count[i]++;
    else if (key[i] > num) ls[i] = add(ls[i], num);
    else rs[i] = add(rs[i], num);
    up(i); return maintain(i);
}
void add(int num) { head = add(head, num); }
int getRank(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) return getRank(ls[i], num);
    return siz[ls[i]] + key_count[i] + getRank(rs[i], num);
}
int getRank(int num) { return getRank(head, num) + 1; }
int removeMostLeft(int i, int mostLeft) {
    if (i == mostLeft) return rs[i];
    ls[i] = removeMostLeft(ls[i], mostLeft);
    up(i); return maintain(i);
}
int remove(int i, int num) {
    if (key[i] < num) rs[i] = remove(rs[i], num);
    else if (key[i] > num) ls[i] = remove(ls[i], num);
    else {
        if (key_count[i] > 1) key_count[i]--;
        else {
            if (ls[i] == 0 && rs[i] == 0) return 0;
            else if (ls[i] != 0 && rs[i] == 0) i = ls[i];
            else if (ls[i] == 0 && rs[i] != 0) i = rs[i];
            else {
                int mostLeft = rs[i];
                while (ls[mostLeft] != 0) mostLeft = ls[mostLeft];
                rs[i] = removeMostLeft(rs[i], mostLeft);
                ls[mostLeft] = ls[i]; rs[mostLeft] = rs[i];
                i = mostLeft;
            }
        }
    }
    up(i); return maintain(i);
}
void remove(int num) { if (getRank(num) != getRank(num + 1)) head = remove(head, num); }
int index(int i, int x) {
    if (siz[ls[i]] >= x) return index(ls[i], x);
    if (siz[ls[i]] + key_count[i] < x) return index(rs[i], x - siz[ls[i]] - key_count[i]);
    return key[i];
}
int index(int x) { return index(head, x); }
int pre(int i, int num) {
    if (i == 0) return INT_MIN;
    if (key[i] >= num) return pre(ls[i], num);
    return max(key[i], pre(rs[i], num));
}
int pre(int num) { return pre(head, num); }
int post(int i, int num) {
    if (i == 0) return INT_MAX;
    if (key[i] <= num) return post(rs[i], num);
    return min(key[i], post(ls[i], num));
}
int post(int num) { return post(head, num); }
void avl_clear() {
    memset(key + 1, 0, cnt * sizeof(int)); memset(height + 1, 0, cnt * sizeof(int));
    memset(ls + 1, 0, cnt * sizeof(int)); memset(rs + 1, 0, cnt * sizeof(int));
    memset(key_count + 1, 0, cnt * sizeof(int)); memset(siz + 1, 0, cnt * sizeof(int));
    cnt = 0; head = 0;
}

// ==================== 题2 课程 queue AVL（照 Code02）====================
const int QMAX = 2001;
int qcnt = 0, qhead = 0;
int qkey[QMAX], qheight[QMAX], qls[QMAX], qrs[QMAX], qvalue[QMAX], qsize[QMAX];

void qup(int i) { qsize[i] = qsize[qls[i]] + qsize[qrs[i]] + 1; qheight[i] = max(qheight[qls[i]], qheight[qrs[i]]) + 1; }
int qleftRotate(int i) { int r = qrs[i]; qrs[i] = qls[r]; qls[r] = i; qup(i); qup(r); return r; }
int qrightRotate(int i) { int l = qls[i]; qls[i] = qrs[l]; qrs[l] = i; qup(i); qup(l); return l; }
int qmaintain(int i) {
    int lh = qheight[qls[i]], rh = qheight[qrs[i]];
    if (lh - rh > 1) {
        if (qheight[qls[qls[i]]] >= qheight[qrs[qls[i]]]) i = qrightRotate(i);
        else { qls[i] = qleftRotate(qls[i]); i = qrightRotate(i); }
    } else if (rh - lh > 1) {
        if (qheight[qrs[qrs[i]]] >= qheight[qls[qrs[i]]]) i = qleftRotate(i);
        else { qrs[i] = qrightRotate(qrs[i]); i = qleftRotate(i); }
    }
    return i;
}
int qadd(int i, int rank, int num, int idx) {
    if (i == 0) { qkey[++qcnt] = num; qvalue[qcnt] = idx; qsize[qcnt] = qheight[qcnt] = 1; return qcnt; }
    if (qsize[qls[i]] >= rank) qls[i] = qadd(qls[i], rank, num, idx);
    else qrs[i] = qadd(qrs[i], rank - qsize[qls[i]] - 1, num, idx);
    qup(i); return qmaintain(i);
}
void qadd(int num, int idx) { qhead = qadd(qhead, idx, num, idx); }
int qfi = 0;
void qfill(int ans[][2], int i) {
    if (i == 0) return;
    qfill(ans, qls[i]);
    ans[qfi][0] = qkey[i]; ans[qfi++][1] = qvalue[i];
    qfill(ans, qrs[i]);
}
void qclear() {
    memset(qkey + 1, 0, qcnt * sizeof(int)); memset(qheight + 1, 0, qcnt * sizeof(int));
    memset(qls + 1, 0, qcnt * sizeof(int)); memset(qrs + 1, 0, qcnt * sizeof(int));
    memset(qvalue + 1, 0, qcnt * sizeof(int)); memset(qsize + 1, 0, qcnt * sizeof(int));
    qcnt = 0; qhead = 0; qfi = 0;
}

// ==================== 暴力 ====================
// 题1/题3 暴力：multiset
// 题2 暴力：vector insert

mt19937 rng(12345);

int main() {
    int fails = 0, total = 0;

    // ---------- 题1：AVL 六操作 vs multiset ----------
    {
        const int T = 300, R = 400;
        for (int r = 0; r < R; r++) {
            avl_clear();
            multiset<int> ms;
            for (int t = 0; t < T; t++) {
                int op = rng() % 6 + 1;
                int x = (int)(rng() % 101) - 50;
                if (op == 1) { add(x); ms.insert(x); }
                else if (op == 2) { remove(x); auto it = ms.find(x); if (it != ms.end()) ms.erase(it); }
                else if (op == 3) {
                    // 排名：比 x 小的个数 + 1
                    long long a = getRank(x);
                    long long b = distance(ms.begin(), ms.lower_bound(x)) + 1;
                    if (a != b) { fails++; printf("t1 FAIL r=%d t=%d op3 x=%d course=%lld brute=%lld\n", r, t, x, a, b); if (fails > 5) return 1; }
                    total++;
                } else if (op == 4) {
                    int k = (rng() % max(1, (int)ms.size())) + 1;
                    if (ms.empty()) continue;
                    long long a = index(k);
                    auto it = ms.begin(); advance(it, k - 1);
                    long long b = *it;
                    if (a != b) { fails++; printf("t1 FAIL r=%d t=%d op4 k=%d course=%lld brute=%lld\n", r, t, k, a, b); if (fails > 5) return 1; }
                    total++;
                } else if (op == 5) {
                    long long a = pre(x);
                    auto it = ms.lower_bound(x);
                    long long b = (it == ms.begin()) ? (long long)INT_MIN : *prev(it);
                    if (a != b) { fails++; printf("t1 FAIL r=%d t=%d op5 x=%d course=%lld brute=%lld\n", r, t, x, a, b); if (fails > 5) return 1; }
                    total++;
                } else {
                    long long a = post(x);
                    auto it = ms.upper_bound(x);
                    long long b = (it == ms.end()) ? (long long)INT_MAX : *it;
                    if (a != b) { fails++; printf("t1 FAIL r=%d t=%d op6 x=%d course=%lld brute=%lld\n", r, t, x, a, b); if (fails > 5) return 1; }
                    total++;
                }
            }
        }
        printf("题1 OK（%d 轮 x %d 步）\n", R, T);
    }

    // ---------- 题2：重建队列 vs vector insert ----------
    {
        const int R = 500;
        for (int r = 0; r < R; r++) {
            int n = rng() % 8 + 1; // 1..8
            // 构造必有解：随机生成一个合法队列，再算每个人 b
            vector<pair<int,int>> people(n); // (身高, b)
            int q[9];
            for (int i = 0; i < n; i++) q[i] = rng() % 6; // 身高 0..5 可重复
            for (int i = 0; i < n; i++) {
                int b = 0;
                for (int j = 0; j < i; j++) if (q[j] >= q[i]) b++;
                people[i] = make_pair(q[i], b);
            }
            // 打乱 people 顺序（模拟输入无序）
            shuffle(people.begin(), people.end(), rng);

            // 课程版：排序 + AVL 按 rank 插入 + 中序
            sort(people.begin(), people.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
                return a.first != b.first ? (a.first > b.first) : (a.second < b.second);
            });
            qclear();
            for (auto& p : people) qadd(p.first, p.second);
            int ansC[9][2];
            qfill(ansC, qhead);

            // 暴力：同样排序 + vector insert
            vector<pair<int,int>> brute;
            for (auto& p : people) brute.insert(brute.begin() + p.second, p);

            if ((int)brute.size() != n) { fails++; printf("t2 FAIL size\n"); if (fails > 5) return 1; }
            for (int i = 0; i < n; i++) {
                if (ansC[i][0] != brute[i].first || ansC[i][1] != brute[i].second) {
                    fails++; printf("t2 FAIL r=%d n=%d i=%d course=(%d,%d) brute=(%d,%d)\n", r, n, i, ansC[i][0], ansC[i][1], brute[i].first, brute[i].second);
                    if (fails > 5) return 1;
                }
                total++;
            }
        }
        printf("题2 OK（%d 轮）\n", R);
    }

    // ---------- 题3：P6136 加强在线 vs multiset ----------
    {
        const int R = 300;
        for (int r = 0; r < R; r++) {
            avl_clear();
            multiset<int> ms;
            int n = rng() % 8 + 1;
            for (int i = 0; i < n; i++) { int x = (int)(rng() % 41) - 20; add(x); ms.insert(x); }
            int m = rng() % 50 + 1;
            int lastAns = 0, ansC = 0, ansB = 0;
            for (int t = 0; t < m; t++) {
                int op = rng() % 6 + 1;
                int x = (int)(rng() % 41) - 20;
                int xx = x ^ lastAns;
                if (op == 1) { add(xx); ms.insert(xx); }
                else if (op == 2) { remove(xx); auto it = ms.find(xx); if (it != ms.end()) ms.erase(it); }
                else if (op == 3) { lastAns = getRank(xx); ansC ^= lastAns; ansB ^= (int)(distance(ms.begin(), ms.lower_bound(xx)) + 1); }
                else if (op == 4) {
                    if (ms.empty()) continue;
                    int k = (rng() % (int)ms.size()) + 1;
                    lastAns = index(k);
                    auto it = ms.begin(); advance(it, k - 1);
                    ansC ^= lastAns; ansB ^= *it;
                }
                else if (op == 5) { lastAns = pre(xx); auto it = ms.lower_bound(xx); ansC ^= lastAns; ansB ^= (it == ms.begin()) ? INT_MIN : *prev(it); }
                else { lastAns = post(xx); auto it = ms.upper_bound(xx); ansC ^= lastAns; ansB ^= (it == ms.end()) ? INT_MAX : *it; }
            }
            if (ansC != ansB) { fails++; printf("t3 FAIL r=%d ansC=%d ansB=%d\n", r, ansC, ansB); if (fails > 5) return 1; }
            total++;
        }
        printf("题3 OK（%d 轮）\n", R);
    }

    printf("== total %d cases, fails %d ==\n", total, fails);
    return fails ? 1 : 0;
}
