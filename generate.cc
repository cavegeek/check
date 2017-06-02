#include "generate.hh"

#include <limits>
#include <random>

template <> bool generate<bool>(std::default_random_engine & rand, short) {
  return std::bernoulli_distribution{0.5}(rand);
}

template <>
unsigned generate<unsigned>(std::default_random_engine & rand, short size) {
  unsigned s = static_cast<unsigned>(size);
  return std::uniform_int_distribution<unsigned>{0, s * s}(rand);
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
