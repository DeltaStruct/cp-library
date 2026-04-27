#pragma once
#include "monge/range_row_argmin_base.hpp"

template<class F>
requires invocable_r(bool,select,F,arg_type(0,select,F),int,int)
struct semionline_range_row_argmin {
  using T = arg_type(0,select,F);
  using S = semionline_range_row_argmin;
  T h; int w;
  vector<pair<T,int>> segtree;
  F f;
  semionline_range_row_argmin(T _h,int _w,F _f) : h(_h),w(_w),segtree(w),f(std::move(_f)) {
    build();
  }
  template<class V>
  requires invocable(value,V,T,int)
  semionline_range_row_argmin(T _h,int _w,V _f) : S(_h,_w,rra_v2s(std::move(_f))) {}
  template<class V,class C>
  requires invocable(value,V,T,int)
  semionline_range_row_argmin(T _h,int _w,V _f,C c) : S(_h,_w,rra_v2s(std::move(_f),std::move(c))) {}
  void build(){
    vector<vector<pair<T,int>>> table(2*w);
    for (int i(0);i < w;++i) table[w+i].emplace_back(h-1,i);
    for (int t(w-1);t;--t){
      segtree[t] = rra_merge(table[t<<1],table[t<<1|1],table[t],f);
      table.pop_back(),table.pop_back();
    }
  }
  int row_argmin(T x,int l,int r){
    assert(l<r);
    int ret = l;
    for (l+=w,r+=w;l < r;l>>=1,r>>=1){
      if (l&1){
        int t = w,y = l++;
        while(y<w){
          if (segtree[y].first>=x) t = min(t,segtree[y].second),y<<=1;
          else y = y<<1|1;
        }
        t = min(t,y-w);
        if (ret<t) swap(ret,t);
        if (f.select(x,t,ret)) ret = t;
      }
      if (r&1){
        int t = w,y = --r;
        while(y<w){
          if (segtree[y].first>=x) t = min(t,segtree[y].second),y<<=1;
          else y = y<<1|1;
        }
        t = min(t,y-w);
        if (ret<t) swap(ret,t);
        if (f.select(x,t,ret)) ret = t;
      }
    }
    return ret;
  }
  return_type(value,F) row_minvalue(T x,int l,int r) requires invocable(value,F,T,int) {
    return f.value(x,row_argmin(x,l,r));
  }
  auto row_min(T x,int l,int r){
    return row_minvalue(x,l,r);
  }
};

template<class F>
requires invocable_r(bool,select,F,arg_type(0,select,F),int,int)
semionline_range_row_argmin(arg_type(0,select,F),int,F) -> semionline_range_row_argmin<F>;

template<class V>
requires invocable(value,V,arg_type(0,value,V),int)
semionline_range_row_argmin(arg_type(0,value,V),int,V) -> semionline_range_row_argmin<rra_v2s<V>>;

template<class V,class C>
requires invocable(value,V,arg_type(0,value,V),int)
semionline_range_row_argmin(arg_type(0,value,V),int,V,C) -> semionline_range_row_argmin<rra_v2s<V,C>>;