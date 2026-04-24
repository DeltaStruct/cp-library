#include "assets/stdc++.hpp"

template<class F>
requires invocable(add,F,arg_type(0,add,F))&&invocable(undo,F)
struct semionline_deletion {
  using T = arg_type(0,add,F);
  int n,now;
  vector<vector<T>> segtree;
  F f;
  semionline_deletion(int _n,F _f) : n(_n),now(-1),segtree(2*n),f(std::move(_f)) {}
  template<class... Args>
  void emplace(int l,int r,Args&&... args){
    assert(0<=l&&l<=r&&r<=n);
    if (l<=now&&now<r){
      f.add(T(std::forward<Args>(args)...));
      segtree[n+now].emplace_back(std::forward<Args>(args)...);
      return query(l,now,std::forward<Args>(args)...),query(now+1,r,std::forward<Args>(args)...);
    }
    for (l+=n,r+=n;l < r;l>>=1,r>>=1){
      if (l&1) segtree[l++].emplace_back(std::forward<Args>(args)...);
      if (r&1) segtree[--r].emplace_back(std::forward<Args>(args)...);
    }
  }
  void add(int l,int r,const T& q){
    emplace(l,r,q);
  }
  void reset(bool end = false){
    if (0<=now&&now<n){
      for (int i(n+now);i;i>>=1) for (int k(0);k < (int)segtree[i].size();++i) f.undo();
    }
    now = (end?n:-1);
  }
  int increment(){
    if (now>=n-1){
      reset();
      return now = n;
    }
    if (now==-1){
      for (int i(countr_zero(bit_floor((unsigned int)n)));i > -1;--i){
        for (auto& a:segtree[n>>i]) f.add(a);
      }
      return now = 0;
    }
    int s = countr_one((unsigned int)(n+now));
    for (int i(0);i <= s;++i) for (int k(0);k < (int)segtree[(n+now)>>i].size();++k) f.undo();
    ++now;
    for (int i(s);i > -1;--i) for (auto& a:segtree[(n+now)>>i]) f.add(a);
    return now;
  }
  int decrement(){
    if (now<=0){
      reset();
      return now = -1;
    }
    if (now==n){
      for (int i(countr_zero(bit_floor((unsigned int)(n+n-1))));i > -1;--i){
        for (auto& a:segtree[(n+n-1)>>i]) f.add(a);
      }
      return now = 0;
    }
    int s = countr_zero((unsigned int)(n+now));
    for (int i(0);i <= s;++i) for (int k(0);k < (int)segtree[(n+now)>>i].size();++k) f.undo();
    --now;
    for (int i(s);i > -1;--i) for (auto& a:segtree[(n+now)>>i]) f.add(a);
    return now;
  }
  struct iterator {
    semionline_deletion& self;
  };
};