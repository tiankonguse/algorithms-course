#include <bits/stdc++.h>
using namespace std;
const int BIT=64; const int MAXN=2002; const int MAXM=MAXN/BIT+1;
long long mat[MAXN][MAXM];
int n,m,s,need;
void setbit(int row,int col,int v){ if(v==0) mat[row][col/BIT]&=~(1LL<<(col%BIT)); else mat[row][col/BIT]|=1LL<<(col%BIT); }
int getbit(int row,int col){ return ((mat[row][col/BIT]>>(col%BIT))&1)==1?1:0; }
void eor(int r1,int r2,int bits){ for(int k=0;k<=bits/BIT;k++) mat[r2][k]^=mat[r1][k]; }
void swaprows(int a,int b){ for(int k=0;k<MAXM;k++) swap(mat[a][k],mat[b][k]); }
void gauss_course(int gn){
  need=0;
  for(int i=1;i<=gn;i++){
    for(int j=i;j<=gn;j++){ if(getbit(j,i)==1){ swaprows(i,j); need=max(need,j); break; } }
    if(getbit(i,i)==0) { printf("  !! gauss 提前 return: i=%d 列 %d 找不到主元\n", i, i); return; }
    for(int j=1;j<=n;j++){ if(i!=j && getbit(j,i)==1) eor(i,j,n+2); }
  }
}
mt19937 rng(20261016);
int main(){
  // 复现第一组失败：随机生成直到失败，dump
  for(int T=0;T<50;T++){
    n=2+rng()%7; m=n+rng()%5;
    printf("--- T=%d n=%d m=%d s=%d\n", T, n, m, max(n,m));
    // A 满秩
    int A[20][20]; memset(A,0,sizeof(A));
    for(int r=1;r<=n;r++){
      while(true){
        for(int c=1;c<=n;c++) A[r][c]=rng()&1;
        int B[20][20]; memset(B,0,sizeof(B));
        for(int i=1;i<=r;i++) for(int c=1;c<=n;c++) B[i][c]=A[i][c];
        int rk=0;
        for(int c=1;c<=n && rk<r;c++){
          int p=-1;
          for(int i=rk+1;i<=r;i++) if(B[i][c]){p=i;break;}
          if(p==-1) continue;
          rk++;
          for(int j=1;j<=n;j++) swap(B[rk][j],B[p][j]);
          for(int i=rk+1;i<=r;i++) if(B[i][c]) for(int j=c;j<=n;j++) B[i][j]^=B[rk][j];
        }
        if(rk==r) break;
      }
    }
    vector<int> x(n+1);
    for(int i=1;i<=n;i++) x[i]=rng()&1;
    vector<int> bx(n+1);
    for(int i=1;i<=n;i++){ int t=0; for(int j=1;j<=n;j++) if(A[i][j]&&x[j]) t^=1; bx[i]=t; }
    vector<string> rows(m); vector<int> b(m);
    for(int i=1;i<=n;i++){ string r; for(int j=1;j<=n;j++) r+=char('0'+A[i][j]); rows[i-1]=r; b[i-1]=bx[i]; }
    for(int i=n+1;i<=m;i++){ string r; int t=0; for(int j=1;j<=n;j++){ int v=rng()&1; r+=char('0'+v); if(v&&x[j]) t^=1; } rows[i-1]=r; b[i-1]=t; }
    for(int i=m-1;i>0;i--){ int j=rng()%(i+1); swap(rows[i],rows[j]); swap(b[i],b[j]); }
    // 打印记录
    for(int i=0;i<m;i++) printf("   rec%d: %s %d\n", i+1, rows[i].c_str(), b[i]);
    // 课程
    s=max(n,m);
    memset(mat,0,sizeof(mat));
    for(int i=1;i<=m;i++){ for(int j=1;j<=n;j++) setbit(i,j,rows[i-1][j-1]-'0'); setbit(i,s+1,b[i-1]); }
    gauss_course(s);
    int sign=1;
    for(int i=1;i<=n;i++) if(getbit(i,i)==0){sign=0;break;}
    printf("   sign=%d need=%d\n", sign, need);
    if(sign==0){
      printf("   !! FAIL: 打印消元后矩阵\n");
      for(int i=1;i<=m;i++){ printf("   r%d: ",i); for(int j=1;j<=s+1;j++) printf("%d",getbit(i,j)); printf("\n"); }
      return 0;
    }
  }
  printf("50 组全过\n");
  return 0;
}
