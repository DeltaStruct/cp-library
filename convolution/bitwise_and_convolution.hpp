#pragma once
#include "convolution/zeta_mobius_transform.hpp"

template<typename T>
requires requires(T v){ v *= v; }
vector<T> bitwise_and_convolution(vector<T> A,vector<T> B){
  A = superset_zeta(std::move(A)),B = superset_zeta(std::move(B));
  for (int i(0);i < (int)A.size();++i) A[i] *= B[i];
  return superset_mobius(std::move(A));
}