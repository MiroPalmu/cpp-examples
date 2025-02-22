#include "syclutils.hpp"

#include <array>
#include <print>

#include "sycl/sycl.hpp"

void
syclutils::ls_platforms() {
    // Loop through available platforms
    for (auto const& platform : sycl::platform::get_platforms()) {
        std::println("Found platform: {}", platform.get_info<sycl::info::platform::name>());
        // Loop through available devices in this platform
        for (auto const& device : platform.get_devices()) {
            std::println("With device: ", device.get_info<sycl::info::device::name>());
        }
        std::println("");
    }
}

void
syclutils::print_kernel_indecies() {
    constexpr int         size = 16;
    std::array<int, size> data;
    auto                  buff = sycl::buffer{ data };
    auto                  q    = sycl::queue{};

    q.submit([&](sycl::handler& h) {
        auto acc = sycl::accessor{ buff, h };
        h.parallel_for(size, [=](auto& idx) { acc[idx] = idx; });
    });

    for (const auto x : sycl::host_accessor{ buff }) { std::print("{} ", x); }
    std::println("");
}
