#define CASE_A // CASE_A, CASE_B or CASE_C

// Results:
// 
// gcc 15.0.0 20240604
// flags: -std=c++26 -Wall -Wextra -Wpedantic
//
// clang version 19.0.0git (7652a59407018c057cdc1163c9f64b5b6f0954eb)
// flags: -std=c++26 -Wall -Wextra -Wpedantic
//
// msvc v19.38
// flags: /std:c++latest /Wall
// 
//
// Case |      g++ | clang |    msvc  | expected |
//    A |  warning | error | compiles |    error |
//    B | compiles | error |    error |    error |
//    C | compiles | error | compiles |    error |

#ifdef CASE_A

// https://eel.is/c++draft/dcl.type.elab#2
//
// > If an elaborated-type-specifier is the sole constituent of a declaration,
// > the declaration is ill-formed unless it is an explicit specialization,
// > an explicit instantiation or it has one of the following forms:
// >
// >    class-key attribute-specifier-seqopt identifier ;
// >    class-key attribute-specifier-seqopt simple-template-id ; 
//
// Does "sole constituent" mean that A) it is the only decleration
// or B) that the elaborated-type-specifier is not part of other deleration?
//
// It is impossible to delcare nested class if it is not already declared inside the class,
// so only B) would make sense, which means that following should be ill-formed,
// as `X::a` is not an explicit specialization, an explicit instantiation
// nor neither of identifier nor a simple-template-id.

struct X {
    struct a;
};
struct X::a;

#elif defined(CASE_B)

// https://eel.is/c++draft/expr.prim.id#qual-2
//
// > A declarative nested-name-specifier shall not have a decltype-specifier.
// 
// `decltype(x)` is declarative nested-name-specifier as it is part of class-head-name. 
struct X {
    struct a;
};
auto x = X{};
struct decltype(x)::a{};

#elif defined(CASE_C)

// https://eel.is/c++draft/class.pre#3
//
// > If a class-head-name contains a nested-name-specifier,
// > the class-specifier shall not inhabit a class scope.
//
// `Bar::Mar` is a class-head-name and `bar::` is a nested-name-specifier.
struct Foo {
    struct Bar {
        struct Mar;
    };
    struct Bar::Mar {};
};

#endif

int main() {}

/*
https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGE6a4AMngMmAByPgBGmMQgAOzSAA6oCoRODB7evv6kyamOAsGhESzRsQm2mPYFDEIETMQEmT5%2BXJXV6XUNBEXhUTF6CvWNzdltQ929JWUSAJS2qF7EyOwc5gDMWDShANRuAIJC2AD6%2BzsA9Od7hyf7pNdHxxY7JA8nbiYa%2B5/7lzsASpgFF5aAQFCAfn9IVdgMhkDsuABWAB0GlROzMGjMkg0ADYNJJoTsqAZgOCdgBaIboEzrAAipks1jMuMpAHUxLR2ZhVARiEx2YlMOhDI5TF9LkTkAYjDsAG4xVICBEATlRqOAhB2EDiuMRZiYiJVOLiGi4AA5kBpEXFkOhkFwuLj1sgVVRcZJIojIriqBoVYjJJhIrMiSSmGSQJTqbSGdZmayKRzaFykzy%2BQKk0KRYI8OLfuciSwFHL4XKuGr1uaw6TyedqSh45YDAQgQQLsnaEToUS3EwlDsTHE3DtR2PgE3nkOR9LDMBB8Oxzti6XR9OdjyhQ5hQuPhKrmOzuudgB3BoMELz9cxYivddoFiJOhA3dLm934dEsdTxcPp/0BRX3fYhXzHYDQNHcDpy/UcR3vVBH2fQDr2IW8QPgxCAIgjdUI/Pdvi%2BDY8CoLY3lOH4iQQAgCEScFLkwKpkTwBRzkZKxLHQfkqAIc57VoZECAATyFZEqiYSINjMHt90HdZsB2ABJKgdkMDcDEiEgmFbdAKSEoUqS3Yi8BiHZmJ2AgEEwHYFE8Ky0AYIZCC8Rh21QZSBSwWd%2BRqUhuzk8zLJ2TyDG89JTMAuhaApfhiDYdAdi8BgAIi9szNUzdaDzLUFEMsQ8AALy09JfJk2l5PS1REky5AtRCCZcyK5VXi1BB%2BxeXY3ICqz%2BBTVAT0vYkSGLCFSrkvz5LHWcFAUCkAGtMEElTqOIPBIi8VsDMwGqaBiKlMAAR1QRJUqwXMdvQ9YrFGibRymmb5sWrS%2BVW9bME27bjOIPbDuO6y8EfehdMwAGtLevB4tpKd9yJOlUBfcwzBs%2BgdnsxyCGcwQEeXTBVIsrSdn2WZTNSwCLKsgRaEW4KYkahgiVeCwibx9sybU8TNO03ThLenKtqMkyzIYVB20SboXmU4XLJArB6FCgRaQAMWkgtFJJ0zHxSVJImRoggqqURiCs0Ihh3O7TOU2rAKF9sxENph0CpraQp3Orwas1m7pKlWbPaymdkZ09FloeKWCYebrMYJR7hPBA8wQbHDFJ1r2x62g%2BoGhQECD%2BLolMlNoqG4Uvb%2BNqTHxAANEAQCYMuNHCnZrZUhgN0q6rstyzLCp8puW6qrLUoc%2BoGpqIkhZA0JCCl8XTNOxxzob14BVSAG3tbEGNvB5EKK%2BIZiC8BwdnLhcrrOMdd/3m3IYouI6Svne%2BQvw%2Bq5ry7t%2B%2BMx1iqYi9e2YUIAOR4jM35/CojROi5wGL8WYqxScnEmDcQgZVYgyJEgrRYExGkH99peDEBSKS0NroEz1l5IqCoG5tmFBSZgbB3r8xApnTkC92ytTIR5J2ekeaGXOlvUqpV8TBQ4RAVQoZ8T12Cg0UhRsKE6WoZwvm882qWx2KLRo087oUksvbKhrBMDIkHPfPeB8j5DhPkuc%2BB8X4nyHLfV%2BXwmDrVQDsVQsk6SHxMdYu%2B%2BxzHtgEdzIRsxn7uJvp4iiH8v7KS2CEP%2BAD3ihkIjJUBtEQD0UYtAti1g4EILuigw2Gx1jKyhP5JSKkUYGGmhonGMidEo1kEwOqJTjac1kbQ86xcrhlS6qU/sM1eYfRMgwlMTDTIMFapELUAozYKDQCJAp7T8QWAaFXAAsg0Wu9cJllJmpoqpbAm4Q3xJERZEJRFpXISbHZci%2BnIJ%2BN4nYCtUCOPcV8MxD8D4LPQnEUxS4z6vPbCs4gnilweNsafUctz3nLIaMfYFVjgkgtCZ/Bg%2BAqBvxCO2UOIQIBEyCXSDg8xaCcERLwPw3BeCoE4B8JklhrKLGWFZDYPBSAEE0Hi%2BYs0QCSDMKiOINpERcHWOsXEKozDmirNIAlHBJDEpZaQclHBeDgg0Eyll8w4CwBgIgFACF/wxHIJQP8z5YhkmYLRLOBA%2BB0FbMQcEEBIgyrGcwYgglOCModQ0QSAB5SI2gtrMtJaQB8bBBAeqSs6jgWhSBYDWsAPsKZwT%2BqwKHIw4hw28HwIbBweAFTxojTyLar0ZVoqqDKzKkR%2BROo8FgGVz0WAut4AqYgGklB0mBnOTKRgVV8FrAANWMieD1QoSWMv4IIEQYh2BSBkIIRQKh1CptILoNoMpjCTn0KtcEkB5hHRqPGqkBAaT0nSZYFk7JGFpl5PyQUwpRR5jJQ2laWAN1YvaL69ILgkWjFaKQIIUTpgDDaHkNIAhP16EAzUKY/RYjjCqK%2BgQXQRieBaIMGDma4PDB6L%2ByDgx0MgfGOhiDpR/3zBsksFYcx9CEulfOuVTjzS4gpB6HYsJ4QQAfgwWaRMIC4EIHeD%2BXBZi8D9VoWYbKQAiuROaKQTpMRYiFbqdYFHJW8FrYiJVJKI1yoVSAJVQm8WkDVZqxYNF1p6ogAa2WYQdGcFUHRhjkgmNwm1GxjjvBhQ8YfXoEdwhRDiDMIiKd8glBqBlYu0gJ5%2BSJDrfiyjpB1Nks4B69aiR1rTxs/RxjzGnN73Y5xjwmETIMoE8q1NInSDbIGM%2BiVUrSAqbUzKzTthtPFeE6JzEyI%2BX6iROsREgrESBnFZwdYVGNOcEE52gzE2kDmd1RQMz2rDUoGXccZzFrQSKkoHa%2BdbqnV1tINtz13rfW7cDS5ENlMZVRq8DGzk8bGWJrbasCN6bX3ZplXm5ABb/VFolRG0t5bBKVse4JtBu2G1NswC2pNwB22gBK12iMChe2YH7YO3bXmx2%2Bf815mdwX526DMPoOcjYqU2FLU%2Brdx10i7pjIeycJ6kxnrZOmS9WZr25mQHem8btycvtQ34CArhcPfqRQRmYAGUhAYyIhsYuQJfgcw4RqDvOajwaaNLr9dhYO1HwwrsXtgcPq%2Bw5MXXRGFikYndFjgRLYv1c4LR9L9nZyylY9ljj2puNEHQnxorunSvldiM%2B9l6w1Q4kFWafUcRzRYjMHERT1Xas2%2Bo6NxrOnxsaogEgIzyXzWzem8QSzbBrO2cY07%2BcLvEoue/fgT34NPOyAx%2BwPzAWcdzojfjsLEWouKet3F2VCWkspc6mluzXTnfOdy/N2Wg4P5mB9yq%2BY/vKCW/jyAVTieRvypT811lpB2VcC5biGPnW4jCsxCf805pFNDfX/Fzfvv9Pp/T1NyfM39Uv9iKWRIiRjjlhVMcKsxwDQLAHoq2VqNqm2Ea%2B2u2%2B2XqPqDgx2CEQaBAZ2YaT2wYV2satAt2aarayaQOkaeAGajgr28672n2jK32Jaq0/2gO1aIO/qYOKQEOuB0Ol4na4YZISOKOjAaO9ePmjeWOsgLeIWIACmy6xO7EpO668AFOO6nA9Y%2B6Eh1gLYbYHYnInOqE3OMhyub6AuH6hubQP6xQWG4u%2BQ6QQuYG6Qou/6OhaG3QQumufOqu1hSuEwCGWQGuOuxhiu5GJGdK5GEqPetuHAKksUjGK4ZYFYyIVY7u1evG6w/GY2JWrW/mVWymq%2BdWSem%2Biq2%2BqqT%2BIAWeJmue7%2BBeqwqg4RJYkRaoKhQwrmcRHmbQ6O/BYmgh06QWreOgohHeTAkWpKluQRWRiWxmrk7kYR9mER8oURMR/87%2B0%2BAqc%2ByRZWlSFWy%2B6Ra%2BveDWORvurWkgyIkeSIjofKcQmIkmcQseEq1%2BGxye2ximZgw2t%2BSRLWpADaSofgkgQAA%3D%3D%3D
*/
