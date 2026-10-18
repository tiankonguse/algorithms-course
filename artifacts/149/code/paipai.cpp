#include <bits/stdc++.h>
using namespace std;

// ===== 课程跳表实现（原样） =====
const int MAXL = 20;
const int MAXN = 100001;
int cnt;
int key[MAXN];
int key_count[MAXN];
int level[MAXN];
int next_node[MAXN][MAXL + 1];
int len[MAXN][MAXL + 1];

void build() {
    cnt = 1;
    key[cnt] = INT_MIN;
    level[cnt] = MAXL;
}

void clear() {
    memset(key + 1, 0, cnt * sizeof(int));
    memset(key_count + 1, 0, cnt * sizeof(int));
    memset(level + 1, 0, cnt * sizeof(int));
    for (int i = 1; i <= cnt; i++) {
        memset(next_node[i], 0, (MAXL + 1) * sizeof(int));
        memset(len[i], 0, (MAXL + 1) * sizeof(int));
    }
    cnt = 0;
}

int randomLevel() {
    int ans = 1;
    while ((rand() / double(RAND_MAX)) < 0.5) {
        ans++;
    }
    return min(ans, MAXL);
}

int find(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        if (next_node[i][h] != 0 && key[next_node[i][h]] == num) {
            return next_node[i][h];
        } else {
            return 0;
        }
    }
    return find(i, h - 1, num);
}

void addCount(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        key_count[next_node[i][h]]++;
    } else {
        addCount(i, h - 1, num);
    }
    len[i][h]++;
}

int addNode(int i, int h, int j) {
    int rightCnt = 0;
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        rightCnt += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        next_node[j][h] = next_node[i][h];
        next_node[i][h] = j;
        len[j][h] = key_count[next_node[j][h]];
        len[i][h] = key_count[next_node[i][h]];
        return rightCnt;
    } else {
        int downCnt = addNode(i, h - 1, j);
        if (h > level[j]) {
            len[i][h]++;
        } else {
            next_node[j][h] = next_node[i][h];
            next_node[i][h] = j;
            len[j][h] = len[i][h] + 1 - downCnt - key_count[j];
            len[i][h] = downCnt + key_count[j];
        }
        return rightCnt + downCnt;
    }
}

void add(int num) {
    if (find(1, MAXL, num) != 0) {
        addCount(1, MAXL, num);
    } else {
        key[++cnt] = num;
        key_count[cnt] = 1;
        level[cnt] = randomLevel();
        addNode(1, MAXL, cnt);
    }
}

void removeCount(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        key_count[next_node[i][h]]--;
    } else {
        removeCount(i, h - 1, num);
    }
    len[i][h]--;
}

void removeNode(int i, int h, int j) {
    if (h < 1) {
        return;
    }
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        i = next_node[i][h];
    }
    if (h > level[j]) {
        len[i][h]--;
    } else {
        next_node[i][h] = next_node[j][h];
        len[i][h] += len[j][h] - 1;
    }
    removeNode(i, h - 1, j);
}

void remove(int num) {
    int j = find(1, MAXL, num);
    if (j != 0) {
        if (key_count[j] > 1) {
            removeCount(1, MAXL, num);
        } else {
            removeNode(1, MAXL, j);
        }
    }
}

int small(int i, int h, int num) {
    int rightCnt = 0;
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        rightCnt += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        return rightCnt;
    } else {
        return rightCnt + small(i, h - 1, num);
    }
}

int getRank(int num) {
    return small(1, MAXL, num) + 1;
}

int index(int i, int h, int x) {
    int c = 0;
    while (next_node[i][h] != 0 && c + len[i][h] < x) {
        c += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        return key[next_node[i][h]];
    } else {
        return index(i, h - 1, x - c);
    }
}

int index(int x) {
    return index(1, MAXL, x);
}

int pre(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        return i == 1 ? INT_MIN : key[i];
    } else {
        return pre(i, h - 1, num);
    }
}

int pre(int num) {
    return pre(1, MAXL, num);
}

int post(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        if (next_node[i][h] == 0) {
            return INT_MAX;
        }
        if (key[next_node[i][h]] > num) {
            return key[next_node[i][h]];
        }
        i = next_node[i][h];
        if (next_node[i][h] == 0) {
            return INT_MAX;
        } else {
            return key[next_node[i][h]];
        }
    } else {
        return post(i, h - 1, num);
    }
}

int post(int num) {
    return post(1, MAXL, num);
}

// ===== 暴力：multiset =====
multiset<int> st;

int main() {
    srand(20261017);
    int T = 400, fails = 0, total = 0;
    for (int t = 0; t < T; t++) {
        clear();
        build();
        st.clear();
        int steps = 300;
        for (int s = 0; s < steps; s++) {
            int op = rand() % 6 + 1;
            int x = rand() % 101 - 50;
            if (op == 1) {
                add(x); st.insert(x);
            } else if (op == 2) {
                remove(x);
                auto it = st.find(x);
                if (it != st.end()) st.erase(it);
            } else if (op == 3) {
                int r = getRank(x);
                int w = 1;
                for (auto v : st) if (v < x) w++;   // multiset 排名：比 x 小的个数+1
                total++;
                if (r != w) { fails++; if (fails <= 5) printf("MISMATCH t%d s%d op3 x=%d course=%d brute=%d\n", t, s, x, r, w); }
            } else if (op == 4) {
                if (st.empty()) continue;
                int k = rand() % (int)st.size() + 1;
                int r = index(k);
                int w = *next(st.begin(), k - 1);
                total++;
                if (r != w) { fails++; if (fails <= 5) printf("MISMATCH t%d s%d op4 k=%d course=%d brute=%d\n", t, s, k, r, w); }
            } else if (op == 5) {
                int r = pre(x);
                int w = INT_MIN;
                for (auto v : st) if (v < x) w = v;
                total++;
                if (r != w) { fails++; if (fails <= 5) printf("MISMATCH t%d s%d op5 x=%d course=%d brute=%d\n", t, s, x, r, w); }
            } else {
                int r = post(x);
                int w = INT_MAX;
                for (auto v : st) if (v > x) { w = v; break; }
                total++;
                if (r != w) { fails++; if (fails <= 5) printf("MISMATCH t%d s%d op6 x=%d course=%d brute=%d\n", t, s, x, r, w); }
            }
        }
    }
    printf("== total %d cases, fails %d ==\n", total, fails);
    return fails ? 1 : 0;
}
