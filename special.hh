#ifndef INCLUDED_SPECIAL_HH
#define INCLUDED_SPECIAL_HH

#include <vector>

template <typename V> extern std::vector<V> const special;

template <> extern std::vector<unsigned> const special<unsigned>;
template <> extern std::vector<int> const special<int>;

#endif
