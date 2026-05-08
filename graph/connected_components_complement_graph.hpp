#pragma once
#include "graph/graph.hpp"
#include "graph/union_find.hpp"

struct connected_components_complement_graph : private union_find<void> {
  using base = union_find<void>;
  graph<void> G;
  vector<int> V;
  connected_components_complement_graph(int _n) : base(_n),G(_n),V() {}
  void add_edge(int a,int b){
    G.emplace_undirected(a,b);
  }
  void build(){
    list<int> S;
    for (int i(0);i < n;++i){
      S.emplace_back(i);
      sort(G[i].begin(),G[i].end(),less<int>());
    }
    queue<int> que;
    runtime_array<int> E(n);
    while(!S.empty()){
      int start = *S.begin(); S.erase(S.begin());
      que.emplace(start);
      while(!que.empty()){
        int x = que.front(); que.pop();
        for (int y:G[x]) E[y] = 1;
        auto it = S.begin();
        while(it!=S.end()) if (E[*it]==0){
          base::connect(x,*it),que.emplace(*it),it=S.erase(it);
        } else ++it;
        for (int y:G[x]) E[y] = 0;
      }
      V.emplace_back(base::leader(start));
    }
  }
  int belong(int x){
    assert((int)V.size()); // no call build();
    return (int)(lower_bound(V.begin(),V.end(),base::leader(x))-V.begin());
  }
  bool connected(int a,int b){
    assert((int)V.size()); // no call build();
    return base::connected(a,b);
  }
  int count_components(){
    assert((int)V.size()); // no call build();
    return (int)V.size();
  }
  vector<int> components(int x){
    assert((int)V.size()); // no call build();
    return base::enumeration(V[x]);
  }
};