// 异或关系（HDU3234 / UVA12232）
// n 个数编号 0~n-1，m 次操作：
//   I x v     声明 第x个数 = v
//   I x y v   声明 第x个数 ^ 第y个数 = v
//   Q k a1..ak 查询这些数字异或起来的值
// 带权并查集维护异或关系 + 虚点 n（值为 0 的基准）
// 矛盾后不再处理任何操作；只有声明（I）计数，Q 不计数
// 1 <= n <= 2e4，1 <= m <= 4e4，1 <= k <= 15，多测，n=m=0 结束
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=3234
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 20002;
const int MAXK = 21;

int n, m;
bool conflict;
int cnti;
int father[MAXN];
int exclu[MAXN];
int nums[MAXK];
int fas[MAXK];

void prepare() {
    conflict = false;
    cnti = 0;
    for (int i = 0; i <= n; i++) {
        father[i] = i;
        exclu[i] = 0;
    }
}

int find(int i) {
    if (i != father[i]) {
        int tmp = father[i];
        father[i] = find(tmp);
        exclu[i] ^= exclu[tmp];
    }
    return father[i];
}

bool opi(int l, int r, int v) {
    cnti++;
    int lf = find(l), rf = find(r);
    if (lf == rf) {
        if ((exclu[l] ^ exclu[r]) != v) {
            conflict = true;
            return false;
        }
    } else {
        if (lf == n) {
            lf = rf;
            rf = n;
        }
        father[lf] = rf;
        exclu[lf] = exclu[r] ^ exclu[l] ^ v;
    }
    return true;
}

int opq(int k) {
    int ans = 0;
    for (int i = 1; i <= k; i++) {
        int fa = find(nums[i]);
        ans ^= exclu[nums[i]];
        fas[i] = fa;
    }
    sort(fas + 1, fas + k + 1);
    for (int l = 1, r = 1; l <= k; l = ++r) {
        while (r + 1 <= k && fas[r + 1] == fas[l]) {
            r++;
        }
        if ((r - l + 1) % 2 != 0 && fas[l] != n) {
            return -1;
        }
    }
    return ans;
}

// I 行的数据解析：一行里可能是 2 个数（x v）或 3 个数（x y v）
int parseLine(const string& line, int& a, int& b, int& c) {
    stringstream ss(line);
    vector<int> tmp;
    int x;
    while (ss >> x) {
        tmp.push_back(x);
    }
    a = tmp.size() >= 1 ? tmp[0] : 0;
    b = tmp.size() >= 2 ? tmp[1] : 0;
    c = tmp.size() >= 3 ? tmp[2] : 0;
    return (int)tmp.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int caseNo = 0;
    cin >> n >> m;
    string line;
    while (!(n == 0 && m == 0)) {
        prepare();
        cout << "Case " << ++caseNo << ":" << endl;
        for (int i = 1; i <= m; i++) {
            string op;
            cin >> op;
            if (op == "I") {
                getline(cin, line);
                int a, b, c;
                int sz = parseLine(line, a, b, c);
                if (!conflict) {
                    int l, r, v;
                    if (sz == 2) {
                        l = a;
                        r = n;
                        v = b;
                    } else {
                        l = a;
                        r = b;
                        v = c;
                    }
                    if (!opi(l, r, v)) {
                        cout << "The first " << cnti << " facts are conflicting." << endl;
                    }
                }
            } else {
                int k;
                cin >> k;
                for (int j = 1; j <= k; j++) {
                    cin >> nums[j];
                }
                if (!conflict) {
                    int ans = opq(k);
                    if (ans == -1) {
                        cout << "I don't know." << endl;
                    } else {
                        cout << ans << endl;
                    }
                }
            }
        }
        cout << endl;
        cin >> n >> m;
    }
    return 0;
}
