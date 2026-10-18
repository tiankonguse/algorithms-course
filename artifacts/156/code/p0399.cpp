// 除法求值
// 变量用字符串表示，等式 l/r = v，查询 l/r 的值，推不出返回 -1.0
// 带权并查集维护倍数关系（乘法），dist 存到根的比值
// 测试链接 : https://leetcode.cn/problems/evaluate-division/
#include <bits/stdc++.h>
using namespace std;

unordered_map<string, string> father;
unordered_map<string, double> dist;

string findSet(const string& x) {
    if (!father.count(x)) {
        return "";
    }
    if (x != father[x]) {
        string tmp = father[x];
        string fa = findSet(tmp);
        dist[x] = dist[x] * dist[tmp];
        father[x] = fa;
    }
    return father[x];
}

void unionSet(const string& l, const string& r, double v) {
    string lf = findSet(l), rf = findSet(r);
    if (lf != rf) {
        father[lf] = rf;
        dist[lf] = dist[r] / dist[l] * v;
    }
}

double query(const string& l, const string& r) {
    string lf = findSet(l), rf = findSet(r);
    if (lf.empty() || rf.empty() || lf != rf) {
        return -1.0;
    }
    return dist[l] / dist[r];
}

vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values,
                            vector<vector<string>>& queries) {
    father.clear();
    dist.clear();
    for (auto& eq : equations) {
        for (auto& key : eq) {
            father[key] = key;
            dist[key] = 1.0;
        }
    }
    for (int i = 0; i < (int)values.size(); i++) {
        unionSet(equations[i][0], equations[i][1], values[i]);
    }
    vector<double> ans;
    for (auto& q : queries) {
        ans.push_back(query(q[0], q[1]));
    }
    return ans;
}
