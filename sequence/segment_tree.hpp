#pragma once
#include "assets/stdc++.hpp"

template<typename T,class F>
struct segment_tree {
  int n;
  vector<T> segtree;
  F f;
  cex segment_tree(F _f) : n(0),segtree(0),f(std::move(_f)) {}
  cex segment_tree(int _n,F _f) : n(_n),segtree(2*n),f(std::move(_f)) {
    for (int i(0);i < n;++i){
      if cex (invocable_r(T,idi,F,int)) segtree[n+i] = f.idi(i);
      else segtree[n+i] = f.id();
    }
    build();
  }
  template<input_iterator I>
  cex segment_tree(I x,I y,F _f) : n((int)distance(x,y)),segtree(2*n),f(std::move(_f)) {
    copy(x,y,segtree.begin()+n);
    build();
  }
  template<rngs::range C>
  cex segment_tree(C&& A,F _f) : n((int)rngs::distance(A)),segtree(2*n),f(std::move(_f)) {
    rngs::copy(A,segtree.begin()+n);
    build();
  }
  cex void build(){
    for (int i(n-1);i;--i) segtree[i] = f.merge(segtree[i<<1],segtree[i<<1|1]);
  }
  cex T& operator[](int x){
    return segtree[n+x];
  }
  cex T* data(){
    return segtree.data()+n;
  }
  cex void update(int x){
    for ((x+=n)>>=1;x;x>>=1) segtree[x] = f.merge(segtree[x<<1],segtree[x<<1|1]);
  }
  cex T& set(int x,const T& y){
    segtree[n+x] = y;
    update(x);
    return segtree[n+x];
  }
  cex T& update(int x,const T& y) requires invocable_r(T,update,F,T&,T&) {
    segtree[n+x] = p(segtree[n+x],y);
    update(x);
    return segtree[n+x];
  }
  template<class P>
  requires invocable_r<T,P,T&,T&>
  cex T& update(int x,const P& p = P()){
    segtree[n+x] = p(segtree[n+x]);
    update(x);
    return segtree[n+x];
  }
  cex T query(int l,int r){
    T resl = f.id(),resr = f.id();
    for (l+=n,r+=n;l < r;l>>=1,r>>=1){
      if (l&1) resl = f.merge(resl,segtree[l++]);
      if (r&1) resr = f.merge(segtree[--r],resr);
    }
    return f.merge(resl,resr);
  }
  cex vector<T>& internal(){
    return segtree;
  }
  using iterator = typename vector<T>::iterator;
  cex iterator begin(){
    return segtree.begin()+n;
  }
  cex iterator end(){
    return segtree.end();
  }
  r_iter_set;
};

template<class F>
segment_tree(int,const F&) -> segment_tree<return_type(merge,F),F>;

template<input_iterator I,class F>
segment_tree(I,I,const F&) -> segment_tree<iter_value_t<I>,F>;

template<rngs::range C,class F>
segment_tree(C&&,const F&) -> segment_tree<rngs::range_value_t<C>,F>;