template<int mod>
requires (mod>1)
struct static_modint {
  long long v;
  
};

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;