#pragma once
#include "assets/stdc++.hpp"
#include "sequence/runtime_array.hpp"

template<typename T,class F>
requires invocable_r(T,merge,F,T&,T&)
struct segment_tree {
  int n;
  runtime_array<T> segtree;
  F f;
  segment_tree(int _n,F _f) : n(_n),segtree(2*n),f(std::move(_f)) {
    for (int i(0);i < n;++i){
      if constexpr (invocable_r(T,idi,F,int)) segtree[n+i] = f.idi(i);
      else segtree[n+i] = f.id();
    }
    build();
  }
  template<input_iterator I>
  segment_tree(I a,I b,F _f) : n((int)distance(a,b)),segtree(2*n),f(std::move(_f)) {
    copy(a,b,segtree.begin()+n);
    build();
  }
  template<rngs::range C>
  segment_tree(C&& A,F _f) : n((int)rngs::distance(A)),segtree(2*n),f(std::move(_f)) {
    rngs::copy(A,segtree.begin()+n);
    build();
  }
  void build(){
    for (int i(n-1);i;--i) segtree[i] = f.merge(segtree[i<<1],segtree[i<<1|1]);
  }
  T& operator[](int x){
    return segtree[n+x];
  }
  void update(int x){
    for ((x+=n)>>=1;x;x>>=1) segtree[x] = f.merge(segtree[x<<1],segtree[x<<1|1]);
  }
  T& set(int x,const T& y){
    segtree[n+x] = y;
    update(x);
    return segtree[n+x];
  }
  T& update(int x,const T& y) requires invocable_r(T,update,F,T&,T&) {
    segtree[n+x] = p(segtree[n+x],y);
    update(x);
    return segtree[n+x];
  }
  template<class P>
  requires invocable_r<T,P,T&,T&>
  T& update(int x,const P& p = P()){
    segtree[n+x] = p(segtree[n+x]);
    update(x);
    return segtree[n+x];
  }
  T query(int l,int r){
    if constexpr (invocable_r(T,id,F)) if (l>=r) return f.id();
    assert(0<=l&&l<r&&r<=n);
    if (l+1==r) return segtree[n+l];
    T resl = segtree[n+l],resr = segtree[n+r-1];
    for (l+=n+1,r+=n-1;l < r;l>>=1,r>>=1){
      if (l&1) resl = f.merge(resl,segtree[l++]);
      if (r&1) resr = f.merge(segtree[--r],resr);
    }
    return f.merge(resl,resr);
  }
  T* internal(){
    return segtree;
  }
  using iterator = T*;
  iterator begin(){
    return segtree+n;
  }
  iterator end(){
    return segtree+2*n;
  }
  r_iter_set;
};

template<class F>
segment_tree(int,F) -> segment_tree<return_type(merge,F),F>;

template<input_iterator I,class F>
segment_tree(I,I,F) -> segment_tree<iter_value_t<I>,F>;

template<rngs::range C,class F>
segment_tree(C&&,F) -> segment_tree<rngs::range_value_t<C>,F>;