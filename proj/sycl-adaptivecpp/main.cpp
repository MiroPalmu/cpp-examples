#include "syclutils.hpp"
import sstd;

int
main() {
    syclutils::ls_platforms();
    syclutils::print_kernel_indecies();
    sstd::generic_print(42);
    sstd::generic_print("foo");
}
