#pragma once
#include "assets/stdc++.hpp"

template<typename T,typename U,class F>
requires invocable_r(T,merge,F,T,T)&&(invocable_r(T,id,F)||invocable_r(T,ids,F,U))
struct dynamic_segment_tree {
  struct node {
    T v;
    node *lp,*rp;
    node() : v(),lp(nullptr),rp(nullptr) {}
    node(T _v) : v(std::move(_v)),lp(nullptr),rp(nullptr) {}
    T ptr_v(node* p,int s,F f){
      if (p!=nullptr) return p->v;
      if constexpr (invocable_r(T,ids,F,U)) return f.ids(s);
      return f.id();
    }
    T& set(U l,U r,U x,const T& _v,F& f){
      if (l+1==r) return v = _v;
      U mid = (l+r)/2;
      if (x<mid){
        if (lp==nullptr) lp = new node();
        return v = f.merge(lp->set(l,mid,x,_v,f),ptr_v(rp,r-mid,f));
      } else {
        if (rp==nullptr) rp = new node();
        return v = f.merge(ptr_v(lp,mid-l,f),rp->set(mid,r,x,_v,f));
      }
    }
    void get(U l,U r,U x,T& res){
      if (l+1==r){
        res = v;
        return;
      }
      U mid = (l+r)/2;
      if (x<mid&&lp!=nullptr) lp->get(l,mid,x);
      if (mid<=x&&rp!=nullptr) rp->get(mid,r,x);
    }
    template<class P>
    void add(U l,U r,U x,P& f){
      if constexpr (invocable_r<T,P,T&,U>) v = f(v,r-l);
      else v = f(v);
      U mid = (l+r)/2;
      if (x<mid) add(l,mid,x,f);
      else add(mid,r,x,f);
    }
    T query(U l,U r,U ll,U rr,F& f){
      if (ll<=l&&r<=rr) return v;
      U mid = (l+r)/2;
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
        return rp->query(mid,rr,ll,rr,f);
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
  U n;
  node* p;
  F f;
  dynamic_segment_tree(U _n,F _f) : n(_n),p(nullptr),f(std::move(_f)) {
    if constexpr (invocable_r(T,ids,F,U)) p = new node(f.ids(n));
    else p = new node(f.id());
  }
  void set(U x,const T& v){
    p->set(0,n,x,v,f);
  }
  const T get(U x){
    T ret;
    if constexpr (invocable_r(T,ids,F,U)) ret = f.ids((U)1);
    else ret = f.id();
    p->get(0,n,x,ret);
  }
  const T operator[](U x){
    return get(x);
  }
  template<class P>
  requires invocable_r<T,P,T&>||invocable_r<T,P,T&,U>
  void add(U x,P _f){
    p->add(0,n,x,_f);
  }
  void add(U x,const T& _v) requires invocable_r(T,add,T&,T)||invocable_r(T,add,T&,T,U) {
    if constexpr (invocable_r(T,add,T&,T,U)) add(x,[&](T& a,U s) -> T { return f.add(a,_v,s); });
    else add(x,[&](T& a) -> T { return f.add(a,_v); });
  }
  T query(int l,int r){
    return p->query(0,n,l,r,f);
  }
};

template<typename U,class F>
dynamic_segment_tree(U,F) -> dynamic_segment_tree<return_type(merge,F),U,F>;