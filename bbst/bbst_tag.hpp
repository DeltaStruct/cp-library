#pragma once
#include "assets/tag.hpp"

template<unsigned long long x>
using bbst_tag = tag_base<x>;
constexpr bbst_tag bbst_size_tag         = make_tag(0);
constexpr bbst_tag bbst_reversible_tag   = make_tag(1);
constexpr bbst_tag bbst_value_tag        = make_tag(2);
constexpr bbst_tag bbst_sum_tag          = make_tag(3)|bbst_value_tag;
constexpr bbst_tag bbst_lazy_tag         = make_tag(4)|bbst_sum_tag;
constexpr bbst_tag bbst_vupd_tag         = make_tag(5)|bbst_value_tag;
constexpr bbst_tag bbst_value_update_tag = bbst_vupd_tag;