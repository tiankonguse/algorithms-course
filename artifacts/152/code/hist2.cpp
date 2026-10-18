#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <map>
using namespace std;
const int MAXN = 100001;
int head = 0, cnt = 0;
int key[MAXN], ls[MAXN], rs[MAXN], siz[MAXN];
double priority[MAXN];
int DEPG = 0, DSTEP = 0, DROUND = 0;
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }
void split(int l, int r, int i, int num, int dep) {
    if (dep > 500) { printf("SPLIT LOOP l=%d r=%d i=%d num=%d (r%d s%d)\n", l, r, i, num, DROUND, DSTEP); DEPG = 1; return; }
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        if (key[i] <= num) { rs[l] = i; split(i, r, rs[i], num, dep+1); }
        else { ls[r] = i; split(l, i, ls[i], num, dep+1); }
        up(i);
    }
}
int merge(int l, int r, int dep) {
    if (dep > 500) { printf("MERGE LOOP l=%d r=%d (r%d s%d)\n", l, r, DROUND, DSTEP); DEPG = 1; return 0; }
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { rs[l] = merge(rs[l], r, dep+1); up(l); return l; }
    else { ls[r] = merge(l, ls[r], dep+1); up(r); return r; }
}
void add(int num) {
    split(0, 0, head, num, 0);
    key[++cnt] = num; siz[cnt] = 1;
    priority[cnt] = (double)rand() / RAND_MAX;
    head = merge(merge(rs[0], cnt, 0), ls[0], 0);
}
void remove(int num) {
    split(0, 0, head, num, 0);
    int lm = rs[0], r = ls[0];
    split(0, 0, lm, num - 1, 0);
    int l = rs[0], m = ls[0];
    head = merge(merge(l, merge(ls[m], rs[m], 0), 0), r, 0);
}
int getRank(int num) {
    split(0, 0, head, num - 1, 0);
    int ans = siz[rs[0]] + 1;
    head = merge(rs[0], ls[0], 0);
    return ans;
}
int index(int i, int x, int dep) {
    if (dep > 500) { printf("INDEX LOOP i=%d x=%d (r%d s%d)\n", i, x, DROUND, DSTEP); DEPG = 1; return 0; }
    if (siz[ls[i]] >= x) return index(ls[i], x, dep+1);
    else if (siz[ls[i]] + 1 < x) return index(rs[i], x - siz[ls[i]] - 1, dep+1);
    else return key[i];
}
int index(int x) { return index(head, x, 0); }
int pre(int i, int num, int dep) {
    if (dep > 500) { printf("PRE LOOP i=%d num=%d (r%d s%d)\n", i, num, DROUND, DSTEP); DEPG = 1; return 0; }
    if (i == 0) return INT_MIN;
    if (key[i] >= num) return pre(ls[i], num, dep+1);
    else return max(key[i], pre(rs[i], num, dep+1));
}
int pre(int num) { return pre(head, num, 0); }
int post(int i, int num, int dep) {
    if (dep > 500) { printf("POST LOOP i=%d num=%d (r%d s%d)\n", i, num, DROUND, DSTEP); DEPG = 1; return 0; }
    if (i == 0) return INT_MAX;
    if (key[i] <= num) return post(rs[i], num, dep+1);
    else return min(key[i], post(ls[i], num, dep+1));
}
int post(int num) { return post(head, num, 0); }
map<int,int> mp; int msize = 0;
void madd(int x){ mp[x]++; msize++; }
void mdel(int x){ if(mp[x]>0){ mp[x]--; if(mp[x]==0) mp.erase(x); msize--; } }
void dumpTree() {
    printf("      tree: head=%d", head);
    for (int i = 1; i <= cnt; i++) {
        if (ls[i]) printf(" %d->L%d", i, ls[i]);
        if (rs[i]) printf(" %d->R%d", i, rs[i]);
    }
    printf("\n");
}
int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    for (int r = 0; r < 2 && !DEPG; r++) {
        head=0; cnt=0; mp.clear(); msize=0;
        DROUND = r;
        for (int s = 0; s < 300 && !DEPG; s++) {
            int op = rand()%6+1, x;
            if(op==4){ if(msize==0){ op=1; x=rand()%41-20; } else x = rand()%msize+1; }
            else if(op==2){ if (msize==0) { op=1; x=rand()%41-20; } else { auto it = mp.begin(); int k = rand()%mp.size(); while(k--) ++it; x = it->first; } }
            else { x = rand()%41-20; }
            DSTEP = s;
            if (r == 1 && s < 15) {
                printf("r%d s%d: op%d x=%d", r, s, op, x);
                if(op==1){ add(x); }
                else if(op==2){ remove(x); }
                else if(op==3){ printf(" ->rank%d", getRank(x)); }
                else if(op==4){ printf(" ->idx%d", index(x)); }
                else if(op==5){ printf(" ->pre%d", pre(x)); }
                else { printf(" ->post%d", post(x)); }
                printf("  mp=");
                for(auto& p:mp){ for(int j=0;j<p.second;j++) printf("%d ", p.first); }
                printf("\n");
                dumpTree();
            } else {
                if(op==1){ add(x); madd(x); }
                else if(op==2){ remove(x); mdel(x); }
                else if(op==3){ getRank(x); }
                else if(op==4){ index(x); }
                else if(op==5){ pre(x); }
                else { post(x); }
            }
            if (DEPG) { printf("TRIGGER r%d s%d\n", r, s); break; }
        }
    }
    return 0;
}
