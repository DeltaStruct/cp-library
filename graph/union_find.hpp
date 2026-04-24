#include "assets/stdc++.hpp"

template<typename T = void,class F = void>
requires (same_as<T,void>&&same_as<F,void>)||invocable(merge,F,T&,T&)
struct union_find;

template<>
struct union_find<void,void> {
  vector<int> par,sz,lst;
  union_find(int n = 0) : par(n,-1),sz(n,1),lst(n) { iota(lst.begin(),lst.end(),0); }
  int leader_compress(int x){
    int ret = x;
    while(par[ret]!=-1) ret = par[ret];
    while(par[x]!=-1) x = exchange(par[x],ret);
    return ret;
  }
  int leader_not_compress(int x){
    while(par[x]!=-1) x = par[x];
    return x;
  }
  virtual int leader(int x){
    return leader_compress(x);
  }
  bool connect(int x,int y){
    x = leader(x),y = leader(y);
    if (x==y) return false;
    if (sz[x]>sz[y]) swap(x,y);
    par[x] = y,sz[y] += sz[x],swap(lst[x],lst[y]);
    return true;
  }
  bool connected(int x,int y){
    return (leader(x)==leader(y));
  }
  vector<int> enumeration(int x,int y = -407){
    if (y==-407) y = size(x);
    vector<int> ret(y);
    for (int i(0);i < y;++i,x=lst[x]) ret[i] = x;
    return ret;
  }
  int size(){
    return par.size();
  }
  int size(int x){
    return sz[leader(x)];
  }
};

template<typename T = void,class F = void>
struct union_find : public union_find<void,void> {
  using base = union_find<void,void>;
  vector<T> val;
  F f;
  union_find(int _n,F _f) : base::base(_n),val(_n),f(std::move(_f)) {}
};