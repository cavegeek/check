#include "generate.hh"

#include <limits>
#include <random>

template <> bool generate_small<bool>(std::default_random_engine & rand) {
  return std::bernoulli_distribution{0.5}(rand);
}

template <>
unsigned generate_small<unsigned>(std::default_random_engine & rand) {
  return std::uniform_int_distribution<unsigned>{0, 10}(rand);
}

template <> int generate_small<int>(std::default_random_engine & rand) {
  return std::uniform_int_distribution<int>{-10, 10}(rand);
}

template <> float generate_small<float>(std::default_random_engine & rand) {
  return std::uniform_real_distribution<float>{-10.f, 10.f}(rand);
}

template <> double generate_small<double>(std::default_random_engine & rand) {
  return std::uniform_real_distribution<double>{-10.f, 10.f}(rand);
}

template <> bool generate_large<bool>(std::default_random_engine & rand) {
  return std::bernoulli_distribution{0.5}(rand);
}

template <>
unsigned generate_large<unsigned>(std::default_random_engine & rand) {
  return std::uniform_int_distribution<unsigned>{
      0, std::numeric_limits<unsigned>::max()}(rand);
}

template <> int generate_large<int>(std::default_random_engine & rand) {
  return std::uniform_int_distribution<int>{
      std::numeric_limits<int>::lowest(),
      std::numeric_limits<int>::max()}(rand);
}

template <> float generate_large<float>(std::default_random_engine & rand) {
  return std::uniform_real_distribution<float>{
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::max()}(rand);
}

template <> double generate_large<double>(std::default_random_engine & rand) {
  return std::uniform_real_distribution<double>{
      std::numeric_limits<double>::lowest(),
      std::numeric_limits<double>::max()}(rand);
}
