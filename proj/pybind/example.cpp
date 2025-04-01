#include "pybind11/pybind11.h"

#include <string_view>
#include <string>
#include <print>

int noicy_add(int i, int j) {
    std::println("{} + {} = {}", i, j, i + j);
    return i + j;
}

class NoicyDog {
    std::string name_;
public:
    NoicyDog(const std::string_view name): name_{name}{
        std::println("NoicyDog {}: ctor", name_);
    }

    void set_name(const std::string_view name) {
        std::println("NoicyDog {}: changing name to {}", name_, name);
        name_ = name;
    }

    std::string_view get_name() const {
        std::println("NoicyDog {}: getting name", name_);
        return name_;
    };
};

PYBIND11_MODULE(pybind_test, m) {
    m.doc() = "pybind11 test plugin"; // optional module docstring

    m.def("noicy_add", &noicy_add, "A function that adds two numbers and tells about it");

    pybind11::class_<NoicyDog>(m, "NoicyDog")
        .def(pybind11::init<std::string_view>())
        .def("set_name", &NoicyDog::set_name)
        .def("get_name", &NoicyDog::get_name);
}
