#pragma once
#include "assets/stdc++.hpp"

template<input_iterator I>
vector<int> zaatu(I first,I last){
  vector<remove_cvref_t<iter_value_t<I>>> Z(first,last);
  vector<int> R(Z.size());
  sort(Z.begin(),Z.end()),Z.erase(unique(Z.begin(),Z.end()),Z.end());
  for (int i(0);first!=last;++i,++first) R[i] = (int)(lower_bound(Z.begin(),Z.end(),*first)-Z.begin());
  return R;
}

template<rngs::range T>
vector<int> zaatu(T&& A){
  return zaatu(rngs::begin(A),rngs::end(A));
}