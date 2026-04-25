#pragma once
#include "assets/stdc++.hpp"

template<typename T>
requires is_unsigned_v<T>||is_unsigned_v<make_unsigned_t<T>>
constexpr int bit_floor_log(const T& v){
  if constexpr (is_unsigned_v<T>) return countr_zero(bit_floor(v));
  return countr_zero(bit_floor((make_unsigned_t<T>)v));
}

template<typename T>
requires is_unsigned_v<T>||is_unsigned_v<make_unsigned_t<T>>
constexpr int bit_ceil_log(const T& v){
  if constexpr (is_unsigned_v<T>) return countr_zero(bit_ceil(v));
  return countr_zero(bit_ceil((make_unsigned_t<T>)v));
}

function_alias(bit_log,bit_ceil_log);