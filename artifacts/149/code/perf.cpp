#include <bits/stdc++.h>
using namespace std;
const int MAXL = 20;
const int MAXN = 200001;
int cnt, key[MAXN], key_count[MAXN], level[MAXN], next_node[MAXN][MAXL+1], len[MAXN][MAXL+1];
void build(){ cnt=1; key[cnt]=INT_MIN; level[cnt]=MAXL; }
void clear(){ memset(key+1,0,cnt*sizeof(int)); memset(key_count+1,0,cnt*sizeof(int)); memset(level+1,0,cnt*sizeof(int)); for(int i=1;i<=cnt;i++){ memset(next_node[i],0,(MAXL+1)*sizeof(int)); memset(len[i],0,(MAXL+1)*sizeof(int)); } cnt=0; }
int randomLevel(){ int ans=1; while((rand()/double(RAND_MAX))<0.5) ans++; return min(ans,MAXL); }
int find(int i,int h,int num){ while(next_node[i][h]!=0 && key[next_node[i][h]]<num) i=next_node[i][h]; if(h==1){ if(next_node[i][h]!=0 && key[next_node[i][h]]==num) return next_node[i][h]; return 0; } return find(i,h-1,num); }
void addCount(int i,int h,int num){ while(next_node[i][h]!=0 && key[next_node[i][h]]<num) i=next_node[i][h]; if(h==1) key_count[next_node[i][h]]++; else addCount(i,h-1,num); len[i][h]++; }
int addNode(int i,int h,int j){ int rightCnt=0; while(next_node[i][h]!=0 && key[next_node[i][h]]<key[j]){ rightCnt+=len[i][h]; i=next_node[i][h]; } if(h==1){ next_node[j][h]=next_node[i][h]; next_node[i][h]=j; len[j][h]=key_count[next_node[j][h]]; len[i][h]=key_count[next_node[i][h]]; return rightCnt; } else { int downCnt=addNode(i,h-1,j); if(h>level[j]){ len[i][h]++; } else { next_node[j][h]=next_node[i][h]; next_node[i][h]=j; len[j][h]=len[i][h]+1-downCnt-key_count[j]; len[i][h]=downCnt+key_count[j]; } return rightCnt+downCnt; } }
void add(int num){ if(find(1,MAXL,num)!=0){ addCount(1,MAXL,num); } else { key[++cnt]=num; key_count[cnt]=1; level[cnt]=randomLevel(); addNode(1,MAXL,cnt); } }
int small(int i,int h,int num){ int rightCnt=0; while(next_node[i][h]!=0 && key[next_node[i][h]]<num){ rightCnt+=len[i][h]; i=next_node[i][h]; } if(h==1) return rightCnt; return rightCnt+small(i,h-1,num); }
int getRank(int num){ return small(1,MAXL,num)+1; }
int main(){
    srand(20261017); build();
    int N=100000; auto t0=chrono::steady_clock::now();
    for(int i=0;i<N;i++) add((rand()%2000001)-1000000);
    long long sum=0; int mxh=0;
    for(int i=0;i<N;i++){ int x=(rand()%2000001)-1000000; sum+=getRank(x); if(level[i+2]>mxh) mxh=level[i+2]; }
    auto t1=chrono::steady_clock::now();
    double sec=chrono::duration<double>(t1-t0).count();
    printf("ops=200000 time=%.3fs nodes=%d maxlevel=%d sum=%lld\n", sec, cnt, mxh, sum);
    return 0;
}
