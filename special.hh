#ifndef INCLUDED_SPECIAL_HH
#define INCLUDED_SPECIAL_HH

#include <vector>

template <typename V> extern std::vector<V> const special;

template <> extern std::vector<unsigned> const special<unsigned>;
template <> extern std::vector<int> const special<int>;
template <> extern std::vector<float> const special<float>;
template <> extern std::vector<double> const special<double>;

#endif
