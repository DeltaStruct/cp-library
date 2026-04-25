#pragma once
#include "graph/undo_able_bipartite_connectivity.hpp"
#include "graph/semionline_dynamic_connectivity.hpp"

template<typename T = void,class F = void>
struct semionline_dynamic_bipartite_connectivity : public semionline_dynamic_connectivity<T,F,undo_able_bipartite_connectivity<T,F>> {
  using base = semionline_dynamic_connectivity<T,F,undo_able_bipartite_connectivity<T,F>>;
  using base::base;
};

semionline_dynamic_bipartite_connectivity(int,int) -> semionline_dynamic_bipartite_connectivity<void,void>;

template<class F>
semionline_dynamic_bipartite_connectivity(int,int,F) -> semionline_dynamic_bipartite_connectivity<remove_cvref_t<arg_type(0,merge,F)>,F>;

template<class F,input_iterator I>
semionline_dynamic_bipartite_connectivity(I,I,int,F) -> semionline_dynamic_bipartite_connectivity<iter_value_t<I>,F>;

template<class F,rngs::range C>
semionline_dynamic_bipartite_connectivity(C&&,int,F) -> semionline_dynamic_bipartite_connectivity<rngs::range_value_t<C>,F>;

#include "graph/offline_dynamic_bipartite_connectivity.hpp"