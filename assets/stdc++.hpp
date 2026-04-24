#pragma once
#include "assets/guide.hpp"
#include <bits/stdc++.h>
#include <concepts>
using namespace std;
namespace rngs = std::ranges;

#define cex constexpr

template<typename R,class F,typename... Args>
concept invocable_r = convertible_to<invoke_result_t<F,Args...>,R>;

#define r_iter_set using reverse_iterator = std::reverse_iterator<iterator>; \
  reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(end()); } \
  reverse_iterator rend() noexcept { return std::make_reverse_iterator(begin()); }

template<class T>
struct function_info {
  using return_type = void;
  using origin_type = void;
  using args_tuple = tuple<>;
};

template<typename R,class F,typename... Args>
struct function_info<R (F::*)(Args...)> {
  using return_type = R;
  using origin_class = F;
  using args_tuple = tuple<Args...>;
};

template<typename R,class F,typename... Args>
struct function_info<R (F::*)(Args...) const> {
  using return_type = R;
  using origin_class = F;
  using args_tuple = tuple<Args...>;
};

template<typename R,typename... Args>
struct function_info<R (*)(Args...)> {
  using return_type = R;
  using origin_class = void;
  using args_tuple = tuple<Args...>;
};

template<class F,class L>
struct function_info<F L::*> : public function_info<F> {};

template<class T>
requires requires { &remove_cvref_t<T>::operator(); }
struct function_info<T> : public function_info<decltype(&remove_cvref_t<T>::operator())> {};

template<class F>
using return_type = typename function_info<F>::return_type;

#define return_type(mem,...) return_type<decltype(&remove_cvref_t<__VA_ARGS__>::mem)>

template<class F>
using origin_class = typename function_info<F>::origin_class;

#define origin_class(mem,...) origin_class<decltype(&remove_cvref_t<__VA_ARGS__>::mem)>

#define invocable(mem,F,...) ([]<class... Args>(){ return requires(F f,Args... args){ f.mem(args...); }; }.template operator()<__VA_ARGS__>())
#define invocable_r(R,mem,F,...) ([]<class... Args>(){ return requires(F f,Args... args){ { f.mem(args...) } -> convertible_to<R>; }; }.template operator()<__VA_ARGS__>())

template<class F>
using args_tuple = typename function_info<F>::args_tuple;

#define args_tuple(mem,...) args_tuple<decltype(&remove_cvref_t<__VA_ARGS__>::mem)>

template<class F>
constexpr int arg_count = tuple_size_v<args_tuple<F>>;

#define arg_count(mem,...) arg_count<decltype(&remove_cvref_t<__VA_ARGS__>::mem)>

template<int i,class F>
using arg_type = tuple_element_t<i,args_tuple<F>>;

#define arg_type(i,mem,...) arg_type<i,decltype(&remove_cvref_t<__VA_ARGS__>::mem)>

template<class T>
struct memfn;

template<typename R,class F,typename... Args>
struct memfn<R (F::*)(Args...)> {
  R (F::* f)(Args...);
  F* self;
  R operator()(Args... args){
    (self->*f)(forward<Args>(args)...);
  }
  memfn(R (F::* g)(Args...),F* s) : f(g),self(s) {}
};

template<typename R,class F,typename... Args>
struct memfn<R (F::*)(Args...) const> {
  R (F::* f)(Args...);
  F* self;
  R operator()(Args... args){
    (self->*f)(forward<Args>(args)...);
  }
  memfn(R (F::* g)(Args...),F* s) : f(g),self(s) {}
};

template<typename R,class F,typename... Args>
memfn(R (F::*)(Args...),F*) -> memfn<R (F::*)(Args...)>;

template<typename R,class F,typename... Args>
memfn(R (F::*)(Args...) const,F*) -> memfn<R (F::*)(Args...) const>;

#define single(...) ([&]{ return __VA_ARGS__; })

auto nop = []{};

struct nvoid { char a; };

template<typename T>
constexpr bool false_v = false;

#define constexpr_if(...) [](auto&& cia,auto&& cib){ if constexpr (__VA_ARGS__) return cia; else return cib; }

#define mem_or_call(mem,obj,...) (constexpr_if(__VA_ARGS__)(obj.*mem,obj))

#define has_memvar(mem,...) (requires(__VA_ARGS__ t){ t.mem; })
#define has_memvar_t(type,mem,...) (has_memvar(mem,__VA_ARGS__)&&convertible_to<decltype(declval<__VA_ARGS__>().mem),type>)

#define function_alias(name,call) struct { template<class... Args> auto operator()(Args&&... args) -> decltype(call(std::forward<Args>(args)...)) { return call(std::forward<Args>(args)...); } } name

#include "assets/zaatu.hpp"
#include "assets/config.hpp"