#include <numeric>

#include "matrixutils.hpp"

std::size_t num_of_elements(const matrix& m) {
    return std::reduce(m.begin(), m.end(), std::size_t{}, [](const std::size_t l, const vec& r) {
        return l + r.size();
    });
}
