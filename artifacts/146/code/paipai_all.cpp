// 146 课对拍：康托展开/逆康托展开/约瑟夫环/约瑟夫加强/完美洗牌/摇摆排序II
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ============ 课程实现 ============

// --- 题1 康托展开（树状数组 + fac 表）---
const int MAXN1 = 1000001;
const int MOD = 998244353;
int fac1[MAXN1], tree1[MAXN1];
int n1, arr1[MAXN1];
int lowbit1(int i){ return i & -i; }
int sum1(int i){ int ans=0; while(i>0){ ans=(ans+tree1[i])%MOD; i-=lowbit1(i);} return ans; }
void add1(int i,int v){ while(i<=n1){ tree1[i]+=v; i+=lowbit1(i);} }
ll cantor(){ // 返回 1-based 名次 mod
  fac1[0]=1; for(int i=1;i<n1;i++) fac1[i]=(ll)fac1[i-1]*i%MOD;
  for(int i=1;i<=n1;i++) add1(i,1);
  ll ans=0;
  for(int i=1;i<=n1;i++){
    ans=(ans+(ll)sum1(arr1[i]-1)*fac1[n1-i]%MOD)%MOD;
    add1(arr1[i],-1);
  }
  ans=(ans+1)%MOD;
  return ans;
}

// --- 题2 逆康托展开（线段树二分 + 阶乘进制）---
const int MAXN2 = 100001;
int n2; ll m2; ll arr2[MAXN2];
int seg[MAXN2<<2];
void build2(int l,int r,int i){ if(l==r){seg[i]=1;} else { int mid=(l+r)>>1; build2(l,mid,i<<1); build2(mid+1,r,i<<1|1); seg[i]=seg[i<<1]+seg[i<<1|1];} }
void add2(int jobi,int jobv,int l,int r,int i){ if(l==r){seg[i]+=jobv;} else { int mid=(l+r)>>1; if(jobi<=mid) add2(jobi,jobv,l,mid,i<<1); else add2(jobi,jobv,mid+1,r,i<<1|1); seg[i]=seg[i<<1]+seg[i<<1|1];} }
int sumseg2(int jobl,int jobr,int l,int r,int i){ if(jobl<=l&&r<=jobr) return seg[i]; int mid=(l+r)>>1; int ans=0; if(jobl<=mid) ans+=sumseg2(jobl,jobr,l,mid,i<<1); if(jobr>mid) ans+=sumseg2(jobl,jobr,mid+1,r,i<<1|1); return ans; }
int getAndDelete2(int k,int l,int r,int i){ int ans; if(l==r){ seg[i]--; ans=l;} else { int mid=(l+r)>>1; if(seg[i<<1]>=k) ans=getAndDelete2(k,l,mid,i<<1); else ans=getAndDelete2(k-seg[i<<1],mid+1,r,i<<1|1); seg[i]=seg[i<<1]+seg[i<<1|1];} return ans; }
void inverseCantor(){
  build2(1,n2,1);
  for(int i=1;i<=n2;i++){ int x=(int)arr2[i]; if(x==1) arr2[i]=0; else arr2[i]=sumseg2(1,x-1,1,n2,1); add2(x,-1,1,n2,1); }
  arr2[n2]+=m2;
  for(int i=n2;i>=1;i--){ arr2[i-1]+=arr2[i]/(n2-i+1); arr2[i]%=n2-i+1; }
  build2(1,n2,1);
  for(int i=1;i<=n2;i++) arr2[i]=getAndDelete2((int)arr2[i]+1,1,n2,1);
}

// --- 题3 约瑟夫环 ---
int joseph(int n,int k){ int ans=1; for(int c=2;c<=n;c++) ans=(ans+k-1)%c+1; return ans; }

// --- 题4 约瑟夫环加强 ---
int joseph1(int n,int arr4[]){
  if(n==1) return 1;
  int nxt[100005]; for(int i=1;i<n;i++) nxt[i]=i+1; nxt[n]=1;
  int pre=n,cur=1;
  for(int i=1;i<n;i++){
    for(int cnt=1;cnt<arr4[i];cnt++){ pre=cur; cur=nxt[cur]; }
    nxt[pre]=nxt[cur]; cur=nxt[cur];
  }
  return cur;
}
int joseph2(int n,int arr4[]){
  if(n==1) return 1;
  int ans=1;
  for(int c=2,i=n-1;c<=n;c++,i--) ans=(ans+arr4[i]-1)%c+1;
  return ans;
}

// --- 题5 完美洗牌 ---
const int MAXN5 = 200005;
int start5[64], split5[64], size5;
void shuffle1(int* arr,int l,int r){ int n=r-l+1; vector<int> help(n); for(int l1=l,r1=(l+r)/2+1,j=0;j<n;l1++,r1++){ help[j++]=arr[r1]; help[j++]=arr[l1]; } for(int i=l,j=0;j<n;i++,j++) arr[i]=help[j]; }
void swap5(int* arr,int i,int j){ int t=arr[i];arr[i]=arr[j];arr[j]=t; }
void reverse5(int* arr,int l,int r){ while(l<r) swap5(arr,l++,r--); }
void rotate5(int* arr,int l,int m,int r){ reverse5(arr,l,m); reverse5(arr,m+1,r); reverse5(arr,l,r); }
int to5(int i,int l,int r){ if(i<=((l+r)>>1)) return i+(i-l+1); else return i-(r-i+1); }
void circle5(int* arr,int l,int r,int i){ for(int j=1,init,cur,next,curv,nextv;j<=i;j++){ init=cur=l+start5[j]-1; next=to5(cur,l,r); curv=arr[cur]; while(next!=init){ nextv=arr[next]; arr[next]=curv; curv=nextv; cur=next; next=to5(cur,l,r);} arr[init]=curv; } }
void build5(int n){ size5=0; for(int s=1,p=2;p<=n;s*=3,p=s*3-1){ start5[++size5]=s; split5[size5]=p; } }
void shuffle2(int* arr,int l,int r){ int n=r-l+1; build5(n); for(int i=size5,m;n>0;){ if(split5[i]<=n){ m=(l+r)/2; rotate5(arr,l+split5[i]/2,m,m+split5[i]/2); circle5(arr,l,l+split5[i]-1,i); l+=split5[i]; n-=split5[i]; } else i--; } }

// --- 题6 摇摆排序 II ---
int first6,last6;
void swap6(int* arr,int i,int j){ int t=arr[i];arr[i]=arr[j];arr[j]=t; }
void partition6(int* arr,int l,int r,int x){ first6=l; last6=r; int i=l; while(i<=last6){ if(arr[i]==x) i++; else if(arr[i]<x){ swap6(arr,first6++,i++);} else { swap6(arr,i,last6--);} } }
int randomizedSelect6(int* arr,int n,int i){ int ans=0; for(int l=0,r=n-1;l<=r;){ partition6(arr,l,r,arr[l+rand()%(r-l+1)]); if(i<first6) r=first6-1; else if(i>last6) l=last6+1; else { ans=arr[i]; break; } } return ans; }
// 复用 5 的 shuffle 全套（start/split 独立数组，命名分开用宏替换即可——直接再写一份）
int start6[64], split6[64], size6;
void swap6b(int* arr,int i,int j){ int t=arr[i];arr[i]=arr[j];arr[j]=t; }
void reverse6(int* arr,int l,int r){ while(l<r) swap6b(arr,l++,r--); }
void rotate6(int* arr,int l,int m,int r){ reverse6(arr,l,m); reverse6(arr,m+1,r); reverse6(arr,l,r); }
int to6(int i,int l,int r){ if(i<=((l+r)>>1)) return i+(i-l+1); else return i-(r-i+1); }
void circle6(int* arr,int l,int r,int i){ for(int j=1,init,cur,next,curv,nextv;j<=i;j++){ init=cur=l+start6[j]-1; next=to6(cur,l,r); curv=arr[cur]; while(next!=init){ nextv=arr[next]; arr[next]=curv; curv=nextv; cur=next; next=to6(cur,l,r);} arr[init]=curv; } }
void build6(int n){ size6=0; for(int s=1,p=2;p<=n;s*=3,p=s*3-1){ start6[++size6]=s; split6[size6]=p; } }
void shuffle6(int* arr,int l,int r){ int n=r-l+1; build6(n); for(int i=size6,m;n>0;){ if(split6[i]<=n){ m=(l+r)/2; rotate6(arr,l+split6[i]/2,m,m+split6[i]/2); circle6(arr,l,l+split6[i]-1,i); l+=split6[i]; n-=split6[i]; } else i--; } }
void wiggleSort6(int* arr,int n){ randomizedSelect6(arr,n,n/2); if((n&1)==0){ shuffle6(arr,0,n-1); reverse6(arr,0,n-1);} else { shuffle6(arr,1,n-1);} }

// ============ 暴力 ============

// 题1 暴力：next_permutation 数名次（1-based，n 小不超 mod）
ll cantorBrute(int n, vector<int> p){
  vector<int> a(n); for(int i=0;i<n;i++) a[i]=i+1;
  ll rank=1;
  do { if(a==p) return rank; rank++; } while(next_permutation(a.begin(),a.end()));
  return -1;
}

// 题2 暴力：全排列列表（0-based），找 s 的 index，输出 index+m（对拍内联实现，此函数不再使用）

// 题3 暴力：vector 模拟（0-based）
int josephBrute(int n,int k){
  vector<int> v(n); for(int i=0;i<n;i++) v[i]=i+1;
  int cur=0;
  while(v.size()>1){
    cur=(cur+k-1)%(int)v.size();
    v.erase(v.begin()+cur);
    // 删除后 cur 指向原下一节点，从 1 重新报
  }
  return v[0];
}

// 题4 暴力：链表模拟（与课程 joseph1 等价，独立写一份）
int joseph4Brute(int n, vector<int> arr4){
  if(n==1) return 1;
  vector<int> nxt(n+1); for(int i=1;i<n;i++) nxt[i]=i+1; nxt[n]=1;
  int pre=n,cur=1;
  for(int i=1;i<n;i++){
    for(int cnt=1;cnt<arr4[i];cnt++){ pre=cur; cur=nxt[cur]; }
    nxt[pre]=nxt[cur]; cur=nxt[cur];
  }
  return cur;
}

// 题5 暴力：独立交叉数组
void shuffleBrute(int* arr,int l,int r){
  int n=r-l+1; vector<int> a(arr+l,arr+r+1);
  vector<int> L(a.begin(),a.begin()+n/2), R(a.begin()+n/2,a.end());
  for(int j=0;j<n/2;j++){ arr[l+2*j]=R[j]; arr[l+2*j+1]=L[j]; }
}

// ============ 对拍主程序 ============

mt19937 rng(20261017);

int main(){
  // ---- 题1 康托展开 ----
  {
    int bad=0, tot=0;
    for(int n=1;n<=8;n++){
      vector<int> a(n); for(int i=0;i<n;i++) a[i]=i+1;
      do {
        n1=n; for(int i=1;i<=n;i++) arr1[i]=a[i-1];
        memset(tree1,0,sizeof(tree1));
        ll got=cantor();
        ll want=cantorBrute(n,a);
        tot++;
        if(got!=want){ bad++; if(bad<=3) printf("T1 MISMATCH n=%d got=%lld want=%lld perm:", n, got, want); }
      } while(next_permutation(a.begin(),a.end()));
    }
    printf("题1 康托展开: %d/%d OK\n", tot-bad, tot);
  }
  // ---- 题2 逆康托展开 ----
  {
    int bad=0, tot=0;
    for(int n=2;n<=6;n++){
      vector<int> aa(n); for(int i=0;i<n;i++) aa[i]=i+1;
      vector<vector<int>> all;
      do { all.push_back(aa); } while(next_permutation(aa.begin(),aa.end()));
      for(int t=0;t<30 && (n<=4 || t<8);t++){
        vector<int> s=aa; shuffle(s.begin(),s.end(),rng);
        int idx=-1;
        for(int i=0;i<(int)all.size();i++) if(all[i]==s){ idx=i; break; }
        // 题目保证 x+m 不超总数：课程 x 是 0-based 排名，m 需满足 idx+m <= size-1
        ll cap = (ll)all.size()-1-idx;
        if (cap < 1) continue;
        ll m = rng() % cap + 1;
        n2=n; m2=m; for(int i=1;i<=n;i++) arr2[i]=s[i-1];
        inverseCantor();
        vector<int> got(n); for(int i=1;i<=n;i++) got[i-1]=(int)arr2[i];
        vector<int> want=all[idx+(int)m];
        tot++;
        if(got!=want){ bad++; if(bad<=3){ printf("T2 MISMATCH n=%d m=%lld\n s=",n,m); for(int x:s)printf("%d ",x); printf("\n got="); for(int x:got)printf("%d ",x); printf("\nwant="); for(int x:want)printf("%d ",x); printf("\n"); } }
      }
    }
    printf("题2 逆康托展开: %d/%d OK\n", tot-bad, tot);
  }
  // ---- 题3 约瑟夫环 ----
  {
    int bad=0, tot=0;
    for(int t=0;t<3000;t++){
      int n=rng()%500+1, k=rng()%500+1;
      int got=joseph(n,k), want=josephBrute(n,k);
      tot++;
      if(got!=want){ bad++; if(bad<=3) printf("T3 MISMATCH n=%d k=%d got=%d want=%d\n",n,k,got,want); }
    }
    // 手算：n=1,k=任意 → 1
    for(int k=1;k<=10;k++){ tot++; if(joseph(1,k)!=1){bad++; printf("T3 n=1 FAIL");} }
    printf("题3 约瑟夫环: %d/%d OK\n", tot-bad, tot);
  }
  // ---- 题4 约瑟夫加强 ----
  {
    int bad=0, tot=0;
    for(int t=0;t<3000;t++){
      int n=rng()%200+1;
      vector<int> arr4(n+1);
      for(int i=1;i<n;i++) arr4[i]=rng()%300+1;
      int got=joseph2(n,&arr4[0]);
      int want=joseph4Brute(n,arr4);
      tot++;
      if(got!=want){ bad++; if(bad<=3) printf("T4 MISMATCH n=%d got=%d want=%d\n",n,got,want); }
    }
    // n=1 边界
    { vector<int> arr4(2); arr4[1]=5; tot++; int g=joseph2(1,&arr4[0]), w=joseph4Brute(1,arr4); if(g!=w){bad++;printf("T4 n=1 FAIL\n");} }
    printf("题4 约瑟夫加强: %d/%d OK\n", tot-bad, tot);
  }
  // ---- 题5 完美洗牌 ----
  {
    int bad=0, tot=0;
    for(int t=0;t<3000;t++){
      int N=rng()%60+2; if(N%2) N--;
      if(N<2) N=2;
      vector<int> a(N), b(N);
      for(int i=0;i<N;i++){ a[i]=rng()%1000; b[i]=a[i]; }
      // 随机 l（保证 r-l+1 偶、l>=0、r<N）
      int len=2*(rng()%(N/2))+2;
      int l=rng()% (N-len+1);
      int r=l+len-1;
      shuffle2(&a[0],l,r);
      shuffleBrute(&b[0],l,r);
      tot++;
      if(a!=b){ bad++; if(bad<=3){ printf("T5 MISMATCH N=%d l=%d r=%d\n",N,l,r); } }
    }
    printf("题5 完美洗牌: %d/%d OK\n", tot-bad, tot);
  }
  // ---- 题6 摇摆排序 II ----
  {
    int bad=0, tot=0;
    // 有解输入的构造：排序 → 小半逆序放偶位、大半逆序放奇位（标准交叉），再随机打乱。
    // 打乱后的数组必然有解（原交叉排列即解）。值域选大避免交叉时相邻相等。
    for(int t=0;t<2000;t++){
      int N=rng()%60+1;
      int range = (t%2==0)?6:200; // 一半小值域（多重复），一半大值域
      vector<int> sorted(N); for(int i=0;i<N;i++) sorted[i]=rng()%range;
      sort(sorted.begin(), sorted.end());
      // 标准交叉：小半（前 (N+1)/2 个）逆序放偶数位，大半（后 N/2 个）逆序放奇数位
      int small=(N+1)/2;
      vector<int> w(N);
      int si=small-1, bi=N-1;
      for(int pos=0; pos<N; pos+=2) w[pos]=sorted[si--];
      for(int pos=1; pos<N; pos+=2) w[pos]=sorted[bi--];
      // 若交叉后仍有相邻相等（重复太多），跳过（该输入无解或难解，不测）
      bool okWiggle=true;
      for(int i=0;i<N-1;i++){ if(i%2==0 && !(w[i]<w[i+1])) okWiggle=false; if(i%2==1 && !(w[i]>w[i+1])) okWiggle=false; }
      if(!okWiggle) continue;
      // 随机打乱 w 作为输入
      vector<int> a=w; shuffle(a.begin(), a.end(), rng);
      vector<int> b=a;
      wiggleSort6(&b[0],N);
      bool ok=true;
      for(int i=0;i<N-1;i++){
        if(i%2==0){ if(!(b[i]<b[i+1])) ok=false; }
        else { if(!(b[i]>b[i+1])) ok=false; }
      }
      tot++;
      if(!ok){ bad++; if(bad<=3){ printf("T6 FAIL N=%d arr=",N); for(int x:a)printf("%d ",x); printf("\n out="); for(int x:b)printf("%d ",x); printf("\n"); } }
    }
    // 确定性边界：重复中位数
    { vector<int> a={1,1,1,1,2,2,2}; vector<int> b=a; wiggleSort6(&b[0],7); bool ok=true; for(int i=0;i<6;i++){ if(i%2==0 && !(b[i]<b[i+1])) ok=false; if(i%2==1 && !(b[i]>b[i+1])) ok=false; } tot++; if(!ok){ bad++; printf("T6 BOUND1 FAIL 1x4+2x3 out="); for(int x:b)printf("%d ",x); printf("\n"); } }
    { vector<int> a={1,1,1,1,2,2,2,2}; vector<int> b=a; wiggleSort6(&b[0],8); bool ok=true; for(int i=0;i<7;i++){ if(i%2==0 && !(b[i]<b[i+1])) ok=false; if(i%2==1 && !(b[i]>b[i+1])) ok=false; } tot++; if(!ok){ bad++; printf("T6 BOUND2 FAIL 1x4+2x4 out="); for(int x:b)printf("%d ",x); printf("\n"); } }
    printf("题6 摇摆排序II: %d/%d OK\n", tot-bad, tot);
  }
  printf("=== 全过 ===\n");
  return 0;
}
