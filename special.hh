#ifndef INCLUDED_SPECIAL_HH
#define INCLUDED_SPECIAL_HH

#include <cassert>
#include <functional>
#include <vector>

template <typename V> extern std::vector<V> const special;

template <> extern std::vector<bool> const special<bool>;
template <> extern std::vector<unsigned> const special<unsigned>;
template <> extern std::vector<int> const special<int>;
template <> extern std::vector<float> const special<float>;
template <> extern std::vector<double> const special<double>;

template <typename V>
constexpr std::vector<V> specials_call(std::function<V()> func) {
  assert(bool(func));
  return {func()};
}

template <typename V, typename arg, typename... args>
constexpr std::vector<V> specials_call(std::function<V(arg, args...)> func) {
  assert(bool(func));
  std::vector<V> results{};
  using dec_arg = typename std::decay<arg>::type;
  for(dec_arg const & val : special<dec_arg>) {
    std::vector<V> new_results =
        specials_call(std::function<V(args const &...)>{
            [&](args const &... a) { return func(val, a...); }});
    results.insert(results.end(), new_results.begin(), new_results.end());
  }
  return results;
}

template <typename V, typename... args>
constexpr std::vector<V> special_construct() {
  return specials_call(std::function<V(args const &...)>{
      [](args const &... as) { return V{as...}; }});
}

#endif
