#pragma once
#include "assets/stdc++.hpp"

struct mo {
  int n,q,w;
  vector<pair<int,int>> Q;
  vector<int> O;
  mo(int x,int y,int width = -407) : n(x),q(y),w(width),Q(y),O(y) {}
  void add(int l,int r){
    static int i = 0;
    Q[i++] = make_pair(l,r);
  }
  void resize(int s){
    Q.resize(s),O.resize(s);
  }
  long long cost(vector<int>& A){
    long long ret = 0;
    int x = 0,y = 0;
    for (auto i:A) ret += abs(x-Q[i].first)+abs(y-Q[i].second),tie(x,y) = Q[i];
    return ret;
  }
  template<class F>
  requires invocable(query,F,int)
  vector<return_type(query,F)> exec(F f,bool random_case = false){
    if (w==-407) w = max(1,(int)(random_case?sqrt(3)*n/sqrt(2*q):n/sqrt(q)));
    iota(O.begin(),O.end(),0);
    sort(O.begin(),O.end(),[&](int a,int b){
      if (Q[a].first/w!=Q[b].first/w) return (Q[a].first<Q[b].first);
      return (Q[a].first/w%2?Q[a].second<Q[b].second:Q[a].second>Q[b].second);
    });
    vector T = O;
    sort(T.begin(),T.end(),[&](int a,int b){
      if ((Q[a].first-w/2)/w!=(Q[b].first-w/2)/w) return (Q[a].first<Q[b].first);
      return ((Q[a].first-w/2)/w%2?Q[a].second<Q[b].second:Q[a].second>Q[b].second);
    });
    if (cost(T)<cost(O)) swap(T,O);
    T.clear();
    vector<return_type(query,F)> R(q);
    int l = 0,r = 0;
    if constexpr (invocable(init,F)) f.init();
    for (int i:O){
      if constexpr (invocable(add,F,int)&&invocable(erase,F,int)){
        while(Q[i].first<l) f.add(--l);
        while(Q[i].second>r) f.add(r++);
        while(Q[i].first>l) f.erase(l++);
        while(Q[i].second<r) f.erase(--r);
      } else if constexpr (
        invocable(add_left,F,int)&&invocable(add_right,F,int)&&
        invocable(erase_left,F,int)&&invocable(erase_right,F,int)
      ){
        while(Q[i].first<l) f.add_left(--l);
        while(Q[i].second>r) f.add_right(r++);
        while(Q[i].first>l) f.erase_left(l++);
        while(Q[i].second<r) f.erase_right(--r);
      } else static_assert(false_v<F>);
      R[i] = f.query(i);
    }
    return R;
  }
  int width(bool random_case = false){
    if (w==-407) w = max(1,(int)(random_case?sqrt(3)*n/sqrt(2*q):n/sqrt(q)));
    return w;
  }
};