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
      BasicRandom() : engine{std::random_device{}()} {}

      // only valid on integral and floating point types
      template<typename T> operator T();

      std::size_t small_size(std::size_t max = 15);
    private:
      std::minstd_rand engine;
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
    template<typename T>
    BasicRandom::operator T() {
      static_assert(
        std::is_integral_v<T> || std::is_floating_point_v<T>,
        "BasicRandom only generates integral and floating point values");
      if constexpr (std::is_same_v<T, bool>) {
        return std::bernoulli_distribution{0.5}(engine);
      } else {
        T const max_val = static_cast<T>(std::exp2((std::numeric_limits<T>::digits / T{2})));
        T const min_val = -std::min(max_val, -std::numeric_limits<T>::min());
        if constexpr (std::is_integral_v<T>) {
          return std::uniform_int_distribution{min_val, max_val}(engine);
        } else if constexpr (std::is_floating_point_v<T>) {
          return std::uniform_real_distribution{min_val, max_val}(engine);
        }
      }
    }

    std::size_t BasicRandom::small_size(std::size_t const max) {
      return std::uniform_int_distribution<std::size_t>{0, max}(engine);
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
