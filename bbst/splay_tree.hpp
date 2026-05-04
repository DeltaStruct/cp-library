#pragma once
#include "bbst/bbst_tag.hpp"

template<class F,class Derived>
struct splay_tree_node_base {
  using node = splay_tree_node_base;
  Derived *lp,*rp,*pp;
  F& f;
  splay_tree_node_base(F& _f) : lp(nullptr),rp(nullptr),pp(nullptr),f(_f) {}
  ~splay_tree_node_base(){
    if (lp!=nullptr) delete lp;
    if (rp!=nullptr) delete rp;
  }
  void push(){}
  void update(){}
};

template<typename T,class F,class D,bool Use = F::tag_raised(bbst_value_tag)>
struct splay_tree_value_node_base : splay_tree_node_base<F,D> {
  using base = splay_tree_node_base<F,D>;
  splay_tree_value_node_base(F& _f) : base(_f) {}
};

template<typename T,class F,class D>
struct splay_tree_value_node_base<T,F,D,true> : splay_tree_node_base<F,D> {
  using base = splay_tree_node_base<F,D>;
  T v;
  template<typename... V>
  splay_tree_value_node_base(F& _f,V&&... _v) : base(_f),v(std::forward<V>(_v)...) {}
};

template<typename T,class F,class D,bool Use = F::tag_raised(bbst_vupd_tag)>
requires (!Use||invocable_r(T,value_composition,F,T&,T&))
struct splay_tree_vupd_node_base : splay_tree_value_node_base<T,F,D> {
  using base = splay_tree_value_node_base<T,F,D>;
  using base::base;
};

template<typename T,class F,class D>
requires invocable_r(T,did,F)
struct splay_tree_vupd_node_base<T,F,D,true> : splay_tree_value_node_base<T,F,D> {
  using base = splay_tree_value_node_base<T,F,D>;
  T vu;
  template<typename... V>
  splay_tree_vupd_node_base(F& _f,V&&... _v) : base(_f,std::forward<V>(_v)...),vu(_f.did()) {}
  void value_update(T u){
    base::v = base::f.value_composition(base::v,u);
    vu = base::f.value_composition(vu,u);
  }
  void push(){
    if constexpr (invocable_r(bool,skip_value_update,F,T&)){
      if (base::f.skip_value_update(vu)) return base::push();
    }
    if (base::lp!=nullptr) base::lp->d = base::f.value_composition(base::lp->d,vu);
    if (base::rp!=nullptr) base::rp->d = base::f.value_composition(base::rp->d,vu);
    vu = base::f.did();
    base::push();
  }
};

template<typename T,class F,class D>
struct splay_tree_vupd_node_base<T,F,D,true> : splay_tree_value_node_base<T,F,D> {
  using base = splay_tree_value_node_base<T,F,D>;
  bool vuf;
  T vu;
  template<typename... V>
  splay_tree_vupd_node_base(F& _f,V&&... _v) : base(_f,std::forward<V>(_v)...),vuf(false),vu() {}
  void value_update(T u){
    base::v = base::f.value_composition(base::v,u);
    if (vuf) vu = base::f.value_composition(vu,u);
    else vu = u,vuf = true;
  }
  void push(){
    if (vuf){
      if (base::lp!=nullptr) base::lp->value_update(vu);
      if (base::rp!=nullptr) base::rp->value_update(vu);
      vuf = false;
    }
    base::push();
  }
};

template<typename T,class F,class D,bool Use = F::tag_raised(bbst_sum_tag)>
requires (!Use||invocable_r(T,merge,F,T&,T&))
struct splay_tree_sum_node_base : splay_tree_vupd_node_base<T,F,D> {
  using base = splay_tree_vupd_node_base<T,F,D>;
  using base::base;
};

template<typename T,class F,class D>
struct splay_tree_sum_node_base<T,F,D,true> : splay_tree_vupd_node_base<T,F,D> {
  using base = splay_tree_vupd_node_base<T,F,D>;
  T sum;
  template<typename... V>
  splay_tree_sum_node_base(F& _f,V&&... _v) : base(_f,std::forward<V>(_v)...),sum(base::v) {}
  void update(){
    if (base::lp!=nullptr) sum = base::f.merge(base::lp->sum,base::v);
    else sum = base::v;
    if (base::rp!=nullptr) sum = base::f.merge(sum,base::rp->sum);
    base::update();
  }
};

template<typename T,typename U,class F,class D,bool Use = F::tag_raised(bbst_lazy_tag)>
requires (!Use||(invocable_r(T,transform,F,T&,U&)&&invocable_r(U,composition,F,U&,U&)))
struct splay_tree_lazy_node_base : splay_tree_sum_node_base<T,F,D> {};

template<typename T,typename U,class F,class D>
requires invocable_r(U,lid,F)
struct splay_tree_lazy_node_base<T,U,F,D,true> : splay_tree_sum_node_base<T,F,D> {
  using base = splay_tree_sum_node_base<T,F,D>;
  U lazy;
  template<typename... V>
  splay_tree_lazy_node_base(F& _f,V&&... _v) : base(_f,std::forward<V>(_v)...),lazy(_f.lid()) {}
  void lazy_update(U u){
    base::sum = base::f.transform(base::sum,u);
    if constexpr (invocable_r(T,value_transform,F,T&,U&)){
      base::v = base::f.value_transform(base::v,u);
    } else base::v = base::f.transform(base::v,u);
    lazy = base::f.composition(lazy,u);
  }
  void push(){
    if constexpr (invocable_r(bool,skip_lazy,F,U&)){
      if (base::f.skip_lazy(lazy)) return base::push();
    }
    if (base::lp!=nullptr) base::lp->lazy = base::f.composition(base::lp->lazy,lazy);
    if (base::rp!=nullptr) base::rp->lazy = base::f.composition(base::rp->lazy,lazy);
    lazy = base::f.lid();
    base::push();
  }
};

template<typename T,typename U,class F,class D>
struct splay_tree_lazy_node_base<T,U,F,D,true> : splay_tree_sum_node_base<T,F,D> {
  using base = splay_tree_sum_node_base<T,F,D>;
  bool lf;
  U lazy;
  template<typename... V>
  splay_tree_lazy_node_base(F& _f,V&&... _v) : base(_f,std::forward<V>(_v)...),lf(false),lazy() {}
  void lazy_update(U u){
    base::sum = base::f.transform(base::sum,u);
    if constexpr (invocable_r(T,value_transform,F,T&,U&)){
      base::v = base::f.value_transform(base::v,u);
    } else base::v = base::f.transform(base::v,u);
    if (lf) lazy = base::f.composition(lazy,u);
    else lazy = u,lf = true;
  }
  void push(){
    if (lf){
      if (base::lp!=nullptr) base::lp->lazy_update(lazy);
      if (base::rp!=nullptr) base::rp->lazy_update(lazy);
      lf = false;
    }
    base::push();
  }
};

template<typename T,typename U,class F,class D,bool Use = F::tag_raised(bbst_size_tag)>
struct splay_tree_size_node_base : splay_tree_lazy_node_base<T,U,F,D> {
  using base = splay_tree_lazy_node_base<T,U,F,D>;
  using base::base;
};

template<typename T,typename U,class F,class D>
struct splay_tree_size_node_base<T,U,F,D,true> : splay_tree_lazy_node_base<T,U,F,D> {
  using base = splay_tree_lazy_node_base<T,U,F,D>;
  using base::base;
  int sz = 1;
  void update(){
    sz = 1;
    if (base::lp!=nullptr) sz += base::lp->sz;
    if (base::rp!=nullptr) sz += base::rp->sz;
    base::update();
  }
};

template<typename T,typename U,class F,class D>
requires invocable(get_size_api,F,int,int,int)
struct splay_tree_size_node_base<T,U,F,D,true> : splay_tree_lazy_node_base<T,U,F,D> {
  using base = splay_tree_lazy_node_base<T,U,F,D>;
  using base::base;
  int sz = 1;
  void push(){
    base::f.get_size_api(sz,(base::lp==nullptr?0:base::lp->sz),(base::rp==nullptr?0:base::rp->sz));
    base::push();
  }
  void update(){
    sz = 1;
    if (base::lp!=nullptr) sz += base::lp->sz;
    if (base::rp!=nullptr) sz += base::rp->sz;
    base::f.get_size_api(sz,(base::lp==nullptr?0:base::lp->sz),(base::rp==nullptr?0:base::rp->sz));
    base::update();
  }
};

template<typename T,typename U,class F,class D,bool Use = F::tag_raised(bbst_reversible_tag)>
struct splay_tree_reversible_node_base : splay_tree_size_node_base<T,U,F,D> {
  using base = splay_tree_size_node_base<T,U,F,D>;
  using base::base;
};

template<typename T,typename U,class F,class D>
struct splay_tree_reversible_node_base<T,U,F,D,true> : splay_tree_size_node_base<T,U,F,D> {
  using base = splay_tree_size_node_base<T,U,F,D>;
  using base::base;
  bool rev = false;
  void reverse(){
    rev ^= 1;
    swap(base::lp,base::rp);
    if constexpr (F::tag_raised(bbst_sum_tag)&&invocable_r(T,reverse_sum,F,T&)){
      base::sum = base::f.reverse_sum(base::sum);
    }
  }
  void push(){
    if (rev){
      if (base::lp!=nullptr) base::lp->reverse();
      if (base::rp!=nullptr) base::rp->reverse();
      rev = false;
    }
    base::push();
  }
};

template<typename T,typename U,class F>
struct splay_tree_node : splay_tree_reversible_node_base<T,U,F,splay_tree_node<T,U,F>> {
  using base = splay_tree_reversible_node_base<T,U,F,splay_tree_node<T,U,F>>;
  using base::base;
};

template<typename T>
struct splay_tree_g {
  template<unsigned long long nTag>
  static constexpr bool tag_raised(bbst_tag<nTag> x){
    return (same_as<T,void>?false:bbst_value_tag.raised(x));
  }
};

template<typename T,typename U= void,class F= splay_tree_g<T>,class Node= splay_tree_node<T,U,F>>
struct splay_tree;

template<class F,class Node>
struct splay_tree<void,void,F,Node> {
  using node = Node*;
  F f;
  splay_tree(F _f) : f(_f) {}
  template<typename... V>
  [[nodiscard]] node make_splay_tree(V&&... v){
    return new Node(f,std::forward<V>(v)...);
  }
  [[nodiscard]] node make_dummy(){
    return nullptr;
  }
  int pos(node p){
    if (p->pp!=nullptr){
      if (p->pp->lp==p) return -1;
      if (p->pp->rp==p) return 1;
    }
    return 0;
  }
  void rot(node p){
    node x = p->pp,y = x->pp;
    if (pos(p)==-1){
      if ((x->lp = p->rp)!=nullptr) p->rp->pp = x;
      p->rp = x,x->pp = p;
    } else {
      if ((x->rp = p->lp)!=nullptr) p->lp->pp = x;
      p->lp = x,x->pp = p;
    }
    x->update(),p->update();
    if ((p->pp = y)!=nullptr){
      if (y->lp==x) y->lp = p;
      if (y->rp==x) y->rp = p;
    }
  }
  bool is_root(node p){
    return p->pp==nullptr||(p->pp->lp!=p&&p->pp->rp!=p);
  }
  node splay(node p){
    if (p==nullptr) return nullptr;
    p->push();
    while(!is_root(p)){
      node x = p->pp;
      if (is_root(x)){
        x->push(),p->push();
        rot(p);
      } else {
        node y = x->pp;
        y->push(),x->push(),p->push();
        rot((pos(x)==pos(p)?x:p)),rot(p);
      }
    }
    return p;
  }
  int size(node x) requires (F::tag_raised(bbst_size_tag)) {
    return (x==nullptr?0:x->sz);
  }
  [[nodiscard]] node splay_at(node p,int x){
    assert(p!=nullptr&&x<size(p));
    p->push();
    if (x==size(p->lp)) return splay(p);
    if (x<size(p->lp)) return splay_at(p->lp,x);
    else return splay_at(p->rp,x-size(p->lp)-1);
  }
  [[nodiscard]] node kth(node p,int x){
    return splay_at(p,x);
  }
  [[nodiscard]] pair<node,node> front(node p,int x) requires (F::tag_raised(bbst_size_tag)) {
    if (p==nullptr) return make_pair(nullptr,nullptr);
    if (x==size(p)) return make_pair(p,nullptr);
    p = splay_at(p,x);
    node ret = p->lp;
    if (ret!=nullptr) ret->pp = nullptr;
    p->lp = nullptr;
    p->update();
    return make_pair(ret,p);
  }
  [[nodiscard]] pair<node,node> back(node p,int x){
    return front(p,size(p)-x);
  }
  [[nodiscard]] node merge(node l,node r){
    if (l==nullptr&&r==nullptr) return nullptr;
    if (l==nullptr) return splay(r);
    if (r==nullptr) return splay(l);
    for (splay(l),splay(r),l->push();l->rp!=nullptr;l->push()) l = l->rp;
    splay(l);
    l->rp = r,r->pp = l;
    l->update();
    return l;
  }
  [[nodiscard]] node merge(node l,node m,node r){
    return merge(merge(l,m),r);
  }
  [[nodiscard]] node isolate(node p){
    p->pp = p->lp = p->rp = nullptr;
    p->update();
    return p;
  }
  [[nodiscard]] pair<node,node> pop(node p){
    assert(p!=nullptr);
    splay(p);
    if (p->lp!=nullptr) return p->lp->pp = nullptr;
    if (p->rp!=nullptr) return p->rp->pp = nullptr;
    node ret = merge(p->lp,p->rp);
    return make_pair(ret,isolate(p));
  }
  [[nodiscard]] node erase(node p){
    auto [a,b] = pop(p);
    delete b;
    return a;
  }
  [[nodiscard]] pair<node,node> pop_at(node p,int x) requires (F::tag_raised(bbst_size_tag)) {
    p = splay_at(p,x);
    if (p->lp!=nullptr) p->lp->pp = nullptr;
    if (p->rp!=nullptr) p->rp->pp = nullptr;
    node ret = merge(p->lp,p->rp);
    return make_pair(ret,isolate(p));
  }
  [[nodiscard]] node erase_at(node p,int x) requires (F::tag_raised(bbst_size_tag)) {
    auto [a,b] = pop_at(p,x);
    delete b;
    return a;
  }
  [[nodiscard]] node insert_at(node p,int x,node i) requires (F::tag_raised(bbst_size_tag)) {
    if (p==nullptr) return i;
    if (x==0){
      p = splay_at(p,0);
      p->lp = i,i->pp = p;
      p->update();
      return p;
    }
    p = splay_at(p,x-1);
    if (p->rp!=nullptr) p->rp->pp = i;
    i->pp = p,i->rp = p->rp,p->rp = i;
    i->update(),p->update();
    return p;
  }
  template<typename... Args>
  [[nodiscard]] node emplace_at(node p,int x,Args&&... args){
    return insert_at(p,x,make_splay_tree(std::forward<Args>(args)...));
  }
  [[nodiscard]] pair<node,int> get_at(node p){
    assert(p!=nullptr);
    splay(p);
    return make_pair(p,size(p->lp));
  }
  [[nodiscard]] tuple<node,node,node> between(node p,int l,int r){
    auto [c,d] = front(p,r);
    auto [a,b] = front(c,l);
    return make_tuple(a,b,d);
  }
  void reverse(node p) requires (F::tag_raised(bbst_reversible_tag)) {
    p->reverse();
  }
  [[nodiscard]] node insert_front(node p,node x){
    return insert_at(p,0,x);
  }
  template<typename... Args>
  [[nodiscard]] node emplace_front(node p,Args... args){
    return emplace_at(p,0,std::forward<Args>(args)...);
  }
  [[nodiscard]] node insert_back(node p,node x){
    return insert_at(p,size(p),x);
  }
  template<typename... Args>
  [[nodiscard]] node emplace_back(node p,Args... args){
    return emplace_at(p,size(p),std::forward<Args>(args)...);
  }
  [[nodiscard]] node insert(node root,node p,node x){
    if (root==nullptr) return x;
    if (p==nullptr) return insert_back(root,x);
    splay(p);
    if (p->lp!=nullptr){
      p->lp = x,x->pp = p;
      p->update();
      return p;
    }
    if ((x->lp = p->lp)!=nullptr) p->lp->pp = x;
    p->lp = x,x->pp = p;
    x->update(),p->update();
    return p;
  }
  template<typename... Args>
  [[nodiscard]] node emplace(node r,node p,Args&&... args){
    return insert(r,p,make_splay_tree(std::forward<Args>(args)...));
  }
};

template<typename T,class F,class Node>
struct splay_tree<T,void,F,Node> : splay_tree<void,void,F,Node> {
  using node = Node*;
  using base = splay_tree<void,void,F,Node>;
  using base::base;
  T& value(node p) requires (F::tag_raised(bbst_value_tag)) {
    p->push();
    return p->v;
  }
  const T& sum(node p) requires (F::tag_raised(bbst_sum_tag)) {
    p->push();
    return p->sum;
  }
  void value_update(node p,T u) requires (F::tag_raised(bbst_value_update_tag)) {
    p->value_update(u);
  }
  template<class P>
  requires (invocable_r<bool,P,T&>&&F::tag_raised(bbst_value_tag))
  [[nodiscard]] node splay_value(node p,P&& cond){
    if (p==nullptr) return nullptr;
    p->push();
    if (cond(p->v)) return splay_value(p->rp,cond);
    node ret = splay_value(p->lp,cond);
    if (ret==nullptr) return splay(p);
    return ret;
  }
  [[nodiscard]] node lower_bound(node p,T v) requires (F::tag_raised(bbst_value_tag)) {
    if constexpr (invocable_r(bool,comp,F,T&,T&)){
      return splay_comp(p,[&](T& x){ return comp(x,v); });
    } else if constexpr (requires (T x){ x < x; }){
      return splay_comp(p,[&](T& x){ return x < v; });
    } else static_assert(false_v<T>);
  }
  [[nodiscard]] node upper_bound(node p,T v) requires (F::tag_raised(bbst_value_tag)) {
    if constexpr (invocable_r(bool,comp,F,T&,T&)){
      return splay_comp(p,[&](T& x){ return !comp(v,x); });
    } else if constexpr (requires (T x){ x < x; }){
      return splay_comp(p,[&](T& x){ return !(v < x); });
    } else static_assert(false_v<T>);
  }
};

template<typename T,typename U,class F,class Node>
struct splay_tree : splay_tree<T,void,F,Node> {
  using node = Node*;
  using base = splay_tree<T,void,F,Node>;
  using base::base;
  void lazy_update(node p,U u) requires (F::tag_raised(bbst_lazy_tag)) {
    p->lazy_update(u);
  }
  void lazy(node p,U u) requires (F::tag_raised(bbst_lazy_tag)) {
    p->lazy_update(u);
  }
};

template<class F>
requires (F::tag_raised(bbst_sum_tag)&&F::tag_raised(bbst_lazy_tag))
splay_tree(F f) -> splay_tree<return_type(merge,F),remove_cvref_t<arg_type(1,transform,F)>,F>;

template<class F>
requires (F::tag_raised(bbst_sum_tag)&&!F::tag_raised(bbst_lazy_tag))
splay_tree(F f) -> splay_tree<return_type(merge,F),void,F>;