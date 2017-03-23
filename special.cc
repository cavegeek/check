#include "special.hh"

#include <limits>

template <>
std::vector<unsigned> const special<unsigned>{
    {0u, std::numeric_limits<unsigned>::max()}};

template <>
std::vector<int> const special<int>{
    {0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()}};
