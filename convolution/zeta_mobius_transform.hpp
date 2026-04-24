#pragma once
#include "assets/stdc++.hpp"

template<typename T>
requires requires(T v){ v += v; }
vector<T> subset_zeta_transform(vector<T> A){
  int s = countr_zero(bit_ceil(A.size())); A.resize(1<<s);
  for (int i(0);i < s;++i) for (int k(0);k < (1<<s);++k) if ((k>>i)&1) A[k] += A[k^(1<<i)];
  return A;
}

template<typename T>
requires requires(T v){ v -= v; }
vector<T> subset_mobius_transform(vector<T> A){
  int s = countr_zero(bit_ceil(A.size())); A.resize(1<<s);
  for (int i(0);i < s;++i) for (int k(0);k < (1<<s);++k) if ((k>>i)&1) A[k] -= A[k^(1<<i)];
  return A;
}

template<typename T>
requires requires(T v){ v += v; }
vector<T> superset_zeta_transform(vector<T> A){
  int s = countr_zero(bit_ceil(A.size())); A.resize(1<<s);
  for (int i(0);i < s;++i) for (int k(0);k < (1<<s);++k) if ((k>>i)&1) A[k^(1<<i)] += A[k];
  return A;
}

template<typename T>
requires requires(T v){ v -= v; }
vector<T> superset_mobius_transform(vector<T> A){
  int s = countr_zero(bit_ceil(A.size())); A.resize(1<<s);
  for (int i(0);i < s;++i) for (int k(0);k < (1<<s);++k) if ((k>>i)&1) A[k^(1<<i)] -= A[k];
  return A;
}

function_alias(subset_zeta,subset_zeta_transform);
function_alias(subset_mobius,subset_mobius_transform);
function_alias(superset_zeta,superset_zeta_transform);
function_alias(superset_mobius,superset_mobius_transform);