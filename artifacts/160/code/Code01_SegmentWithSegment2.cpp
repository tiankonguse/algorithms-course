#include <bits/stdc++.h>

using namespace std;

const int n = 101;
const int m = 1001;
int MINX = 100, MAXX = 200, MINY = 0, MAXY = 1000;
int tree[n << 2][m << 2];

void innerBuild(int yl, int yr, int xi, int yi) {
    tree[xi][yi] = -1;
    if (yl < yr) {
        int mid = (yl + yr) >> 1;
        innerBuild(yl, mid, xi, yi << 1);
        innerBuild(mid + 1, yr, xi, yi << 1 | 1);
    }
}

void innerUpdate(int jobi, int jobv, int yl, int yr, int xi, int yi) {
    if (yl == yr) {
        tree[xi][yi] = max(tree[xi][yi], jobv);
    } else {
        int mid = (yl + yr) >> 1;
        if (jobi <= mid) {
        	innerUpdate(jobi, jobv, yl, mid, xi, yi << 1);
        } else {
        	innerUpdate(jobi, jobv, mid + 1, yr, xi, yi << 1 | 1);
        }
        tree[xi][yi] = max(tree[xi][yi << 1], tree[xi][(yi << 1) | 1]);
    }
}

int innerQuery(int jobl, int jobr, int yl, int yr, int xi, int yi) {
    if (jobl <= yl && yr <= jobr) {
        return tree[xi][yi];
    }
    int mid = (yl + yr) >> 1;
    int ans = -1;
    if (jobl <= mid) {
        ans = max(ans, innerQuery(jobl, jobr, yl, mid, xi, yi << 1));
    }
    if (jobr > mid) {
        ans = max(ans, innerQuery(jobl, jobr, mid + 1, yr, xi, (yi << 1) | 1));
    }
    return ans;
}

void outerBuild(int xl, int xr, int xi) {
	innerBuild(MINY, MAXY, xi, 1);
    if (xl < xr) {
        int mid = (xl + xr) >> 1;
        outerBuild(xl, mid, xi << 1);
        outerBuild(mid + 1, xr, xi << 1 | 1);
    }
}

void outerUpdate(int jobx, int joby, int jobv, int xl, int xr, int xi) {
	innerUpdate(joby, jobv, MINY, MAXY, xi, 1);
    if (xl < xr) {
        int mid = (xl + xr) >> 1;
        if (jobx <= mid) {
        	outerUpdate(jobx, joby, jobv, xl, mid, xi << 1);
        } else {
        	outerUpdate(jobx, joby, jobv, mid + 1, xr, xi << 1 | 1);
        }
    }
}

int outerQuery(int jobxl, int jobxr, int jobyl, int jobyr, int xl, int xr, int xi) {
    if (jobxl <= xl && xr <= jobxr) {
        return innerQuery(jobyl, jobyr, MINY, MAXY, xi, 1);
    }
    int mid = (xl + xr) >> 1;
    int ans = -1;
    if (jobxl <= mid) {
        ans = max(ans, outerQuery(jobxl, jobxr, jobyl, jobyr, xl, mid, xi << 1));
    }
    if (jobxr > mid) {
        ans = max(ans, outerQuery(jobxl, jobxr, jobyl, jobyr, mid + 1, xr, (xi << 1) | 1));
    }
    return ans;
}

int main() {
	int q;
	scanf("%d", &q);
	while(q != 0) {
		outerBuild(MINX, MAXX, 1);
        for (int i = 0; i < q; i++) {
        	char op[2];
            scanf("%s", op);
            if (op[0] == 'I') {
                int a;
                double b, c;
                scanf("%d %lf %lf", &a, &b, &c);
                outerUpdate(a, (int)(b * 10), (int)(c * 10), MINX, MAXX, 1);
            } else {
                int a, b;
                double c, d;
                scanf("%d %d %lf %lf", &a, &b, &c, &d);
                int xl = min(a, b);
                int xr = max(a, b);
                int yl = min((int)(c * 10), (int)(d * 10));
                int yr = max((int)(c * 10), (int)(d * 10));
                int ans = outerQuery(xl, xr, yl, yr, MINX, MAXX, 1);
                if (ans == -1) {
                    printf("-1\n");
                } else {
                    printf("%.1f\n", ans / 10.0);
                }
            }
        }
        scanf("%d", &q);
	}
    return 0;
}
