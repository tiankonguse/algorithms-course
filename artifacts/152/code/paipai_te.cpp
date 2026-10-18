#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
const int MAXN = 100001;
int head = 0, cnt = 0;
char key[MAXN]; int ls[MAXN], rs[MAXN], siz[MAXN]; double priority[MAXN];
char ans[MAXN]; int ansi;
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }
void split(int l, int r, int i, int rank) {
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        if (siz[ls[i]] + 1 <= rank) { rs[l] = i; split(i, r, rs[i], rank - siz[ls[i]] - 1); }
        else { ls[r] = i; split(l, i, ls[i], rank); }
        up(i);
    }
}
int merge(int l, int r) {
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { rs[l] = merge(rs[l], r); up(l); return l; }
    else { ls[r] = merge(l, ls[r]); up(r); return r; }
}
void inorder(int i) {
    if (i != 0) { inorder(ls[i]); ans[++ansi] = key[i]; inorder(rs[i]); }
}
int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    int ROUNDS = 100, STEPS = 100;
    long long total = 0, fails = 0;
    for(int r=0;r<ROUNDS;r++){
        head=0; cnt=0;
        memset(key,0,sizeof(key)); memset(ls,0,sizeof(ls));
        memset(rs,0,sizeof(rs)); memset(siz,0,sizeof(siz));
        int pos = 0;
        vector<char> s;
        for(int st=0;st<STEPS;st++){
            int op = rand()%6;
            if(op==0){ int k = rand()%(s.size()+1); pos = k; }               // Move
            else if(op==1){ if(pos>0) pos--; else { pos = rand()%(s.size()+1); } }  // Prev(保证合法：生成器自己保证)
            else if(op==2){ if(pos<(int)s.size()) pos++; }                     // Next
            else if(op==3){ // Insert
                int len = rand()%8+1;
                vector<char> chars;
                for(int j=0;j<len;j++) chars.push_back(32 + rand()%95);
                split(0,0,head,pos); int l=rs[0], r=ls[0];
                for(int j=0;j<len;j++){ key[++cnt]=chars[j]; siz[cnt]=1; priority[cnt]=(double)rand()/RAND_MAX; l=merge(l,cnt); }
                head = merge(l,r);
                s.insert(s.begin()+pos, chars.begin(), chars.end());
            }
            else if(op==4){ // Delete（保证 pos 后足够字符）
                if (s.size()==0 || pos >= (int)s.size()) continue;
                int len = rand()%((int)s.size()-pos)+1;
                split(0,0,head,pos+len); int lm=rs[0], r=ls[0];
                split(0,0,lm,pos); int l=rs[0];
                head = merge(l,r);
                s.erase(s.begin()+pos, s.begin()+pos+len);
            }
            else { // Get（保证 pos 后足够字符）
                if (s.size()==0 || pos >= (int)s.size()) continue;
                int len = rand()%((int)s.size()-pos)+1;
                split(0,0,head,pos+len); int lm=rs[0], r=ls[0];
                split(0,0,lm,pos); int l=rs[0], m=ls[0];
                ansi=0; inorder(m);
                head = merge(merge(l,m),r);
                total++;
                for(int j=1;j<=ansi;j++){
                    if(ans[j] != s[pos+j-1]){ fails++; printf("r%d s%d get mismatch char %d: %c vs %c\n",r,st,j,ans[j],s[pos+j-1]); if(fails>5) return 1; }
                }
            }
        }
    }
    printf("texteditor(P4008): total=%lld fails=%lld\n", total, fails);
    return fails?1:0;
}
