#include <iostream>

#include "check.hh"

struct Point {
  float x;
  float y;
};

Point operator+(Point const & v, Point const & w) {
  return Point{v.x + w.x, v.y + w.y};
}

bool operator==(Point const & v, Point const & w) {
  return v.x == w.x && v.y == w.y;
}

template <> Gen::operator Point() {
  return {*this, *this};
}

std::ostream & operator<<(std::ostream & o, Point const & p) {
  o << "{" << p.x << ", " << p.y << "}";
  return o;
}

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
  TEST(suite, "sub long",
    GEN(long, x);
    GEN(long, y);
    NAME(diff, x - y);
    return diff < x;
  );
  std::cerr << suite << "\n";

  Suite fl{"float properties"};
  TEST(fl, "commutative",
    GEN(float, x);
    GEN(float, y);
    NAME(sum1, x + y);
    NAME(sum2, y + x);
    return sum1 == sum2;
  );
  std::cerr << fl << "\n";

  Suite user{"user defined type properties"};
  TEST(user, "commutative Point",
    GEN(Point, p);
    GEN(Point, q);
    NAME(sum1, p + q);
    NAME(sum2, q + p);
    return sum1 == sum2;
  );
  std::cerr << user << "\n";

  return 0;
}
