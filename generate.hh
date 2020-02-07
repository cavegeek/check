#ifndef INCLUDED_GENERATE_HH
#define INCLUDED_GENERATE_HH

#include <random>

//! Generates a random example of type V of a given size (0 to 100).
/** 0 generates a random important value, like NaN or 0. */
template <typename V> V generate(std::default_random_engine &, short size);

template <> bool generate<bool>(std::default_random_engine &, short size);
template <>
unsigned generate<unsigned>(std::default_random_engine &, short size);
template <> int generate<int>(std::default_random_engine &, short size);
template <> float generate<float>(std::default_random_engine &, short size);
template <> double generate<double>(std::default_random_engine &, short size);

// Below is the implementations

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
  if(size > 0) {
    int s = static_cast<int>(size);
    return std::uniform_int_distribution<int>{-(s * s), s * s}(rand);
  } else {
    return random_of(
        rand,
        std::experimental::make_array(
            0,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()));
  }
}

template <>
float generate<float>(std::default_random_engine & rand, short size) {
  if(size > 0) {
    float s = static_cast<float>(size);
    return std::uniform_real_distribution<float>{-(s * s), s * s}(rand);
  } else {
    return random_of(
        rand,
        std::experimental::make_array(
            0.f,
            -0.f,
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::infinity(),
            -std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::quiet_NaN()));
  }
}

template <>
double generate<double>(std::default_random_engine & rand, short size) {
  if(size > 0) {
    double s = static_cast<double>(size);
    return std::uniform_real_distribution<double>{-(s * s), s * s}(rand);
  } else {
    return random_of(
        rand,
        std::experimental::make_array(
            0.f,
            -0.f,
            std::numeric_limits<double>::min(),
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::infinity(),
            -std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::quiet_NaN()));
  }
}

#endif
