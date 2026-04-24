#include "assets/stdc++.hpp"
#include "assets/random.hpp"

template<class N>
requires has_memvar(sz,N)&&has_memvar(v,N)
struct rbst_base {
  using node = N*;
  using T = decltype(declval<N>().v);
  void push_from_root(node x) requires has_memvar_t(node,pp,N)&&invocable(push,N) {
    if (x->pp!=nullptr) push_from_root(x->pp);
    x->push();
  }
  template<class P = decltype(rnd32)>
  requires invocable<P>
  node merge(node l,node r,P& rnd = rnd32){
    if (l==nullptr) return r;
    if (r==nullptr) return l;
    if ((int)rnd()%(l->sz+r->sz)<l->sz){
      if constexpr (invocable(push,N)) l->push();
      l->rp = merge(l->rp,r,rnd);
      l->merge();
      return l;
    }
    if constexpr (invocable(push,N)) r->push();
    r->lp = merge(l,r->lp,rnd);
    r->merge();
    return r;
  }
  template<class P = decltype(rnd32)>
  requires invocable<P>
  node merge(node l,node m,node r,P& rnd = rnd32){
    return merge(merge(l,m,rnd),r,rnd);
  }
  template<class C>
  requires invocable_r<bool,C,T&,T&>
  node lower_bound(node x,T& y,C comp = less<>()){
    if (x==nullptr) return nullptr;
    if constexpr (invocable(push,N)) x->push();
    if (comp(x->v,y)) return lower_bound(x->rp,y,move(comp));
    node ret = lower_bound(x->lp,y,comp);
    if (ret==nullptr&&!comp(y,x->v)) return x;
    return ret;
  }
  template<class C>
  requires invocable_r<bool,C,T&,T&>
  node upper_bound(node x,T& y,C comp = less<>()){
    return lower_bound(x,y,[&](T& a,T& b){ return !comp(b,a); });
  }
  pair<node,node> split(node y) requires has_memvar_t(node,pp,N) {
    assert(y!=nullptr);
    if constexpr (has_memvar_t(node,pp,N)&&invocable(push,N)) push_from_root(y);
    node a = nullptr,b = y;
    for (;y->pp!=nullptr;y = y->pp){
      if (y->pp->lp==y) y->pp->lp = b,b = y->pp;
      else y->pp->rp = a,a = y->pp;
      y->pp->merge();
    }
    return make_pair(a,b);
  }
  pair<node,node> split(node x,node y){
    if (y==nullptr) return make_pair(x,nullptr);
    return split(y);
  }
  pair<node,node> front(node x,int n){
    if (x==nullptr) return make_pair(nullptr,nullptr);
    if (n<=0) return make_pair(nullptr,x);
    if (size(x)<=n) return make_pair(x,nullptr);
    if constexpr (invocable(push,N)) x->push();
    if (x->lp!=nullptr&&x->lp->sz>=n){
      auto [a,b] = front(x->lp,n);
      x->lp = b;
      x->merge();
      return make_pair(a,x);
    }
    auto [a,b] = front(x->rp,n-(x->lp!=nullptr?x->lp->sz:0)-1);
    x->rp = a;
    x->merge();
    return make_pair(x,b);
  }
  pair<node,node> back(node x,int n){
    return front(x,size(x)-n);
  }
  tuple<node,node,node> range(node x,int l,int r){
    auto [c,d] = front(x,r);
    auto [a,b] = front(c,l);
    return make_tuple(a,b,d);
  }
  node nth(node x,int n){
    if (x==nullptr) return nullptr;
    if (x->lp!=nullptr&&x->lp->sz>n) return nth(x->lp,n);
    if (x->lp!=nullptr) n -= x->lp->sz;
    if (n==0) return x;
    return nth(x->rp,n-1);
  }
  template<class C>
  requires has_memvar(v,N)&&invocable_r<bool,C,T&,T&>
  node insert(node x,node y,C comp = less<>()){
    auto [a,b] = split(lower_bound(x,y->v,comp));
    return merge(a,y,b);
  }
  pair<node,node> pop(node x) requires has_memvar_t(node,pp,N) {
    assert(x!=nullptr);
    if constexpr (invocable(push,N)) x->push();
    node y = merge(x->lp,x->rp);
    if (x->pp!=nullptr){
      (x->pp->lp==x?x->pp->lp:x->pp->rp) = y;
      x->pp->merge();
      y = top(y);
    }
    x->pp = x->lp = x->rp = nullptr;
    x->merge();
    return make_pair(y,x);
  }
  node erase(node x){
    auto [a,b] = pop(x);
    delete b;
    return a;
  }
  int size(node x){
    if (x==nullptr) return 0;
    return x->sz;
  }
  T& value(node x) requires has_memvar(v,N) {
    assert(valid(x));
    if constexpr (invocable(push,N)) x->push();
    return x->v;
  }
  decltype(declval<node>()->sum)& sum(node x) requires has_memvar(sum,N) {
    assert(valid(x));
    if constexpr (invocable(push,N)) x->push();
    return x->sum;
  }
  node top(node x) requires has_memvar_t(node,pp,N) {
    assert(valid(x));
    while(x->pp!=nullptr) x = x->pp;
    return x;
  }
  node left(node x){
    assert(valid(x));
    return x->lp;
  }
  node right(node x){
    assert(valid(x));
    return x->rp;
  }
  bool valid(node x){
    return (x!=nullptr);
  }
  node dummy(){
    return nullptr;
  }
};