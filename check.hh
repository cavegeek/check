#ifndef CHECK_HH
#define CHECK_HH

#include <experimental/optional>
#include <experimental/tuple>
#include <iostream>
#include <tuple>
#include <type_traits>

#include "special.hh"

template <typename... givens> struct given {
  static inline std::experimental::optional<std::tuple<givens...>>
      check(bool (&func)(givens...), givens const &... gvals) {
    return func(gvals...) ? std::experimental::optional<std::tuple<givens...>>{}
                          : std::tuple<givens...>{gvals...};
  }

  template <typename arg, typename... args>
  static inline std::experimental::optional<std::tuple<givens..., arg, args...>>
      check(bool (&func)(givens..., arg, args...), givens const &... gvals) {
    using dec_arg = typename std::decay<arg>::type;
    for(dec_arg const & val : special<dec_arg>) {
      auto result(given<givens..., arg>::check(func, gvals..., val));
      if(result) {
        return result;
      }
    }
    return {};
  }
};

inline void print_args() {}

template <typename arg, typename... args>
inline void print_args(arg const & val, args const &... vals) {
  std::cerr << val << "\n";
  print_args(vals...);
}

template <typename... args> inline void test(bool (&func)(args...)) {
  auto result(given<>::check(func));
  if(result) {
    std::cerr << "FAILURE\n";
    std::experimental::apply(print_args<args...>, *result);
  } else {
    std::cerr << "SUCCESS\n";
  }
}

#endif
