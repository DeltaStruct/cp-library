// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_range_inversions_query
#include "query/mo.hpp"
#include "sequence/segment_tree.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  vector<int> A(n); for (int& a:A) cin >> a;
  A = zaatu(A);
  long long res = 0;
  segment_tree segtree(n,guide(id,merge)(single(0ll),[](long long a,long long b){ return a+b; }));
  mo M(n,q);
  while(q--){
    int l,r; cin >> l >> r;
    M.add(l,r);
  }
  auto g = guide(add_left,add_right,erase_left,erase_right,query)(
    [&](int i){ res += segtree.query(0,A[i]),segtree.set(A[i],segtree[A[i]]+1); },
    [&](int i){ res += segtree.query(A[i]+1,n),segtree.set(A[i],segtree[A[i]]+1); },
    [&](int i){ res -= segtree.query(0,A[i]),segtree.set(A[i],segtree[A[i]]-1); },
    [&](int i){ res -= segtree.query(A[i]+1,n),segtree.set(A[i],segtree[A[i]]-1); },
    [&](int i){ return res; }
  );
  for (auto a:M.exec(g)) cout << a << '\n';
}