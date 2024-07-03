#include <print>
#include <tuple>
#include <utility>

#include "noicy_class.hpp"

using nsv = noicy_string_view;

auto foo() { return std::tuple{ nsv{ "a" }, std::tuple{ nsv{ "b" }, nsv{ "c" } } }; }

int main() {
    // Does not compile :(
    // constexpr auto [a, b, c] = foo();

    auto&& [a, bc]    = foo();
    const auto [b, c] = std::move(bc);

    std::println("a = '{}'", a.sv());
    std::println("b = '{}'", b.sv());
    std::println("c = '{}'", c.sv());
}
