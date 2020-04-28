#ifndef CHECK_HH
#define CHECK_HH

#include <array>
#include <iostream>
#include <random>
#include <sstream>
#include <type_traits>

using engine_type = std::minstd_rand;

struct Engine {
  using result_type = engine_type::result_type;
  static constexpr result_type min() { return engine_type::min(); }
  static constexpr result_type max() { return engine_type::max(); }
  virtual result_type operator ()() = 0;
};

struct Random {
  Random() : rand(*this) {}
  Random & rand;
  template <typename T> operator T();
  template<typename T, size_t n> T random_of(std::array<T, n> const & vals) {
    return vals[std::uniform_int_distribution<size_t>{0, n - 1}(engine())];
  }

  virtual unsigned size() const = 0;

  virtual Engine & engine() = 0;
};

//! A test suite
struct Suite {
  std::string name;
  unsigned passed{0};
  unsigned failed{0};

  //! Runs func repeatedly, considering a return of false to be failure.
  void test(std::string name, bool (&func)(std::ostream &, Random &));
};

//! Outputs the suite's test results
std::ostream & operator<<(std::ostream & o, Suite const & suite) {
  return o << "Failed " << suite.failed << ", Passed " << suite.passed;
}

// implementation is below this line

struct EngineImpl : Engine {
  EngineImpl() : engine(std::random_device{}()) {}
  engine_type engine;
  result_type operator ()() { return engine(); }
};

// generation

template<typename T, typename = void> struct generate { static std::nullptr_t rand; };

struct RandomImpl : Random {
  RandomImpl(unsigned const arg_size, EngineImpl const arg_engine) :
      size_(arg_size), engine_(arg_engine) {}

  unsigned size_;
  EngineImpl engine_;

  unsigned size() const { return size_; }
  Engine & engine() { return engine_; }
};

template<typename T, T min_v, T max_v>
struct num_range {
  static_assert(
    std::is_integral_v<T> || std::is_floating_point_v<T>,
    "num_range must be used on an integral of floating point type");
  explicit constexpr num_range(T const & v) : val(v) {}
  T val;
  operator T() const { return val; }
  std::ostream operator<<(std::ostream & o) const { o << val; return o; }
};

template<typename T>
struct num_range_generator {
  static constexpr bool value = false;
};

template<typename T, T min_, T max_>
struct num_range_generator<num_range<T, min_, max_>> {
  static constexpr bool value = true;
  static constexpr T min = min_;
  static constexpr T max = max_;
  static bool constexpr in_range(T const & val) {
    return min <= val && max >= val;
  }
  using type = T;
};

template<typename T> Random::operator T() {
  if constexpr (std::is_integral_v<T>) {
    if constexpr (std::is_signed_v<T>) {
      if(size() == 0) {
        return random_of(std::array{
          T{0},
          std::numeric_limits<T>::min(),
          std::numeric_limits<T>::max()});
      } else {
        T max = (std::numeric_limits<T>::max() / 100) * static_cast<T>(size());
        return std::uniform_int_distribution{-max, max}(engine());
      }
    } else {
      if(size() == 0) {
        return random_of(std::array{
          T{0},
          std::numeric_limits<T>::max()});
      } else {
        T max = (std::numeric_limits<T>::max() / 100) * static_cast<T>(size());
        return std::uniform_int_distribution{T{0}, max}(engine());
      }
    }
  } else if constexpr (std::is_floating_point_v<T>) {
    if(size() > 0) {
      T s = static_cast<T>(size());
      return std::uniform_real_distribution{-(s * s), s * s}(engine());
    } else {
      return random_of(std::array{
          T{0},
          -T{0},
          std::numeric_limits<T>::min(),
          std::numeric_limits<T>::lowest(),
          std::numeric_limits<T>::max(),
          std::numeric_limits<T>::infinity(),
          -std::numeric_limits<T>::infinity(),
          std::numeric_limits<T>::quiet_NaN()});
    }
  } else if constexpr (num_range_generator<T>::value) {
    using num = typename num_range_generator<T>::type;
    if(size() > 0) {
      if constexpr (std::is_floating_point_v<num>) {
        num const s = static_cast<num>(size());
        num const s_sq = s*s;
        //TODO improve
        num const min_s{max(num_range_generator<T>::min, -s_sq)};
        num const max_s{min(num_range_generator<T>::max, s_sq)};
        return T{std::uniform_real_distribution{min_s, max_s}(engine())};
      } else {
        num const min{num_range_generator<T>::min};
        num const max{min + (num_range_generator<T>::max - min) / ((num)100 / static_cast<num>(size()))};
        return T{std::uniform_int_distribution{min, max}(engine())};
      }
    } else {
      if constexpr (std::is_floating_point_v<num>) {
        if constexpr (num_range_generator<T>::in_range(0)) {
          return random_of(std::array{
            T{0},
            -T{0},
            T{num_range_generator<T>::min},
            T{num_range_generator<T>::max}});
        } else {
          return random_of(std::array{
            T{num_range_generator<T>::min},
            T{num_range_generator<T>::max}});
        }
      } else {
        if constexpr (num_range_generator<T>::in_range(0)) {
          return random_of(std::array{
            T{0},
            T{num_range_generator<T>::min},
            T{num_range_generator<T>::max}});
        } else {
          return random_of(std::array{
            T{num_range_generator<T>::min},
            T{num_range_generator<T>::max}});
        }
      }
    }
  } else {
    static_assert(
      std::is_invocable_r_v<T, decltype(generate<T>::rand), Random &>,
      "Cannot generate a value of this type. "
      "Please specialize Random::operator T() or struct generate.");
    return generate<T>::rand(*this);
  }
}

template<> Random::operator bool() {
  return std::bernoulli_distribution{0.5}(engine());
}

// testing

void Suite::test(
    std::string name,
    bool (&func)(std::ostream &, Random &)) {
  std::ostringstream log;
  EngineImpl eng{};
  for(unsigned size = 0; size <= 100; ++size) {
    RandomImpl gen{size, eng};
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
    *+[](std::ostream & log, Random & rand) { body })
#define LOG(expr) log << #expr << " = " << (expr) << std::endl;
#define GEN(typ, var) auto var = (typ)(rand); LOG(var)
#define NAME(var, expr) auto var = expr; LOG(var)

#endif
