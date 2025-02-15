#include "matrixutils.hpp"
import std;
import sstd;

int main() {
    const auto msg = std::string_view{ "Hello world!" };
    print_backwards(msg);

    const auto m = matrix{ { 1, 2, 3 }, { 4, 5, 6 } };
    std::println("This matrix {} should have {} elements.", m, num_of_elements(m));
}

