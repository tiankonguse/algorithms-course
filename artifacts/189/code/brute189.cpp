#include <bits/stdc++.h>
using namespace std;
// 用法: brute189.run <题号1..10>，读对应格式的输入，按题面输出
// 核心：Floyd 可达性 + SCC 集合 + 缩点度数/点权

static int n;
static vector<vector<bool>> R; // 可达矩阵

static vector<vector<int>> sccs;    // 每个分量内的点
static vector<int> belong;          // 点 -> 分量下标(0-based)
static vector<vector<bool>> C;      // 缩点图可达（分量间）

static void buildSCC(){
    // R 已算好（含自环）
    vector<bool> used(n+1,false);
    sccs.clear();
    belong.assign(n+1,-1);
    for(int i=1;i<=n;i++){
        if(used[i]) continue;
        // 分量 = 与 i 双向可达的点
        vector<int> g;
        for(int j=1;j<=n;j++){
            if(!used[j] && R[i][j] && R[j][i]) g.push_back(j);
        }
        for(int x:g){ used[x]=true; belong[x]=sccs.size(); }
        sort(g.begin(),g.end());
        sccs.push_back(g);
    }
    // 缩点图可达：分量 a 到 b（a!=b）存在原边
    int k=sccs.size();
    C.assign(k, vector<bool>(k,false));
    for(int a=0;a<k;a++){
        for(int b=0;b<k;b++){
            if(a==b) continue;
            for(int x:sccs[a]){
                for(int y:sccs[b]){
                    if(R[x][y]){ C[a][b]=true; break; }
                }
                if(C[a][b]) break;
            }
        }
    }
}

static void calcReach(const vector<pair<int,int>>& edges, int nn){
    n=nn;
    R.assign(n+1, vector<bool>(n+1,false));
    for(int i=1;i<=n;i++) R[i][i]=true;
    for(auto&e:edges) R[e.first][e.second]=true;
    for(int k2=1;k2<=n;k2++)
        for(int i=1;i<=n;i++) if(R[i][k2])
            for(int j=1;j<=n;j++) if(R[k2][j]) R[i][j]=true;
    buildSCC();
}

static int in0cnt(){ // 入度为 0 的分量数
    int k=sccs.size(), cnt=0;
    for(int a=0;a<k;a++){
        bool has=false;
        for(int b=0;b<k;b++) if(C[b][a]){ has=true; break; }
        if(!has) cnt++;
    }
    return cnt;
}
static int out0cnt(){
    int k=sccs.size(), cnt=0;
    for(int a=0;a<k;a++){
        bool has=false;
        for(int b=0;b<k;b++) if(C[a][b]){ has=true; break; }
        if(!has) cnt++;
    }
    return cnt;
}

int main(int argc, char** argv){
    int which = atoi(argv[1]);
    ios::sync_with_stdio(false); cin.tie(nullptr);
    if(which==1||which==2||which==3){
        if(which==3){
            int m; cin>>n>>m;
            vector<pair<int,int>> es;
            for(int i=0;i<m;i++){
                int a,b,t; cin>>a>>b>>t;
                es.push_back({a,b});
                if(t==2) es.push_back({b,a});
            }
            calcReach(es,n);
            int best=0;
            for(auto&g:sccs) best=max(best,(int)g.size());
            cout<<best<<"\n";
            for(int i=1;i<=n;i++){
                if((int)sccs[belong[i]].size()==best){
                    for(int x:sccs[belong[i]]) cout<<x<<" ";
                    break;
                }
            }
            cout<<"\n";
            return 0;
        }
        int m; cin>>n>>m;
        vector<pair<int,int>> es(m);
        for(auto&e:es){ cin>>e.first>>e.second; }
        calcReach(es,n);
        if(which==1){
            cout<<sccs.size()<<"\n";
            vector<bool> printed(sccs.size(),false);
            for(int i=1;i<=n;i++){
                int g=belong[i];
                if(printed[g]) continue;
                printed[g]=true;
                for(int x:sccs[g]) cout<<x<<" ";
                cout<<"\n";
            }
        } else {
            cout<<sccs.size()<<"\n";
            for(auto&g:sccs){
                cout<<g.size()<<" ";
                for(int x:g) cout<<x<<" ";
                cout<<"\n";
            }
        }
        return 0;
    }
    if(which==4||which==5||which==6){
        if(which==6){
            cin>>n;
            vector<pair<int,int>> es;
            for(int i=1;i<=n;i++){
                int x; cin>>x;
                while(x!=0){ es.push_back({i,x}); cin>>x; }
            }
            calcReach(es,n);
            if(sccs.size()==1){ cout<<"1\n0\n"; return 0; }
            cout<<in0cnt()<<"\n"<<max(in0cnt(),out0cnt())<<"\n";
            return 0;
        }
        int m; cin>>n>>m;
        vector<pair<int,int>> es(m);
        for(auto&e:es) cin>>e.first>>e.second;
        calcReach(es,n);
        if(which==4){
            // 出度为 0 的分量唯一 → 大小，否则 0
            int k=sccs.size(), cnt=0, siz=0;
            for(int a=0;a<k;a++){
                bool has=false;
                for(int b=0;b<k;b++) if(C[a][b]){ has=true; break; }
                if(!has){ cnt++; siz=sccs[a].size(); }
            }
            cout<<(cnt==1?siz:0)<<"\n";
        } else {
            cout<<in0cnt()<<"\n";
        }
        return 0;
    }
    if(which==7){
        int p; cin>>n>>p;
        vector<int> cost(n+1, INT_MAX);
        for(int i=0;i<p;i++){ int u,c; cin>>u>>c; cost[u]=c; }
        int m; cin>>m;
        vector<pair<int,int>> es(m);
        for(auto&e:es) cin>>e.first>>e.second;
        calcReach(es,n);
        // 覆盖判定：从所有可贿赂点出发 BFS
        vector<bool> vis(n+1,false);
        queue<int> q;
        for(int i=1;i<=n;i++) if(cost[i]!=INT_MAX){ vis[i]=true; q.push(i); }
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(auto&e:es) if(e.first==u && !vis[e.second]){ vis[e.second]=true; q.push(e.second); }
        }
        int noAns=-1;
        for(int i=1;i<=n;i++) if(!vis[i]){ noAns=i; break; }
        if(noAns!=-1){ cout<<"NO\n"<<noAns<<"\n"; return 0; }
        long long sum=0;
        for(int a=0;a<(int)sccs.size();a++){
            bool has=false;
            for(int b=0;b<(int)sccs.size();b++) if(C[b][a]){ has=true; break; }
            if(!has){
                int mn=INT_MAX;
                for(int x:sccs[a]) mn=min(mn,cost[x]);
                sum+=mn;
            }
        }
        cout<<"YES\n"<<sum<<"\n";
        return 0;
    }
    if(which==8){
        cin>>n;
        vector<int> val(n+1);
        for(int i=1;i<=n;i++) cin>>val[i];
        int m; cin>>m;
        vector<pair<int,int>> es(m);
        for(auto&e:es) cin>>e.first>>e.second;
        calcReach(es,n);
        long long s1=0, s2=1;
        for(auto&g:sccs){
            int mn=INT_MAX, cnt=0;
            for(int x:g){ if(val[x]<mn){ mn=val[x]; cnt=1; } else if(val[x]==mn) cnt++; }
            s1+=mn;
            s2=s2*cnt%1000000007LL;
        }
        cout<<s1<<" "<<s2<<"\n";
        return 0;
    }
    if(which==9){
        int t; cin>>t;
        while(t--){
            int m; cin>>n>>m;
            vector<pair<int,int>> es(m);
            for(auto&e:es) cin>>e.first>>e.second;
            calcReach(es,n);
            bool ok=true;
            int k=sccs.size();
            // 任意两点 u,v：u 可达 v 或 v 可达 u（缩点图上）
            for(int a=0;a<k && ok;a++) for(int b=a+1;b<k;b++){
                if(!C[a][b] && !C[b][a]){ ok=false; break; }
            }
            cout<<(ok?"Yes":"No")<<"\n";
        }
        return 0;
    }
    if(which==10){
        // 名字：婚姻女→男，暧昧男→女；夫妻两点同分量 → Unsafe
        int npair; cin>>npair;
        unordered_map<string,int> id;
        vector<pair<int,int>> couple(npair+1);
        vector<pair<int,int>> es;
        int tot=0;
        auto get=[&](const string&s)->int{
            auto it=id.find(s);
            if(it!=id.end()) return it->second;
            id[s]=++tot; return tot;
        };
        for(int i=1;i<=npair;i++){
            string g,b; cin>>g>>b;
            int gi=get(g), bi=get(b);
            couple[i]={gi,bi};
            es.push_back({gi,bi});
        }
        int m; cin>>m;
        for(int i=0;i<m;i++){
            string g,b; cin>>g>>b;
            es.push_back({get(b),get(g)});
        }
        n=tot;
        calcReach(es,n);
        for(int i=1;i<=npair;i++){
            cout<<(belong[couple[i].first]==belong[couple[i].second]?"Unsafe":"Safe")<<"\n";
        }
        return 0;
    }
    return 0;
}
