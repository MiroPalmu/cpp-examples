#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <print>
#include <type_traits>

using namespace std;

template <typename T> struct vec2 {
  T x{1}, y{0};
};

template <typename T, size_t N> struct vec2_array {
  array<vec2<T>, N> vecs;
  using vec_type = vec2<T>;
};

#ifndef ARGUMENT_TYPE
#define ARGUMENT_TYPE void
#endif

using arg_type = ARGUMENT_TYPE;
using arr_type = remove_cvref_t<arg_type>;
using vec_type = arr_type::vec_type;

__attribute__((noinline)) auto do_something(arg_type arr) {
  auto new_arr = auto{arr};
  ranges::transform(arr.vecs, new_arr.vecs.begin(), [](const vec_type &p) {
    return vec_type{cos(p.x) - sin(p.y), tan(p.x + p.y)};
  });
  return new_arr;
}

int main(int argc, char **argv) {

  const auto n = stoull(argv[1]);

  auto P = arr_type{};
  for (size_t i{0}; i < n; ++i) {
    P = do_something(P);
  }

  // We print out the final result b/c otherwise the compiler is too smart and
  // notices that we were looping for no result and makes the entire thing a
  // no-op, ruining our benchmark.

  for (const auto &v : P.vecs) {
    std::println("x, y = {}, {}", v.x, v.y);
  }

  return 0;
}
