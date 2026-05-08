#pragma once
#include "graph/strongly_connected_components.hpp"

vector<int> two_sat(int n,vector<pair<int,int>> Q){
  graph G(2*n);
  for (auto [a,b]:Q){
    if (a>0) --a;
    else a += 2*n;
    if (b>0) --b;
    else b += 2*n;
    G.emplace_directed(2*n-a-1,b),G.emplace_directed(2*n-b-1,a);
  }
  strongly_connected_components SCC(G);
  vector<int> ret(n);
  for (int i(0);i < n;++i){
    if (SCC.belong(i)==SCC.belong(2*n-i-1)) return {};
    ret[i] = (SCC.belong(i)>SCC.belong(2*n-i-1));
  }
  return ret;
}