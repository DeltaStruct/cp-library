#pragma once
#include "assets/stdc++.hpp"

template<typename T>
bool check_convex(const vector<T>& A){
  for (int i(1);i < (int)A.size()-1;++i) if (A[i]-A[i-1]>A[i+1]-A[i]) return false;
  return true;
}