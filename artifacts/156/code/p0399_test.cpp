// 除法求值对拍驱动：读等式与查询，调 calcEquation 输出答案
#include "p0399.cpp"

int main() {
    int eqn, qn;
    scanf("%d", &eqn);
    vector<vector<string>> equations;
    vector<double> values;
    for (int i = 0; i < eqn; i++) {
        char a[32], b[32];
        double v;
        scanf("%s %s %lf", a, b, &v);
        equations.push_back({string(a), string(b)});
        values.push_back(v);
    }
    scanf("%d", &qn);
    vector<vector<string>> queries;
    for (int i = 0; i < qn; i++) {
        char a[32], b[32];
        scanf("%s %s", a, b);
        queries.push_back({string(a), string(b)});
    }
    vector<double> ans = calcEquation(equations, values, queries);
    for (double x : ans) {
        printf("%.10f\n", x);
    }
    return 0;
}
