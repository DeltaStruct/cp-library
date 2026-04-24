#include "heap/skew_heap.hpp"

template<typename T>
struct hu_tucker_result_t {
  struct node {
    int v;
    node *lp,*rp;
  };
  T res;
  node tree;
};

template<typename T,class C = less<T>,,class H = skew_heap<T,C>>
T hu_tucker(vector<T> W){
  int n = W.size();
  H heap;
}