#include <hpx/hpx_start.hpp>
#include <hpx/iostream.hpp>

int hpx_main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Any HPX application logic goes here...
    hpx::cout << "hello" << std::endl;
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    // Initialize HPX, run hpx_main.
    hpx::start(argc, argv);

    // ...Execute other code here...

    // Wait for hpx::finalize being called.
    return hpx::stop();
}


