#pragma once
#include "bit/bit_log.hpp"
#include "graph/union_find.hpp"

template<typename T,class F>
struct range_parallel_union_find : public union_find<T,F> {
  using base = union_find<T,F>;
  using base::base;
  vector<fast_union_find> uf = vector(bit_floor_log(base::size()),fast_union_find(base::size()));
  void connect_internal(int x,int y,int w){
    if (w==-1) base::connect(x,y);
    else if (uf[w].connect(x,y)) connect_internal(x,y,w-1),connect_internal(x+(1<<w),y+(1<<w),w-1);
  }
  void connect(int x,int y,int d){
    if (d==0) return;
    int s = bit_floor_log(d);
    connect_internal(x,y,s-1),connect_internal(x+d-(1<<s),y+d-(1<<s),s-1);
  }
};