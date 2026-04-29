// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/staticrmq
#include "assets/stdc++.hpp"
#include "sequence/sparse_table.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  vector<int> A(n); for (int& a:A) cin >> a;
  sparse_table segtree(A,guide(merge)(
    [&](int& a,int& b){ return min(a,b); }
  ));
  while(q--){
    int l,r; cin >> l >> r;
    cout << segtree.query(l,r) << '\n';
  }
}