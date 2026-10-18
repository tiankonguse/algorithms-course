#include <bits/stdc++.h>
using namespace std;
const int BIT = 64;
const int MAXN = 2002;
const int MAXM = MAXN / BIT + 1;
long long mat[MAXN][MAXM];
int n, m, s, need;
void setbit(int row, int col, int v){ if(v==0) mat[row][col/BIT]&=~(1LL<<(col%BIT)); else mat[row][col/BIT]|=1LL<<(col%BIT); }
int getbit(int row, int col){ return ((mat[row][col/BIT]>>(col%BIT))&1)==1?1:0; }
void eor(int row1, int row2, int bits){ for(int k=0;k<=bits/BIT;k++) mat[row2][k]^=mat[row1][k]; }
void swaprows(int a,int b){ for(int k=0;k<MAXM;k++) swap(mat[a][k],mat[b][k]); }
void gauss_course(int gn){
  need=0;
  for(int i=1;i<=gn;i++){
    for(int j=i;j<=gn;j++){ if(getbit(j,i)==1){ swaprows(i,j); need=max(need,j); break; } }
    if(getbit(i,i)==0) return;
    for(int j=1;j<=n;j++){ if(i!=j && getbit(j,i)==1) eor(i,j,n+2); }
  }
}
void dump(){
  printf("need=%d\n", need);
  for(int i=1;i<=m;i++){
    printf("r%d: ", i);
    for(int j=1;j<=s+1;j++) printf("%d", getbit(i,j));
    printf("  (long0=%016llx)\n", mat[i][0]);
  }
}
int main(){
  n=2; m=4; s=max(n,m);
  // 记录1: 10 b=0 (x=(0,1) 时 1*0^0*1=0)  记录2: 11 b=1 (0^1=1)
  // 冗余: 记录3: 10 b=0  记录4: 01 b=1
  string rows[5]={"","10","11","10","01"};
  int b[5]={0,0,1,0,1};
  for(int i=1;i<=m;i++){
    for(int j=1;j<=n;j++) setbit(i,j,rows[i][j-1]-'0');
    setbit(i,s+1,b[i]);
  }
  printf("初始 (s=%d, 常数列=%d, n+2=%d):\n", s, s+1, n+2);
  dump();
  gauss_course(s);
  printf("消元后:\n");
  dump();
  printf("课程解: ");
  for(int i=1;i<=n;i++) printf("%d ", getbit(i,s+1));
  printf(" (真解 x=(0,1), 期望 (0,1))\n");
  return 0;
}
