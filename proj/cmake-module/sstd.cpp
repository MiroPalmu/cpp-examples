export module sstd;
import std;

export void print_backwards(const std::string_view sv) {
    std::println("{} || {}", sv | std::views::reverse | std::ranges::to<std::string>(), sv);
}
