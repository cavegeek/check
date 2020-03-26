#include <iostream>

#include "check.hh"

int main() {
  Suite suite{"int properties"};
  suite.test("commutative", *+[](std::ostream & log, Gen & gen) {
    int x = gen;
    return (2 * x) % 2 == 0;
  });
  suite.test("neg", *+[](std::ostream & log, Gen & gen) {
    int x = gen;
    return x < 0 || -x > 0;
  });
  TEST(suite, "neg w macro",
    GEN(int, x);
    LOG(-x);
    return x <= 0 || -x <= 0;
  );
  TEST(suite, "sub",
    GEN(int, x);
    GEN(int, y);
    NAME(diff, x - y);
    return diff < x;
  );
  std::cerr << suite << "\n";
  return 0;
}
