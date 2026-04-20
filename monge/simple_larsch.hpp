#pragma once
#include "assets/stdc++.hpp"

template<typename T>
struct larsch_ret_t {
  vector<T> dist;
  vector<int> prev;
  vector<int> count;
};

template<class F>
requires invocable(value,F,int,return_type(value,F)&,int)
       &&invocable_r(bool,select,F,int,return_type(value,F)&,int,return_type(value,F)&,int)
larsch_ret_t<return_type(value,F)> simple_larsch(int n,F f){
  using T = return_type(value,F);
  vector<T> R(n+1);
  if constexpr (invocable_r(T,init,F)) fill(R.begin(),R.end(),f.init());
  if constexpr (invocable_r(T,id,F)) R[0] = f.id();
  vector<int> P(n+1);
  auto update = [&](int x,int y) -> void {
    if (!f.select(x,R[x],P[x],R[y],y)) R[x] = f.value(x,R[y],y),P[x] = y;
  };
  pair<int,int>* stk = new pair<int,int>[countr_zero(bit_ceil((unsigned int)n))];
  int top = -1,mid = 0,l = 0,r = n;
  for (;r > 1;r=mid){
    stk[++top] = make_pair(0,r);
    mid = r>>1;
    for (int i(P[0]);i <= P[r];++i) update(mid,i);
  }
  for (;top!=-1;--top){
    l = stk[top].first,r = stk[top].second,mid = (l+r)/2;
    for (int i(l+1);i <= mid;++i) update(r,i);
    if (r-mid==1) continue;
    stk[top++] = make_pair(mid,r),l = mid,mid = (l+r)/2;
    for (int i(P[l]);i <= P[r];++i) update(mid,i);
    while(r-l>3){
      stk[top++] = make_pair(l,r = mid);
      mid = (l+r)/2;
      for (int i(P[l]);i <= P[r];++i) update(mid,i);
    }
  }
  vector<int> D(n+1);
  for (int i(1);i <= n;++i) D[i] = D[P[i]]+1;
  return larsch_ret_t<T>{move(R),move(P),move(D)};
}

template<class F,class C>
requires invocable(value,F,int,return_type(value,F)&,int)
       ||invocable<F,int,return_type<F>&,int>
larsch_ret_t<return_type(value,F)> simple_larsch(int n,F f,C comp = less<>()){
  using T = return_type(value,F);
  auto value = [&](int x,T& v,int y) -> T {
    if constexpr (invocable(value,F,int,return_type(value,F)&,int)) return f.value(x,v,y);
    return f(x,v,y);
  };
  return simple_larsch(n,guide(value,select)(
    value,
    [&](int i,T& x,int k,T& y,int j) -> bool { return comp(x,value(i,y,j)); }
  ));
}