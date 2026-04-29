#pragma once
#include "assets/stdc++.hpp"
#include "sequence/dynamic_segment_tree.hpp"

template<typename T,typename U,class F>
struct patricia_segment_tree_node {
  T v;
  U i;
  int d;
  patricia_segment_tree_node *lp,*rp;
  patricia_segment_tree_node() : v(),i(),d(-1),lp(nullptr),rp(nullptr) {}
  patricia_segment_tree_node(T _v) : v(std::move(_v)),i(),d(-1),lp(nullptr),rp(nullptr) {}
  patricia_segment_tree_node(U _i,int _d) : v(),i(_i),d(_d),lp(nullptr),rp(nullptr) {}
  patricia_segment_tree_node(U x,int y,T z) : v(std::move(z)),i(x),d(y),lp(nullptr),rp(nullptr) {}
  ~patricia_segment_tree_node(){
    if (lp!=nullptr) delete lp;
    if (rp!=nullptr) delete rp;
  }
  void 
  T ptr_v(patricia_segment_tree_node* p,U s,F f){
    if (p!=nullptr) return p->v;
    if constexpr (invocable_r(T,ids,F,U)) return f.ids(s);
    return f.id();
  }
  T& set(U l,U r,U x,const T& _v,F& f){
    if (l+1==r) return v = _v;
    U mid = l+(r-l)/2;
    if (x<mid){
      if (lp==nullptr) lp = new patricia_segment_tree_node();
      return v = f.merge(lp->set(l,mid,x,_v,f),ptr_v(rp,r-mid,f));
    } else {
      if (rp==nullptr) rp = new patricia_segment_tree_node();
      return v = f.merge(ptr_v(lp,mid-l,f),rp->set(mid,r,x,_v,f));
    }
  }
  void get(U l,U r,U x,T& res){
    if (l+1==r){
      res = v;
      return;
    }
    U mid = l+(r-l)/2;
    if (x<mid&&lp!=nullptr) lp->get(l,mid,x,res);
    if (mid<=x&&rp!=nullptr) rp->get(mid,r,x,res);
  }
  template<class P>
  void add(U l,U r,U x,F& f,P& g){
    if constexpr (invocable_r<T,P,T&,U>) v = g(v,r-l);
    else v = g(v);
    if (l+1==r) return;
    U mid = l+(r-l)/2;
    if (x<mid){
      if (lp==nullptr){
        if constexpr (invocable_r(T,ids,F,U)) lp = new patricia_segment_tree_node(f.ids(mid-l));
        else lp = new patricia_segment_tree_node(f.id());
      }
      lp->add(l,mid,x,f,g);
    } else {
      if (rp==nullptr){
        if constexpr (invocable_r(T,ids,F,U)) rp = new patricia_segment_tree_node(f.ids(r-mid));
        else rp = new patricia_segment_tree_node(f.id());
      }
      rp->add(mid,r,x,f,g);
    }
  }
  T query(U l,U r,U ll,U rr,F& f){
    if (ll<=l&&r<=rr) return v;
    U mid = l+(r-l)/2;
    if (rr<=mid){
      if (lp==nullptr){
        if constexpr (invocable_r(T,ids,F,U)) return f.ids(rr-max(ll,l));
        return f.id();
      }
      return lp->query(l,mid,ll,rr,f);
    }
    if (mid<=ll){
      if (rp==nullptr){
        if constexpr (invocable_r(T,ids,F,U)) return f.ids(min(rr,r)-ll);
        return f.id();
      }
      return rp->query(mid,r,ll,rr,f);
    }
    T ret;
    if (lp!=nullptr) ret = lp->query(l,mid,ll,rr,f);
    else if constexpr (invocable_r(T,ids,F,U)) ret = f.ids(mid-max(ll,l));
    else ret = f.id();
    if (rp!=nullptr) return f.merge(ret,rp->query(mid,r,ll,rr,f));
    if constexpr (invocable_r(T,ids,F,U)) return f.merge(ret,f.ids(min(rr,r)-mid));
    return f.merge(ret,f.id());
  }
};

template<typename T,typename U,class F>
using patricia_segment_tree = dynamic_segment_tree<T,U,F,patricia_segment_tree_node<T,U,F>>;