#pragma once
#include "assets/stdc++.hpp"

template<typename T>
requires requires(T v){ v += v; }
vector<T> subset_zeta_transform(vector<T> A){
  int n = bit_ceil(A.size()); A.resize(n);
  for (int i(1);i < n;i<<=1) for (int k(0);k < n;k+=i<<1){
    for (int j(0);j < i;++j) A[k+j+i] += A[k+j];
  }
  return A;
}

template<typename T>
requires requires(T v){ v -= v; }
vector<T> subset_mobius_transform(vector<T> A){
  int n = bit_ceil(A.size()); A.resize(n);
  for (int i(1);i < n;i<<=1) for (int k(0);k < n;k+=i<<1){
    for (int j(0);j < i;++j) A[k+j+i] -= A[k+j];
  }
  return A;
}

template<typename T>
requires requires(T v){ v += v; }
vector<T> superset_zeta_transform(vector<T> A){
  int n = bit_ceil(A.size()); A.resize(n);
  for (int i(1);i < n;i<<=1) for (int k(0);k < n;k+=i<<1){
    for (int j(0);j < i;++j) A[k+j] += A[k+j+i];
  }
  return A;
}

template<typename T>
requires requires(T v){ v -= v; }
vector<T> superset_mobius_transform(vector<T> A){
  int n = bit_ceil(A.size()); A.resize(n);
  for (int i(1);i < n;i<<=1) for (int k(0);k < n;k+=i<<1){
    for (int j(0);j < i;++j) A[k+j] -= A[k+j+i];
  }
  return A;
}

function_alias(subset_zeta,subset_zeta_transform);
function_alias(subset_mobius,subset_mobius_transform);
function_alias(superset_zeta,superset_zeta_transform);
function_alias(superset_mobius,superset_mobius_transform);