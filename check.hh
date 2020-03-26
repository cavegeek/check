#ifndef CHECK_HH
#define CHECK_HH

#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <tuple>
#include <type_traits>

#include "generate.hh"

struct Gen {
  unsigned size;
  std::minstd_rand engine;
  template <typename T> operator T();
};

//! A test suite
struct Suite {
  std::string name;
  unsigned passed{0};
  unsigned failed{0};

  //! Runs func repeatedly, considering a return of false to be failure.
  void test(std::string name, bool (&func)(std::ostream &, Gen &));
};

//! Outputs the suite's test results
std::ostream & operator<<(std::ostream & o, Suite const & suite) {
  return o << "Failed " << suite.failed << ", Passed " << suite.passed;
}

// implementation is below this line

// generation

//TODO

template<typename R, typename T, size_t n> T random_of(R & eng, std::array<T, n> const & vals) {
  return vals[std::uniform_int_distribution<size_t>{0, n - 1}(eng)];
}

template<> Gen::operator int() {
  if(size == 0) {
    return random_of(engine, std::array<int, 3>{
      0,
      std::numeric_limits<int>::min(),
      std::numeric_limits<int>::max()});
  } else {
    int max = (std::numeric_limits<int>::max() / 100) * static_cast<int>(size);
    return std::uniform_int_distribution<int>{-max, max}(engine);
  }
}

// testing

void Suite::test(
    std::string name,
    bool (&func)(std::ostream &, Gen &)) {
  std::ostringstream log;
  std::minstd_rand eng{std::random_device{}()};
  log << name << std::endl;
  for(unsigned size = 0; size <= 100; ++size) {
    Gen gen{size, eng};
    for(unsigned i = 0; i < 10; ++i) {
      bool result = func(log, gen);
      if(!result) {
        std::cerr << "FAILED: " << log.str() << std::endl;
        ++failed;
        return;
      }
    }
  }
  ++passed;
}

// helper macros

#define TEST(suite, name, body) \
  (suite).test( \
    (name), \
    *+[](std::ostream & log, Gen & gen) { body })
#define LOG(expr) log << #expr << " = " << (expr) << std::endl;
#define GEN(typ, var) typ var = gen; LOG(var)
#define NAME(var, expr) auto var = expr; LOG(var)

#endif
