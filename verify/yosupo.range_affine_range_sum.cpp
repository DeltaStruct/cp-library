// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_affine_range_sum
#include "assets/stdc++.hpp"
#include "sequence/lazy_segment_tree.hpp"

struct modint {
  int v;
  modint() : v() {}
  modint(int _v) : v(_v) {}
  modint operator+(modint x){
    return modint(v+x.v-(v+x.v>998244352?998244353:0));
  }
  modint operator*(modint x){
    return modint(v*(long long)x.v%998244353);
  }
};

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  vector<int> A(n); for (auto& a:A) cin >> a;
  lazy_segment_tree segtree(n,guide(merge,transform,composition)(
    [](pair<modint,int> a,pair<modint,int> b){
      return make_pair(a.first+b.first,a.second+b.second);
    },
    [](pair<modint,int> a,pair<modint,modint> b){
      return make_pair(a.first*b.first+b.second*a.second,a.second);
    },
    [](pair<modint,modint> a,pair<modint,modint> b){
      return make_pair(a.first*b.first,a.second*b.first+b.second);
    }
  ));
  for (int i(0);i < n;++i) segtree[i] = make_pair(modint(A[i]),1);
  segtree.build();
  while(q--){
    int t,l,r; cin >> t >> l >> r;
    if (t==0){
      int x,y; cin >> x >> y;
      segtree.apply(l,r,make_pair(x,y));
    } else {
      cout << segtree.query(l,r).first.v << '\n';
    }
  }
}