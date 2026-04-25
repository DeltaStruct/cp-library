// competitive-verifier: LOCALCASE //verify/localcase/joi/voltage
#include "assets/stdc++.hpp"
#include "graph/offline_dynamic_bipartite_connectivity.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,m; cin >> n >> m;
  offline_dynamic_bipartite_connectivity UF(n,m);
  vector<pair<int,int>> A(m);
  for (int i(0);i < m;++i){
    int a,b; cin >> a >> b; --a,--b,A[i] = make_pair(a,b);
    UF.emplace(0,i,a,b),UF.emplace(i+1,m,a,b);
  }
  int res = 0;
  for (int i:UF) res += UF.bipartite()&&UF.same_color(A[i].first,A[i].second);
  cout << res << endl;
}