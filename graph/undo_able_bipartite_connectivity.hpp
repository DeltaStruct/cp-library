#pragma once
#include "graph/undo_able_union_find.hpp"
#include "graph/incremental_bipartite_connectivity.hpp"

template<typename T = void,class F = void>
struct undo_able_bipartite_connectivity : public incremental_bipartite_connectivity<T,F,undo_able_union_find<T,F>> {
  using base = incremental_bipartite_connectivity<T,F,undo_able_union_find<T,F>>;
  using base::base;
  int border = -1;
  virtual bool connect(int x,int y){
    if (base::flag&&border==-1) border = base::S.size();
    return base::connect(x,y);
  }
  void undo(int i = 1){
    assert(i<=hs);
    while(i--){
      base::undo(2);
      if (border<(int)base::S.size()) base::flag = false,border = -1;
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