#ifndef INCLUDED_GENERATE_HH
#define INCLUDED_GENERATE_HH

#include <random>

//! Generates a random small example of type V.
template <typename V> V generate_small(std::default_random_engine &);

template <> bool generate_small<bool>(std::default_random_engine &);
template <> unsigned generate_small<unsigned>(std::default_random_engine &);
template <> int generate_small<int>(std::default_random_engine &);
template <> float generate_small<float>(std::default_random_engine &);
template <> double generate_small<double>(std::default_random_engine &);

//! Generates a random potentially large example of type V.
template <typename V> V generate_large(std::default_random_engine &);

template <> bool generate_large<bool>(std::default_random_engine &);
template <> unsigned generate_large<unsigned>(std::default_random_engine &);
template <> int generate_large<int>(std::default_random_engine &);
template <> float generate_large<float>(std::default_random_engine &);
template <> double generate_large<double>(std::default_random_engine &);

#endif
