#pragma once

template<typename T = void,typename U = int,class F = void>
struct partial_persistent_union_find;

template<>
struct partial_persistent_union_find<void,void> {
  int n;
  int *par,*sz,*tm;
  partial_persistent_union_find(int _n) : n(_n),par(new int[n](-1)),sz(new int[n](1)),tm(new int[n]) {}
  struct union_find {
    partial_persistent_union_find& uf;
    int time;
  };
};

template<typename U>
auto make_partial_persistent_union_find(int n,auto&& f){
  return partial_persistent_union_find<remove_cvref_t<arg_type(0,merge,decltype(f))>,U,decltype(f)>(n,std::forward<decltype(f)>(f));
}