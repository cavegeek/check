#include <iostream>
#include <tuple>

#include "check.hh"

bool f(unsigned u, int const & i) { return true; }

bool g(unsigned u) { return u + u >= u; }

int main() {
  test(f);
  test(g);
  return 0;
};
