// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/unionfind
#include "assets/stdc++.hpp"
#include "graph/union_find.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  union_find UF(n);
  while(q--){
    int t,a,b; cin >> t >> a >> b;
    if (t==0) UF.connect(a,b);
    else cout << UF.connected(a,b) << '\n';
  }
}