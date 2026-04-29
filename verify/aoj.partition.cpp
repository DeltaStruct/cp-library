// competitive-verifier: PROBLEM https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3086
#include "assets/stdc++.hpp"
#include "sequence/sparse_table.hpp"
#include "monge/simple_larsch.hpp"
#define int long long

signed main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,m; cin >> n >> m;
  vector<int> A(n); for (int& a:A) cin >> a;
  sparse_table segtree(A,guide(merge)(
    [](int a,int b){ return max(a,b); }
  ));
  vector R = simple_larsch(n,guide(value,select,id,init)(
    [&](int i,int& x,int k){ return x+(i-k<m?(int)-1e9:segtree.query(k,i)); },
    [&](int i,int& x,int k,int& y,int j){ return x>=y+(i-j<m?(int)-1e9:segtree.query(j,i)); },
    single(0ll),
    single((int)-1e18)
  )).dist;
  cout << R.back() << '\n';
}