#pragma once
#include "graph/graph.hpp"
#include "sequence/runtime_array.hpp"

template<typename T>
vector<int> cycle_detection_directed(const graph<T>& G,int root = -1){
  vector<int> ret(G.size()); runtime_array<int> vis((int)G.size(),1),now((int)G.size());
  auto dfs = [&](auto& self,int x,int ptr) -> bool {
    if (now[x]){
      ret.erase(ret.begin()+ptr,ret.end());
      ret.erase(ret.begin(),ret.begin()+(now[x]-1));
      return true;
    }
    now[x] = ptr+1;
    for (int i(0);int y:G[x]){
      ret[ptr] = G[x][i++].idx;
      if (vis[y]&&self(self,y,ptr+1)) return true;
    }
    now[x] = vis[x] = 0;
    return false;
  };
  if (root==-1){
    for (int i(0);i < (int)G.size();++i) if (vis[i]&&dfs(dfs,i,0)) return ret;
  } else if (dfs(dfs,root,0)) return ret;
  return {};
}

template<typename T>
vector<int> cycle_detection_undirected(const graph<T>& G,int root = -1){
  vector<int> ret(G.size()); runtime_array<int> vis((int)G.size(),1),now((int)G.size());
  auto dfs = [&](auto& self,int x,int e,int ptr) -> bool {
    if (now[x]){
      ret.erase(ret.begin()+ptr,ret.end());
      ret.erase(ret.begin(),ret.begin()+(now[x]-1));
      return true;
    }
    now[x] = ptr+1;
    for (int i(0);int y:G[x]){
      if (x==y){
        ret = {G[x][i].idx};
        return true;
      }
      ret[ptr] = G[x][i++].idx;
      if (e!=ret[ptr]&&vis[y]&&self(self,y,ret[ptr],ptr+1)) return true;
    }
    now[x] = vis[x] = 0;
    return false;
  };
  if (root==-1){
    for (int i(0);i < (int)G.size();++i) if (vis[i]&&dfs(dfs,i,-1,0)) return ret;
  } else if (dfs(dfs,root,-1,0)) return ret;
  return {};
}

template<typename T>
vector<vector<int>> cycle_path_idx_to_vertex(const graph<T>& G,const vector<vector<int>>& A){
  vector<pair<int,int>> E;
  for (int i(0);i < (int)G.size();++i) for (int k(0);k < (int)G[i].size();++k){
    if (G[i][k].idx>=(int)E.size()) E.resize(G[i][k].idx+1);
    E[G[i][k].idx] = make_pair(i,G[i][k].to);
  }
  vector<vector<int>> ret(A.size());
  for (int i(0);i < (int)A.size();++i){
    vector<int>& R = ret[i]; R.reserve(A[i].size());
    if (A[i].size()==1){
      R.emplace_back(E[A[i][0]].first);
      continue;
    }
    R.emplace_back(E[A[i][0]].first),R.emplace_back(E[A[i][0]].second);
    if (E[A[i][1]].first!=R.back()&&E[A[i][1]].second!=R.back()) swap(R[0],R[1]);
    for (int k(1);k < (int)A[i].size()-1;++k){
      R.emplace_back(E[A[i][k]].first^E[A[i][k]].second^R.back());
    }
  }
  return ret;
}

template<typename T>
vector<int> cycle_path_idx_to_vertex(const graph<T>& G,const vector<int>& A){
  return cycle_path_idx_to_vertex(G,vector(1,A))[0];
}