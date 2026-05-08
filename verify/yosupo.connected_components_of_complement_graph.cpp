// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/connected_components_of_complement_graph
#include "assets/stdc++.hpp"
#include "graph/connected_components_complement_graph.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,m; cin >> n >> m;
  connected_components_complement_graph UF(n);
  for (int i(0),a,b;i < m;++i) (cin>>a>>b),UF.add_edge(a,b);
  UF.build();
  int s = UF.count_components();
  cout << s << '\n';
  for (int i(0);i < s;++i){
    vector R = UF.components(i);
    cout << R.size() << ' ';
    for (int a:R) cout << a << ' ';
    cout << '\n';
  }
}