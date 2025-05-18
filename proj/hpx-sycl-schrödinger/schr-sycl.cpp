/* Schrödinger equation solver using sycl. */

#include <cstddef>
#include <print>

#include <sycl/sycl.hpp>

int
main() {
    sycl::queue q{ sycl::default_selector{} };

    constexpr std::size_t N = 8;
    std::vector<int> data1(N, 1);
    std::vector<int> data2(N, 2);

    std::println("{}", data1);
    std::println("{}", data2);
    std::println();

    {
        sycl::buffer<int> buf1(data1);
        sycl::buffer<int> buf2(data2);

        // First kernel: multiply each element by 2
        sycl::event e1 = q.submit([&](sycl::handler& h) {
            auto acc = buf1.get_access<sycl::access_mode::read_write>(h);
            h.parallel_for(sycl::range<1>(data1.size()), [=](sycl::id<1> idx) { acc[idx] *= 2; });
        });

        // Second kernel: add 5 to each element, depends on e1
        sycl::event e2 = q.submit([&](sycl::handler& h) {
            auto acc = buf2.get_access<sycl::access::mode::read_write>(h);
            h.parallel_for(sycl::range<1>(data2.size()), [=](sycl::id<1> idx) { acc[idx] += 5; });
        });

        // Third kernel: square each element, depends on e2
        q.submit(&{
            h.depends_on(e1);
            h.depends_on(e2);
            auto acc1 = buf1.get_access<sycl::access_mode::read_write>(h);
            auto acc2 = buf2.get_access<sycl::access_mode::read>(h);
            h.parallel_for(sycl::range<1>(N), [=](sycl::id<1> idx) { acc1[idx] *= acc2[idx]; });
        });
    } // buffer goes out of scope, data is copied back

    std::println("{}", data1);
    std::println("{}", data2);

    return 0;
}
