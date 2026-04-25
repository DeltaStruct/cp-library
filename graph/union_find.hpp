#pragma once
#include "assets/stdc++.hpp"

struct fast_union_find {
  vector<int> par,sz;
  fast_union_find(int n = 0) : par(n,-1),sz(n,1) {}
  int leader_compress(int x){
    int ret = x;
    while(par[ret]!=-1) ret = par[ret];
    while(par[x]!=-1) x = exchange(par[x],ret);
    return ret;
  }
  int leader_not_compress(int x){
    while(par[x]!=-1) x = par[x];
    return x;
  }
  virtual int leader(int x){
    return leader_compress(x);
  }
  bool connect(int x,int y){
    x = leader(x),y = leader(y);
    if (x==y) return false;
    if (sz[x]>sz[y]) swap(x,y);
    par[x] = y,sz[y] += sz[x];
    return true;
  }
  int size(){
    return (int)par.size();
  }
  int size(int x){
    return sz[leader(x)];
  }
};

template<typename T,class F>
concept union_find_c = (same_as<T,void>&&same_as<F,void>)||invocable_r(void,merge,F,T&,T&);

template<typename T = void,class F = void>
requires union_find_c<T,F>
struct union_find;

template<>
struct union_find<void,void> : public fast_union_find {
  using base = fast_union_find;
  vector<int> lst;
  union_find(int n = 0) : base(n),lst(n) { iota(lst.begin(),lst.end(),0); }
  bool connect(int x,int y){
    x = leader(x),y = leader(y);
    if (base::connect(x,y)){
      swap(lst[x],lst[y]);
      return true;
    }
    return false;
  }
  [[nodiscard]] bool connected(int x,int y){
    return (leader(x)==leader(y));
  }
  vector<int> enumeration(int x,int y = -407){
    if (y==-407) y = size(x);
    vector<int> ret(y);
    for (int i(0);i < y;++i,x=lst[x]) ret[i] = x;
    return ret;
  }
};

template<typename T,class F>
requires union_find_c<T,F>
struct union_find : public union_find<void,void> {
  using base = union_find<void,void>;
  vector<T> val;
  F f;
  union_find(int _n,F _f) : base(_n),val(_n),f(std::move(_f)) {
    if constexpr (invocable_r(T,idi,F,int)) for (int i(0);i < _n;++i) val[i] = f.idi(i);
    else if constexpr (invocable_r(T,id,F)) fill(val.begin(),val.end(),f.id());
  }
  template<input_iterator I>
  union_find(I a,I b,F _f) : base::union_find((int)distance(a,b)),val(a,b),f(std::move(_f)) {}
  template<rngs::range C>
  union_find(C&& A,F _f) : base::union_find((int)rngs::distance(A)),val(A),f(std::move(_f)) {}
  bool connect(int x,int y){
    x = leader(x),y = leader(y);
    if (base::connect(x,y)){
      if (par[x]!=y) swap(x,y);
      f.merge(val[y],val[x]);
      return true;
    }
    return false;
  }
  const T& value(int x){
    return val[leader(x)];
  }
  template<class... Args>
  void add(int x,Args&&... args) requires invocable_r(void,add,F,T&,Args&&...) {
    f.add(val[leader(x)],std::forward<Args>(args)...);
  }
};

union_find(int) -> union_find<void,void>;

template<class F>
union_find(int,F) -> union_find<remove_cvref_t<arg_type(0,merge,F)>,F>;

template<class F,input_iterator I>
union_find(I,I,F) -> union_find<iter_value_t<I>,F>;

template<class F,rngs::range C>
union_find(C&&,F) -> union_find<rngs::range_value_t<C>,F>;