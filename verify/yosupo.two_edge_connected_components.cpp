// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/two_edge_connected_components
#include "assets/stdc++.hpp"
#include "graph/two_edge_connected_components.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,m; cin >> n >> m;
  graph G(n);
  for (int i(0),a,b;i < m;++i) (cin>>a>>b),G.emplace_undirected(a,b);
  low_link L(G);
  two_edge_connected_components T(L);
  cout << T.size() << '\n';
  for (int i(0);i < (int)T.size();++i){
    vector R = T.vertex_set(i);
    cout << R.size() << ' ';
    for (int a:R) cout << a << ' ';
    cout << '\n';
  }
}