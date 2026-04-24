#include "assets/stdc++.hpp"
#include "bbst/rbst_base.hpp"

template<typename T,class F>
requires invocable_r(T,merge,F,T&,T&)
struct rbst_node {
  T v,sum;
  rbst_node *lp,*rp,*pp;
  int sz;
  F f;
  void merge(){
    sz = 1,sum = v;
    if (lp!=nullptr) sz += lp->sz,sum = f.merge(lp->sum,sum);
    if (rp!=nullptr) sz += rp->sz,sum = f.merge(sum,rp->sum);
  }
};

template<class F>
struct rbst : public rbst_base<rbst_node<return_type(merge,F),F>> {
  using T = return_type(merge,F);
  using base = rbst_base<rbst_node<T,F>>;
  using node = rbst_node<T,F>*;
  F f;
  rbst(F _f) : f(std::move(_f)) {}
  node create(T v){
    return new rbst_node<T,F>{v,v,nullptr,nullptr,nullptr,1,f};
  }
  node insert(node x,T v) requires invocable_r(bool,comp,F,T&,T&) {
    return base::insert(x,create(move(v)),memfn(&F::comp,&f));
  }
  node insert(node x,node y) requires invocable_r(bool,comp,F,T&,T&) {
    return base::insert(x,y,memfn(&F::comp,&f));
  }
  pair<node,node> pop(node x,T v) requires invocable_r(bool,comp,F,T&,T&) {
    node y = lower_bound(x,v,memfn(&F::comp,&f));
    assert(y!=nullptr&&y->v==v);
    return base::pop(x,y);
  }
  node erase(node x,T v) requires invocable_r(bool,comp,F,T&,T&) {
    node y = lower_bound(x,v,memfn(&F::comp,&f));
    assert(y!=nullptr&&y->v==v);
    return base::erase(x,y);
  }
};

template<typename T>
struct ordered_rbst_node {
  T v;
  ordered_rbst_node *lp,*rp,*pp;
  int sz;
  void merge(){
    sz = 1;
    if (lp!=nullptr) sz += lp->sz;
    if (rp!=nullptr) sz += rp->sz;
  }
};

template<typename T,class F = less<T>>
requires invocable_r<bool,F,T&,T&>
struct ordered_rbst : public rbst_base<ordered_rbst_node<T>> {
  using base = rbst_base<ordered_rbst_node<T>>;
  using node = ordered_rbst_node<T>*;
  F f;
  ordered_rbst(F _f = less<>()) : f(std::move(_f)) {}
  node create(T v){
    return new ordered_rbst_node<T>{v,nullptr,nullptr,nullptr,1};
  }
  node insert(node x,T v){
    return base::insert(x,create(v),f);
  }
  node insert(node x,node y){
    return base::insert(x,y,f);
  }
  pair<node,node> pop(node x,T v){
    node y = lower_bound(x,v,f);
    assert(y!=nullptr&&y->v==v);
    return base::pop(x,y);
  }
  node erase(node x,T v){
    node y = lower_bound(x,v,f);
    assert(y!=nullptr&&y->v==v);
    return base::erase(x,y);
  }
};

template<typename T,typename U,class F>
requires invocable_r(T,merge,F,T&,T&)&&invocable_r(U,push,F,U&,U&)
       &&invocable_r(T,apply,F,T&,U&)&&invocable_r(U,lazy_id,F)
struct lazy_rbst_node {
  T v,sum;
  U lazy;
  bool rev;
  lazy_rbst_node *lp,*rp,*pp;
  int sz;
  F f;
  void push(){
    v = f.apply(v,lazy),sum = f.apply(sum,lazy);
    if (rev) swap(lp,rp);
    if (lp!=nullptr) lp->rev ^= rev,lp->lazy = f.push(lp->lazy,lazy);
    if (rp!=nullptr) rp->rev ^= rev,rp->lazy = f.push(rp->lazy,lazy);
    rev = false,lazy = f.lazy_id();
  }
  void merge(){
    sz = 1,sum = v;
    if (lp!=nullptr) sz += lp->sz,sum = f.merge(lp->sum,sum);
    if (rp!=nullptr) sz += rp->sz,sum = f.merge(sum,rp->sum);
  }
};

template<class F>
struct lazy_rbst : public rbst_base<lazy_rbst_node<return_type(merge,F),return_type(lazy_id,F),F>> {
  using T = return_type(merge,F);
  using U = return_type(lazy_id,F);
  using base = rbst_base<lazy_rbst_node<T,U,F>>;
  using node = lazy_rbst_node<T,U,F>*;
  F f;
  lazy_rbst(F _f) : f(std::move(_f)) {}
  node create(T v){
    return new lazy_rbst_node<T,U,F>{v,v,f.lazy_id(),false,nullptr,nullptr,nullptr,1,f};
  }
  void reverse(node x){
    x->rev ^= 1;
  }
  void update(node x,U u){
    x->lazy = f.push(x->lazy,u);
    x->push();
  }
  node insert(node x,T v) requires invocable_r(bool,comp,F,T&,T&) {
    return base::insert(x,create(move(v)),memfn(&F::comp,&f));
  }
  node insert(node x,node y) requires invocable_r(bool,comp,F,T&,T&) {
    return base::insert(x,y,memfn(&F::comp,&f));
  }
  pair<node,node> pop(node x,T v) requires invocable_r(bool,comp,F,T&,T&) {
    node y = lower_bound(x,v,memfn(&F::comp,&f));
    assert(y!=nullptr&&y->v==v);
    return base::pop(x,y);
  }
  node erase(node x,T v) requires invocable_r(bool,comp,F,T&,T&) {
    node y = lower_bound(x,v,memfn(&F::comp,&f));
    assert(y!=nullptr&&y->v==v);
    return base::erase(x,y);
  }
};