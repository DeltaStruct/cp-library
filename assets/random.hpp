#include <random>
struct pgc32 {
  const unsigned long long MULT,INC;
  unsigned long long value = 123;
};

auto rnd32 = std::mt19937(std::random_device()());