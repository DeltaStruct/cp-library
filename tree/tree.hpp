#pragma once
#include "assets/stdc++.hpp"
#include "graph/graph.hpp"
#include "sequence/runtime_array.hpp"

template<typename T>
using tree = graph<T>;

template<typename T = void>
struct rooted_tree : public tree<T> {
  using base = tree<T>;
  int root;
  runtime_array<int> depth,sz,par,jump;
  rooted_tree() = default;
  rooted_tree(int n,int _root = 0) : base(n),root(_root),depth(n),sz(n,1),par(n),jump(n) {}
  void build(){
    jump[root] = root;
    auto dfs = [&](auto& rec,int x,int p,int d) -> int {
      depth[x] = d,par[x] = p;
      if (depth[jump[par[x]]]-depth[par[x]]==depth[jump[jump[par[x]]]]-depth[jump[par[x]]]){
        jump[x] = jump[jump[par[x]]];
      } else jump[x] = par[x];
      for (int y:base::operator[](x)) if (y!=p) sz[x] += rec(rec,y,x,d+1);
      return sz[x];
    };
    dfs(dfs,root,root,0);
  }
  int lca(int a,int b){
    if (depth[a]<depth[b]) std::swap(a,b);
    while(depth[a]>depth[b]){
      if (depth[jump[a]]>=depth[b]) a = jump[a];
      else a = par[a];
    }
    if (a==b) return a;
    while(par[a]!=par[b]){
      if (jump[a]!=jump[b]) a = jump[a],b = jump[b];
      else a = par[a],b = par[b];
    }
    return par[a];
  }
  int distance(int a,int b){
    return depth[a]+depth[b]-2*depth[lca(a,b)];
  }
};