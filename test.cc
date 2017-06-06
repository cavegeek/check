#include <iostream>

#include "check.hh"

bool f(unsigned u, int const & i) { return true; }

bool g(unsigned u) { return u + u >= u; }

bool h(float f) { return f < 0.f || f + f >= f; }

bool j(double f) { return f < 0.f || f + f >= f; }

bool k(bool x, bool y) { return x != y; }

struct Point {
  int x;
  int y;
};

template <>
Point generate<Point>(std::default_random_engine & gen, short size) {
  return Point{generate<int>(gen, size), generate<int>(gen, size)};
}

Point rotate(Point p) { return {-p.y, p.x}; }

bool operator==(Point const & p0, Point const & p1) {
  return p0.x == p1.x && p0.y == p1.y;
}

std::ostream & operator<<(std::ostream & o, Point const & p) {
  return o << "{" << p.x << "," << p.y << "}";
}

bool m(Point p) { return rotate(rotate(rotate(rotate(p)))) == p; }

int main() {
  test(*+[](bool b) { return b || !b; });
  test(f);
  test(g);
  test(h);
  test(j);
  test(k);
  test(m);
  test(*+[](unsigned x, unsigned y) { return x + y < 4 || x + y > 100; });
  test(*+[](unsigned x, unsigned y) { return x + y < 200 || x + y > 10000; });
  return 0;
};
