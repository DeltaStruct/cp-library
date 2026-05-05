// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
#include "assets/stdc++.hpp"
#include "bbst/splay_tree.hpp"
#define int long long

signed main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int mod = 998244353;
  splay_tree tree(guide(merge,composition,transform,lid,skip_lazy_update,tag)(
    [&](pair<int,int> a,pair<int,int> b){
      return make_pair((a.first+b.first)%mod,a.second+b.second);
    },
    [&](pair<int,int> a,pair<int,int> b){
      return make_pair(a.first*b.first%mod,(a.second*b.first+b.second)%mod);
    },
    [&](pair<int,int> a,pair<int,int> b){
      return make_pair((a.first*b.first+a.second*b.second)%mod,a.second);
    },
    single(make_pair(1ll,0ll)),
    [](pair<int,int> a){
      return a==make_pair(1ll,0ll);
    },
    bbst_reversible_tag|bbst_lazy_tag|bbst_size_tag
  ));
  auto root = tree.make_dummy();
  int n,q; cin >> n >> q;
  vector<int> A(n); for (int& a:A) (cin>>a),root = tree.emplace_back(root,a,1);
  while(q--){
    int t; cin >> t;
    if (t==0){
      int x,y; cin >> x >> y;
      root = tree.emplace_at(root,x,y,1);
    }
    if (t==1){
      int x; cin >> x;
      root = tree.erase_at(root,x);
    }
    if (t==2){
      int l,r; cin >> l >> r;
      auto [a,b,c] = tree.between(root,l,r);
      tree.reverse(b);
      root = tree.merge(a,b,c);
    }
    if (t==3){
      int l,r,x,y; cin >> l >> r >> x >> y;
      auto [a,b,c] = tree.between(root,l,r);
      tree.lazy_update(b,make_pair(x,y));
      root = tree.merge(a,b,c);
    }
    if (t==4){
      int l,r; cin >> l >> r;
      auto [a,b,c] = tree.between(root,l,r);
      cout << tree.sum(b).first << '\n';
      root = tree.merge(a,b,c);
    }
  }
}