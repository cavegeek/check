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
std::vector<Point> special<Point>{special_construct<Point, int, int>()};

template <> Point generate_small<Point>(std::default_random_engine & gen) {
  return Point{generate_small<int>(gen), generate_small<int>(gen)};
}

template <> Point generate_large<Point>(std::default_random_engine & gen) {
  return Point{generate_large<int>(gen), generate_large<int>(gen)};
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
  test(* + [](bool b) { return b || !b; });
  test(f);
  test(g);
  test(h);
  test(j);
  test(k);
  test(m);
  return 0;
};
