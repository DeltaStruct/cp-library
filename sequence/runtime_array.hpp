#pragma once
#include "assets/stdc++.hpp"
#include <memory>
#include <type_traits>

template<typename T>
struct runtime_array {
  int sz;
  T* ptr;
  T* uinit_alloc(){
    return (T*)(::operator new(sizeof(T)*sz));
  }
  runtime_array() : sz(0),ptr(nullptr) {}
  runtime_array(int n) : sz(n),ptr(uinit_alloc()) {
    if constexpr (is_default_constructible_v<T>) uninitialized_value_construct_n(ptr,sz);
    else static_assert(false_v<T>);
  }
  runtime_array(int n,T x) : sz(n),ptr(uinit_alloc()) {
    uninitialized_fill_n(ptr,sz,x);
  }
  template<input_iterator I>
  runtime_array(I first,I last) : sz((int)distance(first,last)),ptr(uinit_alloc()) {
    uninitialized_copy(first,last,ptr);
  }
  runtime_array(const runtime_array<T>& A) : sz(A.sz),ptr(uinit_alloc()) {
    uninitialized_copy_n(A.ptr,sz,ptr);
  }
  runtime_array(runtime_array<T>&& A) : sz(A.sz),ptr(A.ptr) {
    A.ptr = nullptr,A.sz = 0;
  }
  runtime_array(initializer_list<T> A) : sz(A.size()),ptr(uinit_alloc()) {
    uninitialized_copy(A.begin(),A.end(),ptr);
  }
  template<rngs::range C>
  runtime_array(C&& A) : sz((int)rngs::distance(A)),ptr(uinit_alloc()) {
    uninitialized_copy(rngs::begin(A),rngs::end(A),ptr);
  }
  ~runtime_array(){
    clear();
  }
  void fill(T v){
    fill_n(ptr,sz,v);
  }
  runtime_array& operator=(const runtime_array& A){
    clear();
    sz = A.size(),ptr = uinit_alloc();
    uninitialized_copy(A.begin(),A.end(),ptr);
    return *this;
  }
  runtime_array& operator=(runtime_array&& A){
    clear();
    sz = A.sz,ptr = A.ptr,A.sz = 0,A.ptr = nullptr;
    return *this;
  }
  template<typename... Args>
  void assign(Args&&... args){
    (*this) = runtime_array<T>(std::forward<Args>(args)...);
  }
  operator vector<T>(){
    return vector<T>(ptr,ptr+sz);
  }
  operator vector<T>() const {
    return vector<T>(ptr,ptr+sz);
  }
  T& operator[](int x){
    return ptr[x];
  }
  const T& operator[](int x) const {
    return ptr[x];
  }
  T& at(int x){
    assert(0<=x&&x<sz);
    return ptr[x];
  }
  const T& at(int x) const {
    assert(0<=x&&x<sz);
    return ptr[x];
  }
  T* data(){
    return ptr;
  }
  const T* data() const {
    return ptr;
  }
  T& front(){
    return *ptr;
  }
  const T& front() const {
    return *ptr;
  }
  T& back(){
    return ptr[sz-1];
  }
  const T& back() const {
    return ptr[sz-1];
  }
  void swap(runtime_array& A){
    std::swap(ptr,A.ptr),std::swap(sz,A.sz);
  }
  void clear(){
    if (ptr!=nullptr){
      destroy_n(ptr,sz);
      ::operator delete(ptr);
      ptr = nullptr,sz = 0;
    }
  }
  int size() const {
    return sz;
  }
  bool empty() const {
    return (sz==0);
  }
  using iterator = T*;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  iterator begin(){
    return ptr;
  }
  iterator begin() const {
    return ptr;
  }
  iterator end(){
    return ptr+sz;
  }
  iterator end() const {
    return ptr+sz;
  }
  r_iter_set;
};

template<>
struct runtime_array<void> : public runtime_array<nvoid> {};

template<typename T>
bool operator==(const runtime_array<T>& A,const runtime_array<T>& B){
  return std::equal(A.begin(),A.end(),B.begin(),B.end());
}

template<typename T>
decltype(compare_three_way(declval<T>(),declval<T>())) operator<=>(const runtime_array<T>& A,const runtime_array<T>& B){
  return std::lexicographical_compare_three_way(A.begin(),A.end(),B.begin(),B.end());
}

template<typename T>
vector<T> to_vector(const runtime_array<T>& A){
  vector<T> R(A.begin(),A.end());
  return R;
}

template<typename T>
runtime_array(int,T) -> runtime_array<T>;

template<input_iterator I>
runtime_array(I,I) -> runtime_array<iter_value_t<I>>;

template<rngs::range C>
runtime_array(C&&) -> runtime_array<rngs::range_value_t<C>>;