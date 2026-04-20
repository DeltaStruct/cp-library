#include "assets/stdc++.hpp"

template<typename T = void>
struct tree : private vector<pair<int,T>> {
  using sup = vector<pair<int,T>>;
  using sup::size,sup::empty,sup::resize;
};

template<>
struct tree<void> : private vector<int> {
  using sup = vector<int>;
  using sup::vector;
  using sup::operator[],sup::at,sup::data,sup::front,sup::back;
  using sup::begin,sup::end,sup::rbegin,sup::rend;
  using sup::size,sup::empty,sup::resize;
  using sup::push_back,sup::emplace_back,sup::assign,sup::pop_back,sup::clear,sup::erase;
};