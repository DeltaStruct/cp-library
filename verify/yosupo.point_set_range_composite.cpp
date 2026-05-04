// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_set_range_composite
#include "assets/stdc++.hpp"
#include "sequence/segment_tree.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  vector<pair<long long,long long>> A(n); for (auto& [a,b]:A) cin >> a >> b;
  constexpr long long mod = 998244353;
  segment_tree segtree(A,guide(merge)(
    [&](pair<long long,long long>& x,pair<long long,long long>& y){
      return make_pair(x.first*y.first%mod,(x.second*y.first+y.second)%mod);
    }
  ));
  while(q--){
    int t; cin >> t;
    if (t==0){
      int a,b,c; cin >> a >> b >> c;
      segtree.set(a,make_pair(b,c));
    } else {
      int l,r,x; cin >> l >> r >> x;
      auto [a,b] = segtree.query(l,r);
      cout << (a*x+b)%mod << '\n';
    }
  }
}