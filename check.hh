#ifndef CHECK_HH
#define CHECK_HH

#include <cassert>
#include <experimental/optional>
#include <functional>
#include <tuple>
#include <type_traits>

#include "special.hh"

inline std::experimental::optional<std::tuple<>>
    check(std::function<bool()> func) {
  assert(bool(func));
  return func() ? std::experimental::optional<std::tuple<>>{} : std::tuple<>{};
}

template <typename arg, typename... args>
inline std::experimental::optional<std::tuple<arg, args...>>
    check(std::function<bool(arg, args...)> func) {
  assert(bool(func));
  using dec_arg = typename std::decay<arg>::type;
  for(dec_arg const & val : special<dec_arg>) {
    std::experimental::optional<std::tuple<args...>> result(
        check(std::function<bool(args const &...)>{
            [&](args const &... a) { return func(val, a...); }}));
    if(result) {
      return std::tuple_cat(std::tuple<arg>{val}, *result);
    }
  }
  return {};
}

template <typename... args>
inline std::experimental::optional<std::tuple<args...>>
    check(bool (&func)(args...)) {
  return check(std::function<bool(args...)>(func));
}

#endif
