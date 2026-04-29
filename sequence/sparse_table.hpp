#pragma once
#include "bit/bit_log.hpp"
#include <iterator>

template<typename T,class F>
requires invocable_r(T,merge,F,T&,T&)
struct sparse_table {
  int n,d;
  T** table;
  F f;
  sparse_table(int _n,F _f) : n(_n),d(bit_floor_log(n)),table(new T*[d+1]),f(std::move(_f)) {
    for (int i(0);i <= d;++i) table[i] = new T[n];
    if constexpr (invocable_r(T,idi,F,int)){
      for (int i(0);i < n;++i) table[0][i] = f.idi(i);
      build();
    }
  }
  template<input_iterator I>
  sparse_table(I a,I b,F _f) : sparse_table((int)distance(a,b),std::move(_f)) {
    copy(a,b,table[0]);
    build();
  }
  template<rngs::range C>
  sparse_table(C&& A,F _f) : sparse_table((int)rngs::distance(A),std::move(_f)) {
    rngs::copy(A,table[0]);
    build();
  }
  ~sparse_table(){
    for (int i(0);i <= d;++i) delete[] table[i];
    delete[] table;
  }
  void build(){
    for (int i(0);i < d;++i) for (int k(0);k <= n-(1<<i);++k){
      table[i+1][k] = f.merge(table[i][k],table[i][k+(1<<i)]);
    }
  }
  T& operator[](int x){
    return table[x];
  }
  T query(int l,int r){
    if constexpr (invocable_r(T,id,F)) if (l==r) return f.id();
    assert(0<=l&&l<=r&&r<=n);
    int s = bit_floor_log(r-l);
    return f.merge(table[s][l],table[s][r-(1<<s)]);
  }
};

template<class F>
sparse_table(int,F) -> sparse_table<return_type(merge,F),F>;

template<class F,input_iterator I>
sparse_table(I,I,F) -> sparse_table<iter_value_t<I>,F>;

template<class F,rngs::range C>
sparse_table(C&&,F) -> sparse_table<rngs::range_value_t<C>,F>;