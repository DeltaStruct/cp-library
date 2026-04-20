#pragma once
#include "assets/stdc++.hpp"
#include "monge/monotone_minima.hpp"
#include "monge/smawk.hpp"
#include "check/convex.hpp"

template<typename T>
vector<T> min_plus_convolution(const vector<T>& A,const vector<T>& B){
  bool a = check_convex(A),b = check_convex(B);
  int h = (int)A.size(),w = (int)B.size();
  vector<T> R(h+w-1);
  if (a&&b){
    int i = 0,k = 0;
    for (int j(0);j < h+w-1;++j){
      R[j] = A[i]+B[k];
      if (k==w-1||(i!=h-1&&A[i+1]+B[k]<=A[i]+B[k+1])) ++i;
      else ++k;
    }
    return R;
  }
  if (a){
    vector C = constexpr_if(use_smawk)(smawk,monotone_minima)(h+w-1,w,[&](int i,int k,int j){
      return (i<j)||(i-k<h&&B[k]+A[i-k]<=B[j]+A[i-j]);
    });
    for (int i(0);i < h+w-1;++i) R[i] = B[C[i]]+A[i-C[i]];
    return R;
  }
  if (b){
    vector C = constexpr_if(use_smawk)(smawk,monotone_minima)(h+w-1,h,[&](int i,int k,int j){
      return (i<j)||(i-k<w&&A[k]+B[i-k]<=A[j]+B[i-j]);
    });
    for (int i(0);i < h+w-1;++i) R[i] = A[C[i]]+B[i-C[i]];
    return R;
  }
  assert(false);
  for (int i(0);i < h+w-1;++i) R[i] = A[min(i,h-1)]+B[max(i-h+1,0)];
  for (int i(0);i < h;++i) for (int k(0);k < w;++k) R[i+k] = min(R[i+k],A[i]+B[k]);
  return R;
}