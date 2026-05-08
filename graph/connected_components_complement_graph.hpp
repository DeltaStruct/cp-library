#pragma once
#include "graph/graph.hpp"
#include "graph/union_find.hpp"

struct connected_components_complement_graph : private union_find<void> {
  using base = union_find<void>;
  vector<vector<int>> G;
  vector<int> V;
  connected_components_complement_graph(int _n) : base(_n),G(_n),V() {}
  void add_edge(int a,int b){
    if (a<b) swap(a,b);
    G[a].emplace_back(b);
  }
  void build(){
    set<int> S;
    for (int i(0);i < n;++i){
      set<int> T;
      for (int k:G[i]){
        k = base::leader(k);
        auto it = S.lower_bound(k);
        if (it!=S.end()&&*it==k) T.emplace(k),S.erase(it);
      }
      for (int k:S) base::connect(k,i);
      S = std::move(T);
      S.emplace(base::leader(i));
    }
    V.assign(S.begin(),S.end());
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