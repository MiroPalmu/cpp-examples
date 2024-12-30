#include <print>
#include <cmath>

#define magnitude rpl_magnitude

struct complex{
    double re, im;

    // Macro replaces this.
    double magnitude() const {
        return std::sqrt(re * re + im * im);
    }
};

// Symbol is complex::rpl_magnitude,
// so following undef would lead to compilation error.

// #undef magnitude

int main() {
    const auto z = complex{ 1, 4 };
    std::println("|z| = {}", z.magnitude());
}
