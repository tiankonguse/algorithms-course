#include <bits/stdc++.h>
using namespace std;
// ShowDetails 4 例：验证课程 gauss 的结论分类
// 结论判定：遍历 i=1..n，对角线全 1 且无矛盾 -> 唯一解；有 0 行常数为 1 -> 矛盾；否则多解
int mat[101][101];
int n;
void gauss(int gn){
  for(int i=1;i<=gn;i++){
    for(int j=1;j<=gn;j++){
      if(j<i && mat[j][j]==1) continue;
      if(mat[j][i]==1){ for(int k=1;k<=gn+1;k++) swap(mat[i][k],mat[j][k]); break; }
    }
    if(mat[i][i]==1){
      for(int j=1;j<=gn;j++){
        if(i!=j && mat[j][i]==1){ for(int k=i;k<=gn+1;k++) mat[j][k]^=mat[i][k]; }
      }
    }
  }
}
// 结论：1 唯一解 / -1 矛盾 / 0 多解
int classify(){
  // 严格：矛盾优先（0 行 1 常数）；否则主元行数 == n 且无 0 行 -> 唯一解；否则多解
  for(int i=1;i<=n;i++){
    int allzero=1;
    for(int j=1;j<=n;j++) if(mat[i][j]) { allzero=0; break; }
    if(allzero && mat[i][n+1]) return -1;
  }
  int piv=0;
  for(int i=1;i<=n;i++) if(mat[i][i]) piv++;
  if(piv==n) return 1;
  return 0;
}
void dump(){
  for(int i=1;i<=n;i++){ for(int j=1;j<=n+1;j++) printf("%d ",mat[i][j]); printf("\n"); }
}
int main(){
  // 例1 唯一解
  n=4; memset(mat,0,sizeof(mat));
  mat[1][1]=1;mat[1][2]=1;mat[1][3]=1;mat[1][5]=0;
  mat[2][1]=1;mat[2][3]=1;mat[2][4]=1;mat[2][5]=1;
  mat[3][2]=1;mat[3][3]=1;mat[3][4]=1;mat[3][5]=1;
  mat[4][3]=1;mat[4][4]=1;mat[4][5]=0;
  gauss(4);
  printf("例1 唯一解: 结论=%d (期望1)\n", classify());
  dump();
  // 例2 矛盾
  n=3; memset(mat,0,sizeof(mat));
  mat[1][1]=1;mat[1][2]=1;mat[1][4]=1;
  mat[2][1]=1;mat[2][3]=1;mat[2][4]=1;
  mat[3][2]=1;mat[3][3]=1;mat[3][4]=1;
  gauss(3);
  printf("例2 矛盾: 结论=%d (期望-1)\n", classify());
  dump();
  // 例3 多解
  n=3; memset(mat,0,sizeof(mat));
  mat[1][1]=1;mat[1][3]=1;mat[1][4]=1;
  mat[2][2]=1;mat[2][3]=1;mat[2][4]=1;
  mat[3][1]=1;mat[3][2]=1;mat[3][4]=0;
  gauss(3);
  printf("例3 多解: 结论=%d (期望0)\n", classify());
  dump();
  // 例4 多解但有确定主元
  n=4; memset(mat,0,sizeof(mat));
  mat[1][1]=1;mat[1][3]=1;mat[1][4]=1;mat[1][5]=0;
  mat[2][2]=1;mat[2][3]=1;mat[2][4]=1;mat[2][5]=0;
  mat[3][1]=1;mat[3][2]=1;mat[3][5]=0;
  mat[4][3]=1;mat[4][4]=1;mat[4][5]=1;
  gauss(4);
  printf("例4 多解+确定主元: 结论=%d (期望0)\n", classify());
  dump();
  printf("例4 各主元: x1=%d x2=%d x3=%d (x3 依赖自由元 x4, x3^x4=1)\n",
         mat[1][5], mat[2][5], mat[3][5]);
  return 0;
}
