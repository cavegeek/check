#include "generate.hh"

#include <limits>
#include <random>

namespace {
  template <typename Num> struct Bound {
    static Num min(short size) {
      return (std::numeric_limits<Num>::lowest() / static_cast<Num>(100)) *
          static_cast<Num>(size);
    }
    static Num max(short size) {
      return (std::numeric_limits<Num>::max() / static_cast<Num>(100)) *
          static_cast<Num>(size);
    }
  };
}

template <> bool generate<bool>(std::default_random_engine & rand, short) {
  return std::bernoulli_distribution{0.5}(rand);
}

template <>
unsigned generate<unsigned>(std::default_random_engine & rand, short size) {
  return std::uniform_int_distribution<unsigned>{
      0, Bound<unsigned>::max(size)}(rand);
}

template <> int generate<int>(std::default_random_engine & rand, short size) {
  return std::uniform_int_distribution<int>{
      Bound<int>::min(size), Bound<int>::max(size)}(rand);
}

template <>
float generate<float>(std::default_random_engine & rand, short size) {
  return std::uniform_real_distribution<float>{
      Bound<float>::min(size), Bound<float>::max(size)}(rand);
}

template <>
double generate<double>(std::default_random_engine & rand, short size) {
  return std::uniform_real_distribution<double>{
      Bound<double>::min(size), Bound<double>::max(size)}(rand);
}
