#ifndef CHECK_HH
#define CHECK_HH

#include <type_traits>

#include "special.hh"

template<typename ...args>
bool check(bool (&func)(args...)) {
  return func(special<typename std::decay<args>::type>[0]...);
}

#endif
