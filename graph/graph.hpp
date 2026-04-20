#include "assets/stdc++.hpp"

template<typename T>
struct edge;

template<>
struct edge<void> {
  using type = void;
  int from,to;
  edge(int f,int t) : from(f),to(t) {}
  edge& operator=(const int& x){
    to = x;
    return *this;
  }
  operator int(){
    return to;
  }
  bool operator==(edge& x){
    return (to==x.to&&from==x.from);
  }
  auto operator<=>(edge& x){
    return to<=>x.to;
  }
};

template<typename T>
struct edge : public edge<void> {
  using type = T;
  using super = edge<void>;
  T weight;
  edge(int f,int t,T w) : super(f,t),weight(move(w)) {}
  bool operator==(edge x){
    return (super::operator==(x)&&weight==x.weight);
  }
  auto operator<=>(edge x){
    if (to==x.to) return weight<=>x.weight;
    return super::operator<=>(x);
  }
};

template<typename T>
struct std::tuple_size<graph<T>> : integral_constant<size_t,3> {};

template<size_t i,typename T>
struct std::tuple_element<i,graph<T>> { using type = int; };

template<typename T>
struct std::tuple_element<1,graph<T>> { using type = T; };

template<size_t I,typename T>
tuple_element<I,graph<T>> get(const graph<T>& G){
  if constexpr (I==0) return to;
  else if constexpr (I==1) return weight;
  else if constexpr (I==2) return from;
}

template<size_t I,typename T>
tuple_element<I,graph<T>>& get(graph<T>& G){
  if constexpr (I==0) return to;
  else if constexpr (I==1) return weight;
  else if constexpr (I==2) return from;
}

template<typename T>
edge(int,int,T) -> edge<T>;

template<typename T>
struct graph : private vector<vector<edge<T>>> {
  constexpr bool is_weighted = same_as<T,void>;
  using super = vector<vector<edge<T>>>;
  using super::vector;
  using super::operator[],super::at,super::data,super::front,super::back;
  using super::begin,super::end,super::rbegin,super::rend;
  using super::size,super::empty;
  using super::assign,super::resize,super::push_back,super::emplace_back;
  using super::pop_back,super::erase;
  graph(vector<edge<T>>& E,bool directed = false){
    for (auto& a:E) add(a,directed);
  }
  auto to(int x){ return (*this)[x]|viws::keys; }
  auto weight(int x){ return (*this)[x]|viws::values; }
  auto from(int x){ return (*this)[x]|viws::elements<2>; }
  void add(edge<T> x,bool directed = false){
    if (!directed) (*this)[x.to].emplace_back(edge<T>(x.to,x.from,x.weight));
    (*this)[x.from].emplace_back(move(x));
  }
};