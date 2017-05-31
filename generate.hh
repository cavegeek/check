#ifndef INCLUDED_GENERATE_HH
#define INCLUDED_GENERATE_HH

#include <random>

//! Generates a random example of type V of a given size (0 to 100).
template <typename V> V generate(std::default_random_engine &, short size);

template <> bool generate<bool>(std::default_random_engine &, short size);
template <>
unsigned generate<unsigned>(std::default_random_engine &, short size);
template <> int generate<int>(std::default_random_engine &, short size);
template <> float generate<float>(std::default_random_engine &, short size);
template <> double generate<double>(std::default_random_engine &, short size);

#endif
