#include <config.h>

#include "fifo_utils.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fifo {

auto open_fifo(const std::string& path) -> int {
    const auto fifo_end = open(path.c_str(), O_RDONLY);
    if (fifo_end == -1) {
        throw std::runtime_error(std::format("open({}, O_RDONLY); failed!", path));
    }
    return fifo_end;
}

auto close_fifo(const int fifo) -> void {
    if (close(fifo) == -1) {
        throw std::runtime_error(std::format("close(fifo); failed with errno = {}", errno));
    }
}

} // namespace fifo
