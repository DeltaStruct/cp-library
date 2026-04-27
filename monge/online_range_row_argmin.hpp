#pragma once
#include "assets/stdc++.hpp"

template<class F,class C = less<>>
requires invocable_r<bool,C,return_type(value,F),return_type(value,F)>
struct rra_v2s1 {
  using T = arg_type(0,value,F);
  F f;
  C comp;
  bool select(T i,int k,int j){
    return comp(f.value(i,k),f.value(i,j));
  }
  rra_v2s1(F _f,C _comp = C()) : f(std::move(_f)),comp(std::move(_comp)) {}
};

template<class F>
requires invocable_r(bool,select,F,arg_type(0,select,F),int,int)
struct online_range_row_argmin {
  using T = arg_type(0,select,F);
  using S = online_range_row_argmin;
  T h; int w;
  vector<pair<T,int>> segtree;
  F f;
  online_range_row_argmin(T _h,int _w,F _f) : h(_h),w(_w),segtree(w),f(std::move(_f)) {
    build();
  }
  template<class V>
  requires invocable(value,V,T,int)
  online_range_row_argmin(T _h,int _w,V _f) : S(_h,_w,rra_v2s1(std::move(_f))) {}
  template<class V,class C>
  requires invocable(value,V,T,int)
  online_range_row_argmin(T _h,int _w,V _f,C c) : S(_h,_w,rra_v2s1(std::move(_f),std::move(c))) {}
  void build(){
    vector<vector<pair<T,int>>> table(2*w);
    for (int i(0);i < w;++i) table[w+i].emplace_back(h-1,i);
    for (int t(w-1);t;--t) segtree[t] = [&](auto& X,auto& Y,auto& R){
      for (int i(0),k(0);i < (int)X.size();){
        if (!f.select(min(X[i],Y[k]).first,X[i].second,Y[k].second)){
          T l = min(X[i],Y[k]).first,r = max((i==0?-1:X[i-1].first),(k==0?-1:Y[k-1].first));
          while(l-r>1){
            T mid = (l+r)/2;
            if (f.select(mid,X[i].second,Y[k].second)) r = mid;
            else l = mid;
          }
          l = (r!=-1&&(i==0||X[i-1].first!=r));
          R.reserve(i+l+(int)Y.size()-k);
          copy(X.begin(),X.begin()+i,back_inserter(R));
          if (l) R.emplace_back(r,X[i].second);
          copy(Y.begin()+k,Y.end(),back_inserter(R));
          int ret = X[i].second;
          table.pop_back(),table.pop_back();
          return make_pair(r,ret);
        }
        if (X[i].first==Y[k].first) ++i,++k;
        else if (X[i].first<Y[k].first) ++i;
        else ++k;
      }
      R = X;
      table.pop_back(),table.pop_back();
      return R.back();
    }(table[t<<1],table[t<<1|1],table[t]);
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
};

template<class F>
requires invocable_r(bool,select,F,arg_type(0,select,F),int,int)
online_range_row_argmin(arg_type(0,select,F),int,F) -> online_range_row_argmin<F>;

template<class V>
requires invocable(value,V,arg_type(0,value,V),int)
online_range_row_argmin(arg_type(0,value,V),int,V) -> online_range_row_argmin<rra_v2s1<V>>;

template<class V,class C>
requires invocable(value,V,arg_type(0,value,V),int)
online_range_row_argmin(arg_type(0,value,V),int,V,C) -> online_range_row_argmin<rra_v2s1<V,C>>;