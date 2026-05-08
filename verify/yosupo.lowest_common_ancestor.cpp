// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/lca
#include "tree/tree.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  rooted_tree G(n,0);
  for (int i(1),a;i < n;++i) (cin>>a),G.emplace_undirected(i,a);
  G.build();
  while(q--){
    int a,b; cin >> a >> b;
    cout << G.lca(a,b) << '\n';
  }
}