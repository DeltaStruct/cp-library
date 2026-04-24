#include "assets/stdc++.hpp"
#include "graph/graph.hpp"

template<typename T>
struct tree : public graph<T> {};

template<typename T>
struct rooted_tree : public tree<T> {
  int root;
  vector<int> depth,subtree_size;
  void build(){
    for (int y:graph<T>::to(x));
  }
};