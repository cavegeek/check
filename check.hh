#ifndef CHECK_HH
#define CHECK_HH

#include <experimental/optional>
#include <experimental/tuple>
#include <iostream>
#include <random>
#include <tuple>
#include <type_traits>

#include "generate.hh"
#include "special.hh"

template <typename... args>
std::experimental::optional<std::tuple<args...>>
    test_with(bool (&func)(args...), args const &... vals) {
  return func(vals...) ? std::experimental::optional<std::tuple<args...>>{}
                       : std::tuple<args...>{vals...};
}

template <typename T> T gen(std::default_random_engine & eng, short size) {
  return generate<typename std::decay<T>::type>(eng, size);
}

template <typename... args>
std::experimental::optional<std::tuple<args...>> test_at(
    std::default_random_engine & eng, bool (&func)(args...), short size) {
  return test_with<args...>(
      func, gen<typename std::decay<args>::type>(eng, size)...);
}

template <typename... args>
std::experimental::optional<std::tuple<args...>>
    test_all(std::default_random_engine & eng, bool (&func)(args...)) {
  for(short size = 0; size < 1000; ++size) {
    auto result = test_at(
        eng,
        func,
        std::uniform_int_distribution<short>{
            0, static_cast<short>(size / 10)}(eng));
    if(result) {
      return result;
    }
  }
  return {};
}

void print_args() {}

template <typename arg, typename... args>
void print_args(arg const & val, args const &... vals) {
  std::cerr << val << "\n";
  print_args(vals...);
}

template <typename... args> void test(bool (&func)(args...)) {
  std::default_random_engine gen{};
  auto result(test_all(gen, func));
  if(result) {
    std::cerr << "FAILURE\n";
    std::experimental::apply(print_args<args...>, *result);
  } else {
    std::cerr << "SUCCESS\n";
  }
}

#endif
