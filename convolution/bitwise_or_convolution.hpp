#include "convolution/zeta_mobius_transform.hpp"

template<typename T>
requires requires(T v){ v *= v; }
vector<T> bitwise_or_convolution(vector<T> A,vector<T> B){
  A = subset_zeta(A),B = subset_zeta(B);
  for (int i(0);i < (int)A.size();++i) A[i] *= B[i];
  return subset_mobius(A);
}