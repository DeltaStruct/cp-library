#pragma once
#include "assets/stdc++.hpp"
#include "bit/bit_log.hpp"
#include "sequence/runtime_array.hpp"

template<typename U,class F>
concept lazysegtree_use_lid = requires(F f){ { f.lid() } -> convertible_to<U>; };

template<typename U,class F,bool UseId = lazysegtree_use_lid<U,F>>
struct lazy_segtree_lid {
  static constexpr bool lid = true;
  lazy_segtree_lid(int _n){}
};

template<typename U,class F>
struct lazy_segtree_lid<U,F,false> {
  static constexpr bool lid = false;
  runtime_array<unsigned long long> B;
  lazy_segtree_lid(int _n) : B((_n+63)>>6) {}
  bool flag(int x){
    return B[x>>6]&(1ull<<(x&63));
  }
  void flip(int x){
    B[x>>6] ^= 1ull<<(x&63);
  }
};

template<typename T,typename U,class F>
concept lazy_segtree_c = invocable_r(T,transform,F,T&,U&)&&invocable_r(U,composition,F,U&,U&);

template<typename T,typename U,class F>
requires invocable_r(T,merge,F,T&,T&)&&lazy_segtree_c<T,U,F>
struct lazy_segment_tree : lazy_segtree_lid<U,F> {
  using base = lazy_segtree_lid<U,F>;
  int n;
  runtime_array<T> segtree;
  runtime_array<U> lazy;
  F f;
  lazy_segment_tree(int _n,F _f) : base(_n),n(_n),segtree(2*n),lazy(n),f(std::move(_f)) {
    if constexpr (invocable_r(T,idi,F,int)) for (int i(0);i < n;++i) segtree[n+i] = f.idi(i);
    else if constexpr (invocable_r(T,id,F)) segtree.fill(f.id()),build();
    build();
    if constexpr (invocable_r(U,lid,F)) lazy.fill(f.lid());
  }
  template<input_iterator I>
  lazy_segment_tree(I a,I b,F _f) : base((int)distance(a,b)),n((int)distance(a,b)),segtree(2*n),lazy(n),f(std::move(_f)) {
    copy(a,b,segtree.begin()+n);
    build();
  }
  template<rngs::range C>
  lazy_segment_tree(C&& A,F _f) : base((int)rngs::distance(A)),n((int)rngs::distance(A)),segtree(2*n),lazy(n),f(std::move(_f)) {
    rngs::copy(A,segtree.begin()+n);
    build();
  }
  void build(){
    for (int i(n-1);i;--i) segtree[i] = f.merge(segtree[i<<1],segtree[i<<1|1]);
  }
  T& operator[](int x){
    assert(0<=x&&x<n);
    for (int i(bit_floor_log(n+x));i;--i) lpush((n+x)>>i);
    return segtree[n+x];
  }
  void lupdate(int i,U x){
    segtree[i] = f.transform(segtree[i],x);
    if (i<n){
      if constexpr (base::lid&&invocable_r(bool,skip_lazy_update,F,U&)){
        if (f.skip_lazy_update(lazy[i])){
          lazy[i] = x;
          return;
        }
      } else if constexpr (!base::lid){
        if (!base::flag(i)){
          lazy[i] = x;
          base::flip(i);
          return;
        }
      }
      lazy[i] = f.composition(lazy[i],x);
    }
  }
  void lpush(int i){
    if (i==0) return;
    if constexpr (base::lid&&invocable_r(bool,skip_lazy_update,F,U&)){
      if (f.skip_lazy_update(lazy[i])) return;
    }
    if constexpr (!base::lid){
      if (!base::flag(i)) return;
    }
    lupdate(i<<1,lazy[i]);
    lupdate(i<<1|1,lazy[i]);
    if constexpr (base::lid) lazy[i] = f.lid();
    else base::flip(i);
  }
  void iupdate(int x){
    for (;x;x>>=1) segtree[x] = f.merge(segtree[x<<1],segtree[x<<1|1]);
  }
  void update(int x){
    for ((x+=n)>>=1;x;x>>=1) segtree[x] = f.merge(segtree[x<<1],segtree[x<<1|1]);
  }
  T& set(int x,const T& y){
    assert(0<=x&&x<n);
    for (int i(bit_floor_log(n+x));i;--i) lpush((n+x)>>i);
    segtree[n+x] = y;
    update(x);
    return segtree[n+x];
  }
  T& update(int x,const T& y) requires invocable_r(T,update,F,T&,T&) {
    assert(0<=x&&x<n);
    for (int i(bit_floor_log(n+x));i;--i) lpush((n+x)>>i);
    segtree[n+x] = f.update(segtree[n+x],y);
    update(x);
    return segtree[n+x];
  }
  template<class P>
  requires invocable_r<T,P,T&,T&>
  T& update(int x,const P& p = P()){
    assert(0<=x&&x<n);
    for (int i(bit_floor_log(n+x));i;--i) lpush((n+x)>>i);
    segtree[n+x] = p(segtree[n+x]);
    update(x);
    return segtree[n+x];
  }
  void apply(int l,int r,const U& u){
    assert(0<=l&&l<=r&&r<=n);
    if (l==r) return;
    int log = bit_floor_log(n+n-1);
    l += n,r += n-1;
    for (int i(log),s(countr_zero((unsigned int)l));i > s;--i) lpush(l>>i);
    for (int i(log),s(countr_zero((unsigned int)r+1));i > s;--i) lpush(r>>i);
    for (int ll(l),rr(r+1);ll < rr;ll>>=1,rr>>=1){
      if (ll&1) lupdate(ll++,u);
      if (rr&1) lupdate(--rr,u);
    }
    iupdate(l>>=countr_zero((unsigned int)l)+1);
    iupdate(r>>=countr_zero((unsigned int)r+1)+1);
  }
  T query(int l,int r){
    if constexpr (invocable_r(T,id,F)) if (l>=r) return f.id();
    assert(0<=l&&l<r&&r<=n);
    if (l+1==r) return (*this)[l];
    if (r-l==2) return f.merge((*this)[l],(*this)[l+1]);
    T resl = (*this)[l],resr = (*this)[r-1];
    for (l+=n+1,r+=n-1;l < r;l>>=1,r>>=1){
      if (l&1) resl = f.merge(resl,segtree[l++]);
      if (r&1) resr = f.merge(segtree[--r],resr);
    }
    return f.merge(resl,resr);
  }
  T* internal(){
    return segtree;
  }
  T& top(){
    return segtree[1];
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
lazy_segment_tree(int,F) -> lazy_segment_tree<remove_cvref_t<return_type(merge,F)>,remove_cvref_t<return_type(composition,F)>,F>;

template<input_iterator I,class F>
lazy_segment_tree(I,I,F) -> lazy_segment_tree<iter_value_t<I>,remove_cvref_t<return_type(composition,F)>,F>;

template<rngs::range C,class F>
lazy_segment_tree(C&&,F) -> lazy_segment_tree<rngs::range_value_t<C>,remove_cvref_t<return_type(composition,F)>,F>;