template<typename T,class C = less<T>>
struct skew_heap {
  struct node_t {
    T key;
    node_t *lp,*rp;
  };
  using node = node_t*;
  C comp;
  node new_heap(T v){
    return node_t{v,nullptr,nullptr};
  }
  node meld(node a,node b){
    if (a==nullptr) return b;
    if (b==nullptr) return a;
    if (!comp(a->key,b->key)) swap(a,b);
    a->rp = meld(a->rp,b);
    swap(a->lp,a->rp);
    return a;
  }
  node push(node x,T v){
    return meld(x,new_heap(std::move(v)));
  }
  T top(node x){
    return x->key;
  }
  node pop(node x){
    node ret = meld(x->lp,x->rp);
    delete x;
    return ret;
  }
};