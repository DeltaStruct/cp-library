#pragma once
#include "assets/stdc++.hpp"
#include "convolution/bitwise_or_convolution.hpp"

template<typename T>
requires requires(T v){ v += v; v -= v; v *= v; }
vector<T> subset_convolution(vector<T> A,vector<T> B){
  int s = countr_zero(bit_ceil(max(A.size(),B.size()))),n = 1<<s;
  A.resize(n),B.resize(n);
  struct rank {
    vector<T> A;
    rank(int n,int i,T&& v) : A(n) {
      A[popcount((unsigned int)i)] = std::move(v);
    }
    rank& operator+=(rank& x){
      for (int i(0);i < (int)A.size();++i) A[i] += x.A[i];
      return *this;
    }
    rank& operator-=(rank& x){
      for (int i(0);i < (int)A.size();++i) A[i] -= x.A[i];
      return *this;
    }
    rank& operator*=(rank& x){
      vector<T> R(A.size());
      T tmp;
      for (int i(0);i < (int)A.size();++i) for (int k(0);k < (int)A.size();++k){
        tmp = A[i];
        R[i+k] += (tmp *= x.A[k]);
      }
      A = std::move(R);
      return *this;
    }
    T& operator[](int x){
      return A[popcount((unsigned int)x)];
    }
  };
  vector<rank> X(n),Y(n);
  for (int i(0);i < n;++i) X[i] = rank(s,i,std::move(A[i])),Y[i] = rank(s,i,std::move(B[i]));
  X = bitwise_or_convolution(std::move(X),std::move(Y));
  for (int i(0);i < n;++i) A[i] = X[i][i];
  return A;
}