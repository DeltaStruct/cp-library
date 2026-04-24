#pragma once
#include "graph/undo_able_union_find.hpp"
#include "query/semionline_deletion.hpp"

template<class S>
struct guide_sodc {
  S& self;
  void add(pair<int,int> x){
    self.S::connect(x.first,x.second);
  }
  void undo(){
    self.S::undo();
  }
};

template<typename T = void,class F = void>
struct semionline_dynamic_connectivity : public semionline_deletion<guide_sodc<undo_able_union_find<T,F>>>,undo_able_union_find<T,F> {
  using uauf = undo_able_union_find<T,F>;
  using guide = guide_sodc<uauf>;
  using deletion = semionline_deletion<guide>;
  semionline_dynamic_connectivity(int n,int q) : deletion(q,guide{*this}),uauf(n) {}
  semionline_dynamic_connectivity(int n,int q,F f) : deletion(q,guide{*this}),uauf(n,std::move(f)) {}
  template<input_iterator I>
  semionline_dynamic_connectivity(I a,I b,int q,F f) : deletion(q,guide{*this}),uauf(a,b,std::move(f)) {}
  template<rngs::range C>
  semionline_dynamic_connectivity(C&& A,int q,F f) : deletion(q,guide{*this}),uauf(std::forward<C&&>(A),std::move(f)) {}
  private:
  using uauf::connect,uauf::undo,uauf::get_state,uauf::snapshot,uauf::rollback;
};

semionline_dynamic_connectivity(int,int) -> semionline_dynamic_connectivity<void,void>;

template<class F>
semionline_dynamic_connectivity(int,int,F) -> semionline_dynamic_connectivity<remove_cvref_t<arg_type(0,merge,F)>,F>;

template<class F,input_iterator I>
semionline_dynamic_connectivity(I,I,int,F) -> semionline_dynamic_connectivity<iter_value_t<I>,F>;

template<class F,rngs::range C>
semionline_dynamic_connectivity(C&&,int,F) -> semionline_dynamic_connectivity<rngs::range_value_t<C>,F>;