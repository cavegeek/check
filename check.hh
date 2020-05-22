#ifndef CHECK_HH
#define CHECK_HH

#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <type_traits>

namespace check {

  namespace detail {
    struct BasicRandom {
      enum Range { random, all };

      BasicRandom() : engine{std::random_device{}()} {}

      // only valid on integral and floating point types
      template<typename T> operator T();

      template<typename T> T random_of(std::initializer_list<T> const & vals);
    private:
      std::minstd_rand engine;
      Range range;
    };
  }

  struct Random : detail::BasicRandom {
    Random() : rand(*this) {}
    void operator =(Random const &) = delete;
    Random & rand; // alternative name for *this for readability
    template<typename T> operator T();
  };

  struct Suite {
    Random rand;
    virtual void test() = 0;
    void property(std::string const & name, bool result, std::string const & log);
    void check();
  private:
    std::map<std::string, std::string> failures;
  };

  void Suite::check() {
    for(unsigned i = 0; i < 10; ++i) {
      test();
    }
    for(auto i : failures) {
      std::cerr << i.first << std::endl << i.second << std::endl;
    }
  }

  namespace detail {
    template<typename T> T BasicRandom::random_of(std::initializer_list<T> const & vals) {
      return *vals.begin()[
        std::uniform_int_distribution<size_t>{size_t{0}, vals.size()}(engine)];
    }

    template<typename T>
    BasicRandom::operator T() {
      static_assert(
        std::is_integral_v<T> || std::is_floating_point_v<T>,
        "BasicRandom only generates integral and floating point values");
      T const max_val = static_cast<T>(std::exp2((std::numeric_limits<T>::digits / T{2})));
      T const min_val = -std::min(max_val, -std::numeric_limits<T>::min());
      if constexpr (std::is_integral_v<T>) {
        return std::uniform_int_distribution{min_val, max_val}(engine);
      } else if constexpr (std::is_floating_point_v<T>) {
        return std::uniform_real_distribution{min_val, max_val}(engine);
      }
    }
  }

  template<typename T>
  Random::operator T() {
    return BasicRandom::operator T();
  }

  void Suite::property(std::string const & name, bool result, std::string const & output) {
    if(!result) {
      failures.insert({name, output});
    }
  }

  template<
      typename T,
      typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
  std::string format(T const & v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }

  inline std::string output() {return ""; }

  template<typename T, typename ...Ts> inline std::string output(T const & v, Ts const & ...vs) {
    return format(v) + "\n" + output(vs...);
  }
}

#endif
