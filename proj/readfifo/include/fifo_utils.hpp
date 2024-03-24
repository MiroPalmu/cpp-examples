#pragma once

/// @file Header not needed but used to get more realistic project for GNU build
/// system.

#include <string>

namespace fifo {

auto open_fifo(const std::string& path) -> int;
auto close_fifo(const int fifo) -> void;

} // namespace fifo
