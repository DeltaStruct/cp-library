#pragma once

template<unsigned long long Tag>
struct tag_base {
  static constexpr unsigned long long val(){
    return Tag;
  }
  template<unsigned long long nTag>
  constexpr auto operator|([[maybe_unused]] tag_base<nTag> x) -> tag_base<Tag|nTag> {
    return tag_base<Tag|nTag>();
  };
  template<unsigned long long nTag>
  constexpr auto operator|([[maybe_unused]] tag_base<nTag> x) const -> tag_base<Tag|nTag> {
    return tag_base<Tag|nTag>();
  };
  template<unsigned long long nTag>
  static constexpr bool raised([[maybe_unused]] tag_base<nTag> x){
    return (Tag&nTag)==nTag;
  }
  static constexpr bool raised(unsigned long long x){
    return (Tag&x)==x;
  }
};

struct no_template_tag {
  unsigned long long val;
  template<unsigned long long Tag>
  constexpr no_template_tag([[maybe_unused]] tag_base<Tag> x) : val(Tag) {}
};

#define make_tag(n) (tag_base<1ull<<(n)>())