#pragma once
#include "graph/graph.hpp"

template<typename T>
struct dijkstra_shortest_path {
  vector<T> dist;
  vector<int> from,idx;
};

template<typename T,class Comp = less<T>>
requires invocable_r<bool,Comp,const T&,const T&>
dijkstra_shortest_path<T> dijkstra(const graph<T>& G,int root,const Comp& comp = less<T>()){
  int n = (int)G.size();
  vector<T> dist(n,-1);
  vector<int> from(n,-1),idx(n,-1);
  auto cmp = [&](const pair<T,int>& x,const pair<T,int>& y) -> bool {
    return comp(y.first,x.first)||(!(comp(x.first,y.first))&&y.second<x.second);
  };
  priority_queue<pair<T,int>,vector<pair<T,int>>,decltype(cmp)> que(cmp);
  dist[root] = 0,que.emplace(0,root);
  while(!que.empty()){
    auto [a,b] = que.top(); que.pop();
    if (comp(dist[b],a)) continue;
    for (int i(0);auto [c,d]:G[b]){
      if ((from[c]==-1&&dist[c]==-1)||comp(a+d,dist[c])){
        que.emplace(dist[c]=a+d,c),from[c] = b,idx[c] = G[b][i].idx;
      }
      ++i;
    }
  }
  return dijkstra_shortest_path<T>{std::move(dist),std::move(from),std::move(idx)};
}