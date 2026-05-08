#pragma once
#include "graph/graph.hpp"
#include "sequence/runtime_array.hpp"

struct low_link {
  int n;
  graph<void>& G;
  runtime_array<int> ord,low;
  runtime_array<unsigned long long> E,V;
  int ces(){
    int ret = 0;
    for (auto& A:G) ret += A.size();
    return ((ret>>1)+63)>>6;
  }
  low_link(graph<void>& _G) : n((int)_G.size()),G(_G),ord(n,-1),low(n),E(ces()),V((n+63)>>6) {
    int o = 0;
    for (int i(0);i < n;++i) if (ord[i]==-1) build_dfs(i,o);
  }
  void build_dfs(int root,int& o){
    auto dfs = [&](auto& self,int x,int p,int e) -> int {
      low[x] = ord[x] = o++;
      int c = 0;
      for (int i(0);int y:G[x]) if (G[x][i++].idx!=e){
        if (ord[y]==-1){
          low[x] = min(low[x],self(self,y,x,G[x][i-1].idx)),++c;
          if (p!=-1&&ord[x]<=low[y]) V[x>>6] |= (1ull<<(x&63));
        } else low[x] = min(low[x],ord[y]);
      }
      if (e!=-1&&low[x]>ord[p]) E[e>>6] |= (1ull<<(e&63));
      if (c>=2&&p==-1) V[x>>6] |= (1ull<<(x&63));
      return low[x];
    };
    dfs(dfs,root,-1,-1);
  }
  bool is_bridge(int e){
    return E[e>>6]&(1ull<<(e&63));
  }
  bool is_bridge(int a,int b){
    if (ord[a]>ord[b]) swap(a,b);
    return ord[a]<low[b];
  }
  bool is_articulation_point(int v){
    return V[v<<6]&(1ull<<(v&63));
  }
};