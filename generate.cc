#include "generate.hh"

#include <array>
#include <experimental/array>
#include <limits>
#include <random>

template <typename T, size_t n>
T random_of(std::default_random_engine & rand, std::array<T, n> const & vals) {
  static_assert(n > 0, "Cannot return random_of an empty array");
  return vals[std::uniform_int_distribution<size_t>{0, n - 1}(rand)];
}

template <> bool generate<bool>(std::default_random_engine & rand, short) {
  return std::bernoulli_distribution{0.5}(rand);
}

template <>
unsigned generate<unsigned>(std::default_random_engine & rand, short size) {
  if(size > 0) {
    unsigned s = static_cast<unsigned>(size);
    return std::uniform_int_distribution<unsigned>{0, s * s}(rand);
  } else {
    return random_of(
        rand,
        std::experimental::make_array(
            0u, std::numeric_limits<unsigned>::max()));
  };
}

template <> int generate<int>(std::default_random_engine & rand, short size) {
  int s = static_cast<int>(size);
  return std::uniform_int_distribution<int>{-(s * s), s * s}(rand);
}

template <>
float generate<float>(std::default_random_engine & rand, short size) {
  float s = static_cast<float>(size);
  return std::uniform_real_distribution<float>{-(s * s), s * s}(rand);
}

template <>
double generate<double>(std::default_random_engine & rand, short size) {
  double s = static_cast<double>(size);
  return std::uniform_real_distribution<double>{-(s * s), s * s}(rand);
}
