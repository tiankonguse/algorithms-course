// 对拍：withoutcount（无词频 FHQ） vs multiset，P3369 六操作
// 单进程 inline：课程代码去 main 内嵌 + multiset 同进程
// 约束：op4 x 必须 <= 当前集合大小；空集合时 op4 转 op1（避免 multiset advance 越界 UB）
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <map>
using namespace std;

// ===== 课程代码（withoutcount）=====
const int MAXN = 100001;
int head = 0;
int cnt = 0;
int key[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];
double priority[MAXN];

void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }

void split(int l, int r, int i, int num) {
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        if (key[i] <= num) { rs[l] = i; split(i, r, rs[i], num); }
        else { ls[r] = i; split(l, i, ls[i], num); }
        up(i);
    }
}

int merge(int l, int r) {
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { rs[l] = merge(rs[l], r); up(l); return l; }
    else { ls[r] = merge(l, ls[r]); up(r); return r; }
}

void add(int num) {
    split(0, 0, head, num);
    key[++cnt] = num;
    siz[cnt] = 1;
    priority[cnt] = (double)rand() / RAND_MAX;
    head = merge(merge(rs[0], cnt), ls[0]);
}

void remove(int num) {
    split(0, 0, head, num);
    int lm = rs[0];
    int r = ls[0];
    split(0, 0, lm, num - 1);
    int l = rs[0];
    int m = ls[0];
    head = merge(merge(l, merge(ls[m], rs[m])), r);
}

int getRank(int num) {
    split(0, 0, head, num - 1);
    int ans = siz[rs[0]] + 1;
    head = merge(rs[0], ls[0]);
    return ans;
}

int index(int i, int x) {
    if (siz[ls[i]] >= x) return index(ls[i], x);
    else if (siz[ls[i]] + 1 < x) return index(rs[i], x - siz[ls[i]] - 1);
    else return key[i];
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
// ===== 课程代码结束 =====

// 暴力 multiset 的等价实现（避免 std::multiset advance 的坑，用 map 计数模拟 multiset）
map<int,int> mp; // value -> count
int msize = 0;

void madd(int x){ mp[x]++; msize++; }
void mdel(int x){ if(mp[x]>0){ mp[x]--; if(mp[x]==0) mp.erase(x); msize--; } }
int mrank(int x){ // 比 x 小的个数 + 1
    int c=0; for(auto& p:mp){ if(p.first<x) c+=p.second; else break; } return c+1;
}
int mindex(int k){ for(auto& p:mp){ if(k<=p.second) return p.first; k-=p.second; } return -1; }
int mpre(int x){ auto it=mp.lower_bound(x); if(it==mp.begin()) return INT_MIN; --it; return it->first; }
int mpost(int x){ auto it=mp.upper_bound(x); if(it==mp.end()) return INT_MAX; return it->first; }

int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    int ROUNDS = 400, STEPS = 300;
    long long total = 0, fails = 0;
    for(int r=0;r<ROUNDS;r++){
        head=0; cnt=0; mp.clear(); msize=0;
        memset(key, 0, sizeof(key)); memset(ls, 0, sizeof(ls));
        memset(rs, 0, sizeof(rs)); memset(siz, 0, sizeof(siz));
        for(int s=0;s<STEPS;s++){
            int op = rand()%6+1;
            int x;
            if(op==4){ // 排名查询：x 必须在 [1, size]
                if(msize==0){ op=1; x=rand()%41-20; }
                else x = rand()%msize+1;
            } else if(op==2){ // 删除：随机选已存在的，可能不存在（no-op）
                x = rand()%41-20;
            } else {
                x = rand()%41-20;
            }
            total++;
            if(op==1){ add(x); madd(x); }
            else if(op==2){ remove(x); mdel(x); }
            else if(op==3){ int a=getRank(x), b=mrank(x); if(a!=b){ fails++; printf("r%d s%d op3 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>10) return 1; } }
            else if(op==4){ int a=index(x), b=mindex(x); if(a!=b){ fails++; printf("r%d s%d op4 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>10) return 1; } }
            else if(op==5){ int a=pre(x), b=mpre(x); if(a!=b){ fails++; printf("r%d s%d op5 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>10) return 1; } }
            else { int a=post(x), b=mpost(x); if(a!=b){ fails++; printf("r%d s%d op6 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>10) return 1; } }
        }
    }
    printf("withoutcount: total=%lld fails=%lld\n", total, fails);
    return fails?1:0;
}
