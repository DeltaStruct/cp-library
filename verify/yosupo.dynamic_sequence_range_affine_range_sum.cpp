#include "assets/stdc++.hpp"
#include "bbst/rbst.hpp"
#define int long long

signed main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int mod = 998244353;
  lazy_rbst tree(guide(merge,push,apply,lazy_id)(
    [&](pair<int,int> a,pair<int,int> b){
      return make_pair((a.first+b.first)%mod,a.second+b.second);
    },
    [&](pair<int,int> a,pair<int,int> b){
      return make_pair(a.first*b.first%mod,(a.second*b.first+b.second)%mod);
    },
    [&](pair<int,int> a,pair<int,int> b){
      return make_pair((a.first*b.first+a.second*b.second)%mod,a.second);
    },
    single(make_pair(1,0))
  ));
  auto root = tree.dummy();
  int n,q; cin >> n >> q;
  vector<int> A(n); for (int& a:A) (cin>>a),root = tree.merge(root,tree.create(make_pair(a,1)));
  while(q--){
    int t; cin >> t;
    if (t==0){
      int x,y; cin >> x >> y;
      auto [a,b] = tree.front(root,x);
      root = tree.merge(a,tree.create(make_pair(y,1)),b);
    }
    if (t==1){
      int x; cin >> x;
      auto [a,b,c] = tree.range(root,x,x+1);
      root = tree.merge(a,c);
    }
    if (t==2){
      int l,r; cin >> l >> r;
      auto [a,b,c] = tree.range(root,l,r);
      tree.reverse(b);
      root = tree.merge(a,b,c);
    }
    if (t==3){
      int l,r,x,y; cin >> l >> r >> x >> y;
      auto [a,b,c] = tree.range(root,l,r);
      tree.update(b,make_pair(x,y));
      root = tree.merge(a,b,c);
    }
    if (t==4){
      int l,r; cin >> l >> r;
      auto [a,b,c] = tree.range(root,l,r);
      cout << tree.sum(b).first << '\n';
      root = tree.merge(a,b,c);
    }
  }
}