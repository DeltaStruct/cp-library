#pragma once
#include "assets/stdc++.hpp"

template<typename T>
struct runtime_array {
  int sz;
  T* ptr;
  cex runtime_array() : sz(0),ptr(nullptr) {}
  cex runtime_array(int n) : sz(n),ptr(new T[n]) {}
  cex runtime_array(int n,T x) : sz(n),ptr(new T[n](x)) {}
  template<input_iterator I>
  cex runtime_array(I first,I last) : sz((int)distance(first,last)),ptr(new T[sz]) {
    copy(first,last,ptr);
  }
  cex runtime_array(const runtime_array<T>& A) : sz(A.sz),ptr(new T[A.sz]) {
    copy(A.ptr,A.ptr+sz,ptr);
  }
  cex runtime_array(runtime_array<T>&& A) : sz(A.sz),ptr(A.ptr) {
    A.ptr = nullptr,A.sz = 0;
  }
  cex runtime_array(initializer_list<T> A) : sz(A.size()),ptr(new T[A.size()]) {
    copy(A.begin(),A.end(),ptr);
  }
  template<rngs::range C>
  cex runtime_array(C&& A) : sz((int)rngs::distance(A)),ptr(new T[sz]) {
    rngs::copy(A,ptr);
  }
  cex ~runtime_array(){
    clear();
  }
  cex runtime_array& operator=(const runtime_array& A){
    clear();
    sz = A.size(),ptr = new T[sz];
    copy(A.begin(),A.end(),ptr);
    return *this;
  }
  cex runtime_array& operator=(runtime_array&& A){
    clear();
    sz = A.sz,ptr = A.ptr,A.sz = 0,A.ptr = nullptr;
    return *this;
  }
  cex T& operator[](int x){
    return ptr[x];
  }
  cex T& at(int x){
    assert(0<=x<sz);
    return ptr[x];
  }
  cex T* data(){
    return ptr;
  }
  cex T& front(){
    return *ptr;
  }
  cex T& back(){
    return ptr[sz-1];
  }
  void swap(runtime_array& A){
    std::swap(ptr,A.ptr),std::swap(sz,A.sz);
  }
  cex void clear(){
    if (ptr!=nullptr){
      delete[] ptr;
      ptr = nullptr,sz = 0;
    }
  }
  cex int size(){
    return sz;
  }
  cex bool empty(){
    return (sz==0);
  }
  using iterator = T*;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  iterator begin(){
    return ptr;
  }
  iterator end(){
    return ptr+sz;
  }
  r_iter_set;
};

template<>
struct runtime_array<void> : public runtime_array<nvoid> {};

template<typename T>
cex bool operator==(const runtime_array<T>& A,const runtime_array<T>& B){
  return std::equal(A.begin(),A.end(),B.begin(),B.end());
}

template<typename T>
cex decltype(compare_three_way(declval<T>(),declval<T>())) operator<=>(const runtime_array<T>& A,const runtime_array<T>& B){
  return std::lexicographical_compare_three_way(A.begin(),A.end(),B.begin(),B.end());
}

template<typename T>
cex vector<T> to_vector(const runtime_array<T>& A){
  vector<T> R(A.begin(),A.end());
  return R;
}

template<typename T>
runtime_array(int,T) -> runtime_array<T>;

template<input_iterator I>
runtime_array(I,I) -> runtime_array<iter_value_t<I>>;

template<rngs::range C>
runtime_array(C&&) -> runtime_array<rngs::range_value_t<C>>;