#pragma once
#include "graph/low_link.hpp"

struct two_edge_connected_components : graph<void> {
  using base = graph<void>;
  low_link& L;
  std::vector<std::vector<int>> V;
  runtime_array<int> B;
  two_edge_connected_components(low_link& _L) : L(_L),V(),B((int)L.G.size(),-1) {
    std::vector<int> vis(L.G.size(),1);
    for (int i(0);i < (int)L.G.size();++i) if (B[i]==-1) build(i,vis);
  }
  void build(int root,std::vector<int>& vis){
    auto dfs = [&](auto& rec,int x,int p,int c) -> void {
      if (p!=-1&&L.is_bridge(x,p)){
        base::emplace_back(),base::emplace_undirected(c,(int)V.size());
        c = (int)V.size(),V.emplace_back();
      }
      B[x] = c,V[c].emplace_back(x),vis[x] = 0;
      for (int y:L.G[x]) if (vis[y]) rec(rec,y,x,c);
    };
    V.emplace_back(),base::emplace_back();
    dfs(dfs,root,-1,(int)V.size()-1);
  }
  int belong(int x){
    return B[x];
  }
  std::vector<int> vertex_set(int x){
    return V[x];
  }
};