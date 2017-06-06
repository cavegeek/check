#include "special.hh"

#include <limits>

template <> std::vector<bool> const special<bool>{{false, true}};

template <>
std::vector<int> const special<int>{
    {0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()}};

template <>
std::vector<float> const special<float>{
    {0.f,
     -0.f,
     std::numeric_limits<float>::min(),
     std::numeric_limits<float>::lowest(),
     std::numeric_limits<float>::max(),
     std::numeric_limits<float>::infinity(),
     -std::numeric_limits<float>::infinity(),
     std::numeric_limits<float>::quiet_NaN()}};

template <>
std::vector<double> const special<double>{
    {0.f,
     -0.f,
     std::numeric_limits<double>::min(),
     std::numeric_limits<double>::lowest(),
     std::numeric_limits<double>::max(),
     std::numeric_limits<double>::infinity(),
     -std::numeric_limits<double>::infinity(),
     std::numeric_limits<double>::quiet_NaN()}};
