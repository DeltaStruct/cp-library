#pragma once
#include "convolution/zeta_mobius_transform.hpp"
#include "sequence/runtime_array.hpp"

template<typename T>
requires requires(T v){ v += v; v -= v; v *= v; }
vector<T> subset_convolution(vector<T> A,vector<T> B){
  int s = countr_zero(bit_ceil(max(A.size(),B.size()))),n = 1<<s;
  A.resize(n),B.resize(n);
  vector X(s+1,vector<T>(n)),Y = X;
  for (int i(0);i < n;++i){
    X[popcount((unsigned int)i)][i] = A[i],Y[popcount((unsigned int)i)][i] = B[i];
  }
  for (int i(0);i <= s;++i){
    X[i] = subset_zeta(std::move(X[i])),Y[i] = subset_zeta(std::move(Y[i]));
  }
  runtime_array<T> res(s+2);
  T tmp;
  for (int i(0);i < n;++i){
    for (int k(0);k <= s;++k) for (int j(0);j <= s-k;++j){
      tmp = X[k][i],res[k+j] += (tmp *= Y[j][i]);
    }
    for (int k(0);k <= s;++k) X[k][i] = res[k],res[k] = T();
  }
  for (int i(0);i <= s;++i) X[i] = subset_mobius(std::move(X[i]));
  for (int i(0);i < n;++i) A[i] = X[popcount((unsigned int)i)][i];
  return A;
}