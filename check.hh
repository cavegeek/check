#ifndef CHECK_HH
#define CHECK_HH

#include <cassert>
#include <functional>
#include <type_traits>

#include "special.hh"

inline bool check(std::function<bool()> func) {
  assert(bool(func));
  return func();
}

template <typename arg, typename... args>
inline bool check(std::function<bool(arg, args...)> func) {
  assert(bool(func));
  using dec_arg = typename std::decay<arg>::type;
  for(dec_arg const & val : special<dec_arg>) {
    if(!check(std::function<bool(args const &...)>{
           [&](args const &... a) { return func(val, a...); }})) {
      return false;
    }
  }
  return true;
}

template <typename... args> inline bool check(bool (&func)(args...)) {
  return check(std::function<bool(args...)>(func));
}

#endif
