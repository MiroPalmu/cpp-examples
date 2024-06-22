#include <algorithm>
#include <functional>
#include <generator>
#include <print>
#include <ranges>
#include <type_traits>
#include <utility>

using namespace std;

template<ranges::range R>
using gen_t = generator<ranges::range_const_reference_t<R>>;

template<typename P, typename C>
struct filter_invoke_closure : ranges::range_adaptor_closure<filter_invoke_closure<P, C>> {
    remove_cvref_t<P> predicate;
    remove_cvref_t<C> callback;

    template<ranges::input_range R>
    auto operator()(this auto&& self, R&& input) {
        return views::filter(views::all(std::forward<R>(input)), [&](const auto& x) {
            if (invoke(self.predicate, x)) {
                invoke(self.callback, x);
                return false;
            }
            return true;
        });
    }
};

struct filter_invoke_adaptor {
    auto operator()(this auto&&,
                    ranges::viewable_range auto&& input,
                    auto predicate,
                    auto callback) {
        return filter_invoke_closure<decltype(predicate), decltype(callback)>{
            .predicate = predicate,
            .callback  = callback
        }(move(input));
    }

    auto operator()(this auto&&, auto predicate, auto callback) {
        return filter_invoke_closure<decltype(predicate), decltype(callback)>{ .predicate =
                                                                                   predicate,
                                                                               .callback =
                                                                                   callback };
    }
};

constexpr auto filter_invoke = filter_invoke_adaptor{};

int main() {
    auto even         = [](const int x) { return x % 2 == 0; };
    auto treven       = [](const int x) { return x % 3 == 0; };
    auto print_even   = [](const int x) { std::println("odd:    {}", x); };
    auto print_treven = [](const int x) { std::println("treven: {}", x); };
    auto print_unused = [](const int x) { std::println("unused: {}", x); };

    auto nums = views::iota(0, 10);
    auto b    = nums | filter_invoke(treven, print_treven) | filter_invoke(even, print_even);

    ranges::for_each(b, print_unused);
}
