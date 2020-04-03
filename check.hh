#ifndef CHECK_HH
#define CHECK_HH

#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <tuple>
#include <type_traits>

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

template<typename T> Gen::operator T() {
  if constexpr (std::is_integral_v<T>) {
    if constexpr (std::is_signed_v<T>) {
      if(size == 0) {
        return random_of(engine, std::array{
          T{0},
          std::numeric_limits<T>::min(),
          std::numeric_limits<T>::max()});
      } else {
        T max = (std::numeric_limits<T>::max() / 100) * static_cast<T>(size);
        return std::uniform_int_distribution<T>{-max, max}(engine);
      }
    } else {
      if(size == 0) {
        return random_of(engine, std::array{
          T{0},
          std::numeric_limits<T>::max()});
      } else {
        T max = (std::numeric_limits<T>::max() / 100) * static_cast<T>(size);
        return std::uniform_int_distribution<T>{-max, max}(engine);
      }
    }
  } else if constexpr (std::is_floating_point_v<T>) {
    if(size > 0) {
      T s = static_cast<T>(size);
      return std::uniform_real_distribution<T>{-(s * s), s * s}(engine);
    } else {
      return random_of(engine, std::array{
          T{0},
          -T{0},
          std::numeric_limits<T>::min(),
          std::numeric_limits<T>::lowest(),
          std::numeric_limits<T>::max(),
          std::numeric_limits<T>::infinity(),
          -std::numeric_limits<T>::infinity(),
          std::numeric_limits<T>::quiet_NaN()});
    }
  }
}

template<> Gen::operator bool() {
  return std::bernoulli_distribution{0.5}(engine);
}

// testing

void Suite::test(
    std::string name,
    bool (&func)(std::ostream &, Gen &)) {
  std::ostringstream log;
  std::minstd_rand eng{std::random_device{}()};
  for(unsigned size = 0; size <= 100; ++size) {
    Gen gen{size, eng};
    for(unsigned i = 0; i < 10; ++i) {
      log.str("");
      log << name << std::endl;
      bool result = func(log, gen);
      log.flush();
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
