#pragma once
#include "assets/stdc++.hpp"

template<class F>
requires invocable_r<bool,F,int,int,int>
vector<int> monotone_minima(int h,int w,F f){
  assert(w>0);
  if (h==0) return {};
  vector<int> R(h+1);
  R.back() = w-1;
  for (int s(bit_ceil((unsigned int)h)>>1);s;s>>=1) for (int i(s);i < h;i += s<<1){
    int l = i-s,r = min(i+s,h);
    R[i] = R[l];
    for (int k(R[l]+1);k <= R[r];++k) if (!f(i,R[i],k)) R[i] = k;
  }
  R.pop_back();
  return R;
}

template<class F>
requires invocable_r(bool,select,F,int,int,int)
vector<int> monotone_minima(int h,int w,F f){
  return monotone_minima(h,w,[&](int i,int k,int j) -> bool { return f.select(i,k,j); });
}

template<class F,class C>
requires (invocable(value,F,int,int)||invocable<F,int,int>)
vector<int> monotone_minima(int h,int w,F f,C comp = less<>()){
  return monotone_minima(h,w,[&](int i,int k,int j){
    if constexpr (invocable(value,F,int,int)) return comp(f.value(i,k),f.value(k,j));
    else return comp(f(i,k),f(k,j));
  });
}
