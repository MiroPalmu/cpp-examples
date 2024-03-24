

template<typename...>
struct TypeList;

template<typename Head, typename... Tails>
struct TypeList<Head, Tails...> {
    using head  = Head;
    using tails = TypeList<Tails...>;
};

// Recursive template pattern needs following elements:
//
// * Templated forward decleration of struct that just amount of template parameters
//
// Idea is that base case and recursive case update some constant time property (A)
// (eg. constexpr static member variable)
// of the class, based on their templated parameters which are used to
// explicitly define some specializations of this struct.
//
// * (optional) Templated helper definition of A
//
// * Recusvice case
//
// Template parameters are used to define (potentially updated) property A
// with value of anohter specialization.
//
// * Templated base case of the recursive case

//////////////////////////////////////////////////////////////////////////

// CountOccuranceOf decleration
template<typename T, typename TList>
struct CountOccuranceOf;

// CountOccuranceOf helper constant
template<typename T, typename TList>
inline constexpr std::size_t CountOccuranceOf_v = CountOccuranceOf<T, TList>::value;

// CountOccuranceOf base case
template<typename T>
struct CountOccuranceOf<T, TypeList<>> {
    constexpr static std::size_t value = 0;
};

// CountOccuranceOf recursive case
template<typename T, typename Head, typename... Tail>
struct CountOccuranceOf<T, TypeList<Head, Tail...>> {
    constexpr static auto increment    = static_cast<std::size_t>(std::is_same_v<T, Head>);
    constexpr static std::size_t value = increment + CountOccuranceOf_v<T, TypeList<Tail...>>;
};

//////////////////////////////////////////////////////////////////////////
