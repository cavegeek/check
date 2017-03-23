#include <iostream>

#include "check.hh"

bool f(unsigned u, int const & i) {
  std::cout << u << ", " << i << std::endl;
  return true;
}

int main() {
  check(f);
  return 0;
};
