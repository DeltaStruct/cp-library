#pragma once
#include "graph/graph.hpp"
#include "graph/union_find.hpp"

struct strongly_connected_components : graph<void> {
  using bs = graph<void>;
  int n;
  graph<void>& G;
  runtime_array<int> L,B;
  std::vector<int> S,V;
  strongly_connected_components(graph<void>& _G) : bs(),n((int)_G.size()),G(_G),L(n),B(n),S(),V() {
    iota(L.begin(),L.end(),0);
    runtime_array<int> ord((int)G.size(),-1),rid((int)G.size());
    int o = 0;
    auto dfs0 = [&](auto& dfs,int x) -> void {
      ord[x] = -2;
      for (int y:G[x]) if (ord[y]==-1) dfs(dfs,y);
      rid[ord[x] = o++] = x;
    };
    for (int i(0);i < (int)G.size();++i) if (ord[i]==-1) dfs0(dfs0,i);
    graph<void> H((int)G.size());
    for (int i(0);i < (int)G.size();++i) for (int k:G[i]) H.emplace_directed(k,i);
    auto dfs1 = [&](auto& dfs,int x,int y) -> void {
      std::swap(L[x],L[y]),B[x] = (int)S.size()-1,++S.back();
      for (int t:H[x]) if (ord[t]<ord[y]&&L[t]==t) dfs(dfs,t,y);
    };
    for (int i((int)G.size()-1);i > -1;--i) if (L[rid[i]]==rid[i]){
      S.emplace_back(0),V.emplace_back(rid[i]);
      dfs1(dfs1,rid[i],rid[i]);
    }
  }
  int belong(int x){
    return B[x];
  }
  int count_components(){
    return (int)S.size();
  }
  int size(int x){
    return S[x];
  }
  std::vector<int> components(int x){
    std::vector<int> ret; ret.reserve(S[x]);
    for (int i(V[x]),k(0);k < S[x];++k,i=L[i]) ret.emplace_back(i);
    return ret;
  }
};