#include <iostream>

#include "check.hh"

using namespace check;

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

template <> Random::operator Point() {
  return {rand, rand};
}

std::string format(Point const & p) {
  return "{" + format(p.x) + ", " + format(p.y) + "}";
}

struct properties : Suite {
  void test() override {
    int x = rand;
    int y = rand;
    property("int commutative", x + y == y + x, output(x, y, x + y, y + x));
    int z = rand;
    property("int associative", x + (y + z) == (x + y) + z, output(x, y, z, y + z, x + (y + z), (x + y), (x + y) + z));
    float v = rand;
    float w = rand;
    property("float commutative", v + w == w + v, output(v, w, v + w, w + v));
    property("float sub", v - w == w - v, output(v, w, v - w, w - v));
    Point p = rand;
    Point q = rand;
    property("Point commutative", p + q == q + p, output(p, q, p + q, q + p));
  }
};

int main() {
  properties{}.check();
  return 0;
}
