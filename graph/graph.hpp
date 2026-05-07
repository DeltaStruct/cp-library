#pragma once
#include "assets/stdc++.hpp"
#include <ranges>

template<typename T = void>
struct edge;

template<>
struct edge<void> {
  int to,idx;
  edge(int _to,int _idx) : to(_to),idx(_idx) {}
  operator int(){
    return to;
  }
  operator int() const {
    return to;
  }
  struct cost_t {};
  cost_t cost{};
};

template<typename T>
struct edge : edge<void> {
  using base = edge<void>;
  using cost_t = T;
  T cost;
  edge(int _to,int _idx,T _cost) : base(_to,_idx),cost(_cost) {}
};

template<typename T>
struct std::tuple_size<edge<T>> : integral_constant<size_t,2> {};

template<typename T>
struct std::tuple_element<0,edge<T>> {
  using type = int;
};

template<typename T>
struct std::tuple_element<1,edge<T>> {
  using type = typename edge<T>::cost_t;
};
template<size_t I,typename T>
decltype(auto) get(const edge<T>& e){
  if constexpr (I==0) return e.to;
  else if constexpr (I==1) return e.cost;
  else static_assert(false_v<T>);
}

template<size_t I,typename T>
decltype(auto) get(edge<T>& e){
  if constexpr (I==0) return e.to;
  else if constexpr (I==1) return e.cost;
  else static_assert(false_v<T>);
}

template<typename T = void,class Edge = edge<T>>
struct edges : private vector<Edge> {
  using base = vector<Edge>;
  using self = edges;
  using base::base;
  using base::operator[],base::at,base::data,base::front,base::back;
  using base::size,base::resize,base::empty,base::capacity,base::reserve,base::shrink_to_fit;
  using base::assign,base::insert,base::emplace,base::erase,base::swap,base::clear;
  using base::push_back,base::emplace_back,base::pop_back;
  using base::begin,base::end,base::rbegin,base::rend;
  auto to(){
    return (*this)|views::keys;
  }
  auto to() const {
    return (*this)|views::keys;
  }
  auto cost(){
    return (*this)|views::values;
  }
  auto cost() const {
    return (*this)|views::values;
  }
};

template<typename T = void,class Edges = edges<T>>
struct graph : private vector<Edges> {
  using base = vector<Edges>;
  using self = graph;
  using base::base;
  using base::operator[],base::at,base::data,base::front,base::back;
  using base::size,base::resize,base::empty,base::capacity,base::reserve,base::shrink_to_fit;
  using base::assign,base::insert,base::emplace,base::erase,base::swap,base::clear;
  using base::push_back,base::emplace_back,base::pop_back;
  using base::begin,base::end,base::rbegin,base::rend;
  int idx = 0;
  template<typename... Args>
  void emplace_directed(int a,int b,Args&&... args){
    operator[](a).emplace_back(b,idx++,T(std::forward<Args>(args)...));
  }
  void emplace_directed(int a,int b) requires same_as<T,void> {
    operator[](a).emplace_back(b,idx++);
  }
  template<typename... Args>
  void emplace_undirected(int a,int b,Args&&... args){
    operator[](a).emplace_back(b,idx,T(std::forward<Args>(args)...));
    operator[](b).emplace_back(a,idx++,operator[](a).back().cost);
  }
  void emplace_undirected(int a,int b) requires same_as<T,void> {
    operator[](a).emplace_back(b,idx),operator[](b).emplace_back(a,idx++);
  }
  auto to(int x){
    return operator[](x).to();
  }
  auto to(int x) const {
    return operator[](x).to();
  }
  auto cost(int x){
    return operator[](x).cost();
  }
  auto cost(int x) const {
    return operator[](x).cost();
  }
};

graph() -> graph<void>;
graph(int) -> graph<void>;