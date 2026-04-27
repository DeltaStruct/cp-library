#pragma once
#include "assets/stdc++.hpp"

template<class F,class C = less<>>
requires invocable_r<bool,C,return_type(value,F),return_type(value,F)>
struct rra_v2s {
  using T = arg_type(0,value,F);
  F f;
  C comp;
  return_type(value,F) value(T i,int k){
    return f.value(i,k);
  }
  bool select(T i,int k,int j){
    return comp(f.value(i,k),f.value(i,j));
  }
  rra_v2s(F _f,C _comp = C()) : f(std::move(_f)),comp(std::move(_comp)) {}
};

template<typename T,class F>
pair<T,int> rra_merge(vector<pair<T,int>>& X,vector<pair<T,int>>& Y,vector<pair<T,int>>& R,F& f){
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
      return make_pair(r,ret);
    }
    if (X[i].first==Y[k].first) ++i,++k;
    else if (X[i].first<Y[k].first) ++i;
    else ++k;
  }
  R = X;
  return R.back();
}