#pragma once

#include <vector>
#include <cstddef>

using vec = std::vector<double>;
using matrix = std::vector<vec>;

std::size_t num_of_elements(const matrix&);
