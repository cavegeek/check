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

template <typename... givens> struct given {
  static std::experimental::optional<std::tuple<givens...>>
      check_special(bool (&func)(givens...), givens const &... gvals) {
    return func(gvals...) ? std::experimental::optional<std::tuple<givens...>>{}
                          : std::tuple<givens...>{gvals...};
  }

  template <typename arg, typename... args>
  static std::experimental::optional<std::tuple<givens..., arg, args...>>
      check_special(
          bool (&func)(givens..., arg, args...), givens const &... gvals) {
    using dec_arg = typename std::decay<arg>::type;
    for(dec_arg const & val : special<dec_arg>) {
      auto result(given<givens..., arg>::check_special(func, gvals..., val));
      if(result) {
        return result;
      }
    }
    return {};
  }

  static std::experimental::optional<std::tuple<givens...>> check(
      std::default_random_engine &,
      bool (&func)(givens...),
      givens const &... gvals) {
    return func(gvals...) ? std::experimental::optional<std::tuple<givens...>>{}
                          : std::tuple<givens...>{gvals...};
  }

  template <typename arg, typename... args>
  static std::experimental::optional<std::tuple<givens..., arg, args...>> check(
      std::default_random_engine & gen,
      bool (&func)(givens..., arg, args...),
      givens const &... gvals) {
    using dec_arg = typename std::decay<arg>::type;
    for(short i = 0; i < 1000; ++i) {
      auto result(given<givens..., arg>::check(
          gen, func, gvals..., generate<dec_arg>(gen, i / 10)));
      if(result) {
        return result;
      }
    }
    return {};
  }
};

void print_args() {}

template <typename arg, typename... args>
void print_args(arg const & val, args const &... vals) {
  std::cerr << val << "\n";
  print_args(vals...);
}

template <typename... args> void test(bool (&func)(args...)) {
  auto result(given<>::check_special(func));
  if(result) {
    std::cerr << "FAILURE\n";
    std::experimental::apply(print_args<args...>, *result);
  } else {
    std::default_random_engine gen{};
    auto result_small(given<>::check(gen, func));
    if(result_small) {
      std::cerr << "FAILURE\n";
      std::experimental::apply(print_args<args...>, *result_small);
    } else {
      std::cerr << "SUCCESS\n";
    }
  }
}

#endif
