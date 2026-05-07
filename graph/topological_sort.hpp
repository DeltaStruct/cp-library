#pragma once
#include "graph/graph.hpp"
#include "sequence/runtime_array.hpp"

template<typename T>
vector<int> topological_sort(const graph<T>& G){
  int n = (int)G.size();
  vector<int> ret; ret.reserve(n);
  runtime_array<int> D(n);
  queue<int> que;
  for (auto& A:G) for (int a:A) ++D[a];
  for (int i(0);i < n;++i) if (D[i]==0) que.emplace(i);
  while(!que.empty()){
    int x = que.front(); que.pop();
    ret.emplace_back(x);
    for (int y:G[x]) if (--D[y]==0) que.emplace(y);
  }
  if ((int)ret.size()!=n) return {};
  return ret;
}

template<typename T>
bool is_unique_topological_order(const graph<T>& G){
  int n = (int)G.size();
  vector<int> ret; ret.reserve(n);
  runtime_array<int> D(n);
  queue<int> que;
  for (auto& A:G) for (int a:A) ++D[a];
  for (int i(0);i < n;++i) if (D[i]==0) que.emplace(i);
  while(!que.empty()){
    if (que.size()>1) return false;
    int x = que.front(); que.pop();
    ret.emplace_back(x);
    for (int y:G[x]) if (--D[y]==0) que.emplace(y);
  }
  return ((int)ret.size()==n);
}