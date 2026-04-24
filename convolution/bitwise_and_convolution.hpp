#include "convolution/zeta_mobius_transform.hpp"

template<typename T>
requires requires(T v){ v *= v; }
vector<T> bitwise_and_convolution(vector<T> A,vector<T> B){
  A = superset_zeta(A),B = superset_zeta(B);
  for (int i(0);i < (int)A.size();++i) A[i] *= B[i];
  return superset_mobius(A);
}