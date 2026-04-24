// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum
#include "assets/stdc++.hpp"
#include "graph/offline_dynamic_connectivity.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  vector<long long> A(n); for (auto& a:A) cin >> a;
  offline_dynamic_connectivity B(A,q,guide(merge,split,add)(
    [&](long long& x,long long& y){ x += y; },
    [&](long long& x,long long& y){ x -= y; },
    [&](long long& x,int y){ x += y; }
  ));
  map<pair<int,int>,int> E;
  vector<tuple<int,int,int>> Q(q);
  for (int i(0);i < q;++i){
    auto& [t,a,b] = Q[i]; b = -1;
    cin >> t >> a;
    if (t!=3) cin >> b;
    if (t<2&&a>b) swap(a,b);
    if (t==0) E[make_pair(a,b)] = i;
    if (t==1) B.emplace(E[make_pair(a,b)],i+1,a,b),E.erase(make_pair(a,b));
  }
  for (auto [a,b]:E) B.emplace(b,q,a);
  for (int i:B){
    auto [t,a,b] = Q[i];
    if (t==2) B.add(a,b);
    if (t==3) cout << B.value(B.leader(a)) << '\n';
  }
}