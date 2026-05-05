#pragma once
#include "assets/tag.hpp"

#define grp0(x,...) tuple_element_t<0,GuT> x; __VA_OPT__(grp1(__VA_ARGS__))
#define grp1(x,...) tuple_element_t<1,GuT> x; __VA_OPT__(grp2(__VA_ARGS__))
#define grp2(x,...) tuple_element_t<2,GuT> x; __VA_OPT__(grp3(__VA_ARGS__))
#define grp3(x,...) tuple_element_t<3,GuT> x; __VA_OPT__(grp4(__VA_ARGS__))
#define grp4(x,...) tuple_element_t<4,GuT> x; __VA_OPT__(grp5(__VA_ARGS__))
#define grp5(x,...) tuple_element_t<5,GuT> x; __VA_OPT__(grp6(__VA_ARGS__))
#define grp6(x,...) tuple_element_t<6,GuT> x; __VA_OPT__(grp7(__VA_ARGS__))
#define grp7(x,...) tuple_element_t<7,GuT> x; __VA_OPT__(grp8(__VA_ARGS__))
#define grp8(x,...) tuple_element_t<8,GuT> x; __VA_OPT__(grp9(__VA_ARGS__))

#define guide(...) [](auto&&... args){ using GuT = tuple<remove_cvref_t<decltype(args)>...>; struct guide { __VA_OPT__(grp0(__VA_ARGS__)) static constexpr bool tag_raised(no_template_tag x){ if constexpr (requires { decltype(declval<guide>().tag)::raised(x.val); }) return decltype(declval<guide>().tag)::raised(x.val); return 0; } }; return guide{std::forward<decltype(args)>(args)...}; }