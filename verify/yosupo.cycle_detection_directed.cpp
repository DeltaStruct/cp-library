// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/cycle_detection
#include "assets/stdc++.hpp"
#include "graph/cycle_detection.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,m; cin >> n >> m;
  graph G(n);
  for (int i(0),a,b;i < m;++i) (cin>>a>>b),G.emplace_directed(a,b);
  vector ret = cycle_detection_directed(G);
  if (ret.empty()) cout << -1 << '\n';
  else {
    cout << ret.size() << '\n';
    for (int a:ret) cout << a << '\n';
  }
}