#pragma once
#include "graph/undo_able_union_find.hpp"
#include "graph/incremental_bipartite_connectivity.hpp"

template<typename T = void,class F = void>
struct undo_able_bipartite_connectivity : public incremental_bipartite_connectivity<T,F,undo_able_union_find<T,F>> {
  using base = incremental_bipartite_connectivity<T,F,undo_able_union_find<T,F>>;
  using base::base;
  stack<unsigned long long> H;
  int hs = 0;
  virtual bool connect(int x,int y){
    if ((hs&63)==0) H.emplace(0);
    if (base::flag) H.top() |= 1ull<<(hs&63);
    ++hs;
    return base::connect(x,y);
  }
  void undo(int i = 1){
    assert(i<=hs);
    while(i--){
      base::undo(2);
      if (H.top()&(1ull<<(--hs&63))) base::flag = true,H.top() ^= 1ull<<(hs&63);
      else base::flag = false;
      if ((hs&63)==0) H.pop();
    }
  }
};

undo_able_bipartite_connectivity(int) -> undo_able_bipartite_connectivity<void,void>;

template<class F>
undo_able_bipartite_connectivity(int,F) -> undo_able_bipartite_connectivity<remove_cvref_t<arg_type(0,merge,F)>,F>;

template<class F,input_iterator I>
undo_able_bipartite_connectivity(I,I,F) -> undo_able_bipartite_connectivity<iter_value_t<I>,F>;

template<class F,rngs::range C>
undo_able_bipartite_connectivity(C&&,F) -> undo_able_bipartite_connectivity<rngs::range_value_t<C>,F>;