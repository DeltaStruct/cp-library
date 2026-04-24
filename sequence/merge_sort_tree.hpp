#include "assets/stdc++.hpp"

template<typename T,class F>
requires invocable_r(bool,comp,T&,T&)
struct merge_sort_tree {
  int n;
  vector<vector<T>> segtree;
  F f;
  merge_sort_tree(int _n,F _f) : n(_n),segtree(2*n),f(_f) {}
  void build(){
    for (int i(n-1);i;--i) rngs::merge(
      segtree[i<<1],
      segtree[i<<1|1],
      back_inserter(segtree[i]),
      memfn(&comp,&f)
    );
  }
};