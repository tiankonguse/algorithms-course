#include <cstdio>

const int MAXN = 1000002;
int n, k, m;

int cur[MAXN];
int path[MAXN];
int cntp;

int sta[MAXN][2];
int u, e;
int stacksize;

void push(int u, int e) {
    sta[stacksize][0] = u;
    sta[stacksize][1] = e;
    stacksize++;
}

void pop() {
    stacksize--;
    u = sta[stacksize][0];
    e = sta[stacksize][1];
}

void prepare(int len, int num) {
    n = len;
    k = num;
    m = 1;
    for (int i = 1; i <= n - 1; i++) {
        m *= k;
    }
    for (int i = 0; i < m; i++) {
        cur[i] = 0;
    }
    cntp = 0;
}

void euler(int node, int edge) {
    stacksize = 0;
    push(node, edge);
    while (stacksize > 0) {
        pop();
        if (cur[u] < k) {
            int ne = cur[u]++;
            push(u, e);
            push((u * k + ne) % m, ne);
        } else {
            path[++cntp] = e;
        }
    }
}

int main() {
    int len;
    int num = 10;
    scanf("%d", &len);
    while (len != 0) {
        prepare(len, num);
        euler(0, -1);
        for (int i = 1; i <= n - 1; i++) {
            putchar('0');
        }
        for (int i = cntp - 1; i >= 1; i--) {
            putchar((char)('0' + path[i]));
        }
        putchar('\n');
        scanf("%d", &len);
    }
    return 0;
}
