// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_reverse_range_sum
#include "assets/stdc++.hpp"
#include "bbst/splay_tree.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  splay_tree tree(guide(merge,tag)(
    [](long long a,long long b){ return a+b; },
    bbst_sum_tag|bbst_reversible_tag|bbst_size_tag
  ));
  auto root = tree.make_dummy();
  int n,q; cin >> n >> q;
  vector<long long> A(n); for (auto& a:A) (cin>>a),tree.emplace_back(root,a);
  while(q--){
    int t,l,r; cin >> t >> l >> r;
    auto [a,b,c] = tree.between(l,r);
    if (t==0) tree.reverse(b);
    else cout << tree.sum(b) << '\n';
    root = tree.merge(a,b,c);
  }
}