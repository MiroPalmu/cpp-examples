
// Linux
#include <fcntl.h>
#include <unistd.h>

// Gnulib
// #include "full-read.h"

// C++
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <ranges>
#include <span>
#include <thread>
#include <vector>

int main(int argc, char** argv) {
    if (not(argc == 2 or argc == 3)) {
        std::cout << "Usage: readfifo <relative/path/to/fifo> [how_many_reads]" << std::endl;
        return 1;
    }

    std::cout << "Opening fifo with path: " << argv[1] << std::endl;
    auto fifo_end = open(argv[1], O_RDONLY);
    if (fifo_end == -1) {
        std::cout << "open(\"" << argv[1] << "\", O_RDONLY); failed!" << std::endl;
        return 1;
    }

    auto whole_recv_buf = std::vector<std::byte>{};

    static constexpr auto bytes_to_read = 2;
    auto recv_buf                       = std::array<std::byte, bytes_to_read>{};

    for (;;) {
        std::cout << "Trying to read " << bytes_to_read << " bytes..." << std::endl;

        const auto read_bytes = read(fifo_end, recv_buf.data(), bytes_to_read);

        if (read_bytes == -1) {
            std::cout << "read(...) returned -1 and set errno to " << errno << std::endl;
            return 1;
        }

        if (read_bytes == 0) {
            std::cout << "At EOF" << std::endl;
            // At EOF
            break;
        }

        std::cout << "Got " << read_bytes << " bytes!" << std::endl;

        const auto read = std::span<std::byte>(recv_buf.data(), read_bytes);
        std::ranges::copy(read, std::back_inserter(whole_recv_buf));

        if (argc == 3) {
            static auto how_many_reads = 0;
            const auto limit           = std::atoi(argv[2]);
            if (++how_many_reads >= limit) {
                std::cout << "Stopped reading after " << how_many_reads << " read(s)!" << std::endl;
                break;
            }
        }
        using namespace std::literals;
        std::this_thread::sleep_for(500ms);
    }

    std::cout << "Closing fifo..." << std::endl;
    if (close(fifo_end) == -1) {
        std::cout << "close(fifo) returned -1!" << std::endl;
        return 1;
    }

    std::cout << "As whole got:" << std::endl;
    for (const auto x : whole_recv_buf) std::cout << static_cast<char>(x);
    std::cout << std::endl;
}
