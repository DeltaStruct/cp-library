#pragma once
#include "graph/topological_sort.hpp"

template<typename T>
vector<int> offline_dag_reachability(const graph<T>& G,const vector<pair<int,int>>& Q){
  int n = (int)G.size(),q = (int)Q.size();
  runtime_array O = topological_sort(G);
  vector<int> ret(q);
  for (int l(0);l < q;l+=64){
    int r = min(l+64,q);
    vector<unsigned long long> dp(n);
    for (int i(l);i < r;++i) dp[Q[i].first] |= (1ull<<(i-l));
    for (int i:O) for (int k:G[i]) dp[k] |= dp[i];
    for (int i(l);i < r;++i) ret[i] = ((dp[Q[i].second]>>(i-l))&1);
  }
  return ret;
}