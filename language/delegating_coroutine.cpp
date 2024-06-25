#include <generator>
#include <print>
#include <ranges>

std::generator<const int&> fib_alg(int a, int b) {
    while (true) {
        const auto c = a + b;
        co_yield c;
        a = b;
        b = c;
    }
}

/// Generate fib numbers starting from (a, b) = (n, 2n).
///
/// Note that fib2 is not a coroutine but it returns a coroutine handle.
std::generator<const int&> fib2(const int n) { return fib_alg(n, 2 * n); }

int main() {
    std::println("fib_alg(0, 1):");
    for (const auto x : fib_alg(0, 1) | std::views::take(5)) { std::print("{} ", x); }
    std::println();
    std::println();

    std::println("fib2(5):");
    for (const auto x : fib2(5) | std::views::take(5)) { std::print("{} ", x); }
    std::println();
}
