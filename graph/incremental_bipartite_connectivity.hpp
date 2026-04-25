#pragma once
#include "graph/union_find.hpp"

template<typename T = void,class F = void,class base = union_find<T,F>>
struct incremental_bipartite_connectivity;

template<class base>
struct incremental_bipartite_connectivity<void,void,base> : public base {
  using self = incremental_bipartite_connectivity;
  int n;
  bool flag;
  incremental_bipartite_connectivity(int _n) : base(2*_n),n(_n),flag(false) {}
  bool connect(int x,int y){
    base::connect(x,n+y),base::connect(n+x,y);
    return !(flag |= base::connected(x,n+x));
  }
  bool connected(int x,int y){
    return base::connected(x,y)||base::connected(x,n+y);
  }
  bool bipartite(){
    return !flag;
  }
  bool bipartite(int x){
    return !base::connected(x,n+x);
  }
  bool same_color(int x,int y){
    return !base::connected(x,n+y)||base::connected(x,y);
  }
  bool different_color(int x,int y){
    return !base::connected(x,y)||base::connected(x,n+y);
  }
  int size(){
    return n;
  }
  int size(int x){
    return (base::connected(x,n+x)?base::size(x)>>1:base::size(x));
  }
};

template<typename T,class F,class base>
struct incremental_bipartite_connectivity : public incremental_bipartite_connectivity<void,void,base> {
  using vibc = incremental_bipartite_connectivity<void,void,base>;
  using self = incremental_bipartite_connectivity;
  incremental_bipartite_connectivity(int _n,F f) : base(2*_n,std::move(f)),vibc::n(_n),vibc::flag(false) {}
  template<input_iterator I>
  incremental_bipartite_connectivity(I a,I b,F f) : vibc::n((int)distance(a,b)),base(2*vibc::n,std::move(f)),vibc::flag(false) {
    for (int i(0);i < vibc::n;++i) base::val[i] = base::val[vibc::n+i] = *(a++);
  }
  template<rngs::range C>
  incremental_bipartite_connectivity(C&& A,F f) : self(rngs::begin(A),rngs::end(A),std::move(f)) {}
};

incremental_bipartite_connectivity(int) -> incremental_bipartite_connectivity<void,void>;

template<class F>
incremental_bipartite_connectivity(int,F) -> incremental_bipartite_connectivity<remove_cvref_t<arg_type(0,merge,F)>,F>;

template<class F,input_iterator I>
incremental_bipartite_connectivity(I,I,F) -> incremental_bipartite_connectivity<iter_value_t<I>,F>;

template<class F,rngs::range C>
incremental_bipartite_connectivity(C&&,F) -> incremental_bipartite_connectivity<rngs::range_value_t<C>,F>;