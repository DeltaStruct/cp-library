#pragma once
#include "assets/stdc++.hpp"
#include "graph/union_find.hpp"

template<typename T,class F>
requires same_as<F,void>||invocable_r(void,split,F,T&,T&)
struct undo_able_union_find : public union_find<T,F> {
  using base = union_find<T,F>;
  using base::union_find;
  using base::par,base::sz,base::cnt,base::lst;
  stack<pair<int,int>> S;
  int inner = 0;
  virtual int leader(int x){
    return base::leader_not_compress(x);
  }
  virtual bool connect(int x,int y){
    x = leader(x),y = leader(y);
    if (base::connect(x,y)){
      if (par[x]!=y) swap(x,y);
      S.emplace(x,y);
      return true;
    }
    S.emplace(-1,-1);
    return false;
  }
  void undo(int i = 1){
    assert(i<=(int)S.size());
    while(i--){
      auto [a,b] = S.top(); S.pop();
      if (a==-1) continue;
      par[a] = -1,sz[b] -= sz[a],++cnt,swap(lst[a],lst[b]);
      if constexpr (!same_as<F,void>) base::f.split(base::val[b],base::val[a]);
    }
  }
  int get_state(){
    return S.size();
  }
  int snapshot(){
    return inner = S.size();
  }
  void rollback(int state = -407){
    if (state==-407) state = inner;
    undo(S.size()-state);
  }
  template<class... Args>
  void add(int x,Args&&... args) requires invocable_r(void,add,F,T&,Args&&...) {
    for (;x!=-1;x=par[x]) base::f.add(base::val[x],std::forward<Args>(args)...);
  }
};

undo_able_union_find(int) -> undo_able_union_find<void,void>;

template<class F>
undo_able_union_find(int,F) -> undo_able_union_find<remove_cvref_t<arg_type(0,merge,F)>,F>;

template<class F,input_iterator I>
undo_able_union_find(I,I,F) -> undo_able_union_find<iter_value_t<I>,F>;

template<class F,rngs::range C>
undo_able_union_find(C&&,F) -> undo_able_union_find<rngs::range_value_t<C>,F>;