#include <bits/stdc++.h>

using namespace std;

const int MAXN = 3000001;
const int BLEN = 3001;
const int BNUM = (MAXN / BLEN) << 1;

char space[BNUM][BLEN];
int pool[BNUM];
int top = 0;

int nxt[BNUM];
int siz[BNUM];

char str[MAXN];

void prepare() {
    for (int i = 1, id = BNUM - 1; i < BNUM; i++, id--) {
        pool[i] = id;
    }
    top = BNUM - 1;
    siz[0] = 0;
    nxt[0] = -1;
}

int assign() {
    return pool[top--];
}

void recycle(int id) {
    pool[++top] = id;
}

int bi, pi;

void find(int pos) {
    int curb = 0;
    while (curb != -1 && pos > siz[curb]) {
        pos -= siz[curb];
        curb = nxt[curb];
    }
    bi = curb;
    pi = pos;
}

void linkAndWrite(int curb, int nextb, char* src, int pos, int len) {
    nxt[nextb] = nxt[curb];
    nxt[curb] = nextb;
    memcpy(space[nextb], src + pos, len);
    siz[nextb] = len;
}

void merge(int curb, int nextb) {
    memcpy(space[curb] + siz[curb], space[nextb], siz[nextb]);
    siz[curb] += siz[nextb];
    nxt[curb] = nxt[nextb];
    recycle(nextb);
}

void split(int curb, int pos) {
    if (curb == -1 || pos == siz[curb]) return;
    int nextb = assign();
    linkAndWrite(curb, nextb, space[curb], pos, siz[curb] - pos);
    siz[curb] = pos;
}

void maintain() {
    for (int curb = 0, nextb; curb != -1; curb = nxt[curb]) {
        nextb = nxt[curb];
        while (nextb != -1 && siz[curb] + siz[nextb] <= BLEN) {
            merge(curb, nextb);
            nextb = nxt[curb];
        }
    }
}

void insert(int pos, int len) {
    find(pos);
    split(bi, pi);
    int curb = bi, newb, done = 0;
    while (done + BLEN <= len) {
        newb = assign();
        linkAndWrite(curb, newb, str, done, BLEN);
        done += BLEN;
        curb = newb;
    }
    if (len > done) {
        newb = assign();
        linkAndWrite(curb, newb, str, done, len - done);
    }
    maintain();
}

void erase(int pos, int len) {
    find(pos);
    split(bi, pi);
    int curb = bi;
    int nextb = nxt[curb];
    while (nextb != -1 && len > siz[nextb]) {
        len -= siz[nextb];
        recycle(nextb);
        nextb = nxt[nextb];
    }
    if (nextb != -1) {
        split(nextb, len);
        recycle(nextb);
        nxt[curb] = nxt[nextb];
    } else {
        nxt[curb] = -1;
    }
    maintain();
}

void get(int pos, int len) {
    find(pos);
    int curb = bi;
    pos = pi;
    int done = (len < siz[curb] - pos) ? len : (siz[curb] - pos);
    memcpy(str, space[curb] + pos, done);
    curb = nxt[curb];
    while (curb != -1 && done + siz[curb] <= len) {
        memcpy(str + done, space[curb], siz[curb]);
        done += siz[curb];
        curb = nxt[curb];
    }
    if (curb != -1 && done < len) {
        memcpy(str + done, space[curb], len - done);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    int pos = 0, len;
    char op[10];
    prepare();
    for (int i = 1; i <= n; i++) {
        cin >> op;
        if (op[0] == 'P') {
            pos--;
        } else if (op[0] == 'N') {
            pos++;
        } else if (op[0] == 'M') {
            cin >> pos;
        } else if (op[0] == 'I') {
            cin >> len;
            for (int j = 0; j < len; ) {
                char ch = cin.get();
                if (32 <= ch && ch <= 126) {
                    str[j++] = ch;
                }
            }
            insert(pos, len);
        } else if (op[0] == 'D') {
            cin >> len;
            erase(pos, len);
        } else {
            cin >> len;
            get(pos, len);
            cout.write(str, len);
            cout.put('\n');
        }
    }
    return 0;
}
