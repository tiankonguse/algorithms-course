#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;

int arr[MAXN];
int left_[MAXN];
int right_[MAXN];
int stack_[MAXN];
int n;

void build() {
    for (int i = 1, top = 0, pos = 0; i <= n; i++) {
        pos = top;
        while (pos > 0 && arr[stack_[pos]] > arr[i]) {
            pos--;
        }
        if (pos > 0) {
            right_[stack_[pos]] = i;
        }
        if (pos < top) {
            left_[i] = stack_[pos + 1];
        }
        stack_[++pos] = i;
        top = pos;
    }
}

// 防止递归爆栈用迭代的方式实现先序遍历
void pre() {
    int size = 1, i = 0, cur;
    while (size > 0) {
        cur = stack_[size--];
        arr[++i] = cur;
        if (right_[cur] != 0) {
            stack_[++size] = right_[cur];
        }
        if (left_[cur] != 0) {
            stack_[++size] = left_[cur];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1, v; i <= n; i++) {
        cin >> v;
        arr[v] = i;
    }
    build();
    pre();
    for (int i = 1; i <= n; i++) {
        if (i > 1) {
            cout << " ";
        }
        cout << arr[i];
    }
    cout << endl;
    return 0;
}
