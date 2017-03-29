#include <iostream>
#include <tuple>

#include "check.hh"

bool f(unsigned u, int const & i) { return true; }

bool g(unsigned u) { return u + u >= u; }

bool h(float f) { return f < 0.f || f + f >= f; }

bool j(double f) { return f < 0.f || f + f >= f; }

int main() {
  test(f);
  test(g);
  test(h);
  test(j);
  return 0;
};
