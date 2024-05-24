#include <type_traits>

struct X;

/*
As X is incomplete type, operator sizeof "shall not" be applied to it [expr.sizeof].
Now assuming this means that sizeof(X) is invalid expression,
it should be detectable using requires expression [expr.prim.reg]:

  > The substitution of template arguments into a requires-expression
  > can result in the formation of invalid types or expressions
  > in the immediate context of its requirements ([temp.deduct.general]) ...
  > ... In such cases, the requires-expression evaluates to false;
  > it does not cause the program to be ill-formed.

However simply this does not compile:

```
static_assert(not requires { sizeof(Y); })
```

because [expr.prim.req.general]:

  > If the substitution of template arguments into a requirement
  > would always result in a substitution failure,
  > the program is ill-formed; no diagnostic required

This can be prevented by wrapping the requires expression to a concept or generic lambda.
*/

template <typename T>
concept complete = requires { sizeof(T); };

static_assert(not complete<X>);

static_assert(not
    []<typename T = X>(){
        return requires { sizeof(T); };
    }()
);

/*
Now we can try this trick with with std::make_signed[_t],
for which subtituting template argument X makes program ill-formed.

In [meta.rqmts] it is stated that:

  > Unless otherwise specified, an incomplete type may be used to
  > instantiate a template specified in [type.traits].
  > The behavior of a program is undefined if:
  >
  > - an instantiation of ] template specified in [type.traits]
  >   directly or indirectly depends on an incompletely-defined object type T, and
  >
  > - that instantiation could yield a different result
  >   were T hypothetically completed.

so one might think that std::make_signed<X> would be undefined behaviour.
However, [meta.trans.sign] states for make_signed<T>:

  > Mandates: T is an integral or enumeration type other than cv bool.

and in [structure.specifications] mandates are defined as:

  > the conditions that, if not met, render the program ill-formed.

which implies that std::make_signed<X> makes program ill-formed.

This makes following not compile:

```
static_assert(not requires { typename std::make_signed_t<X>; });
```

Now depending on if std::make_signed<X> can be interpeted as invalid type,
it could be detected using requires expression. But it turns out that
implementations have differing behaviour:

Below the code in #if ... #endif will compile with:

- Clang trunk (*) with -std=c++{20,26} -libstd=libc++

but not with:
 
- Clang trunk (*) with -std=c++{20,26}
- MSVC v19.38 with /std:c++{20,latest}
- GCC {14.1.1 20240507, trunk 15.0.0 20240524} with -std=c++{20,26}

(*) 9223ccb0e56d6d4de17808e2e4000c8019a9a218

This indicates that the libc++ implementation differs from the others.
*/

#if false // true

template <typename T>
concept signable = requires { typename std::make_signed_t<T>; };

static_assert(not signable<X>);

static_assert(not
    []<typename T = X>(){
        return requires { typename std::make_signed_t<T>; };
    }()
);

#endif

int main() { return 42; }

/*
Full-link on godbolt:

https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIMwBspK4AMngMmAByPgBGmMQSGgCcpAAOqAqETgwe3r7%2BQemZjgJhEdEscQlcybaY9iUMQgRMxAS5Pn5cdQ3Zza0EZVGx8XoKLW0d%2Bd3jA0MVVRIAlLaoXsTI7BzmAMzhyN5YANQmO24EAJ6pmAD6BMRMhAqn2CYaAIJv7%2BPEXg5HAA1TlYPl8APQAKi%2B30BRzwCjhDDQLFS9AImCOl2upCOqGuDyIxCOmQAXphUFQTmYzAoEGJaEcGKgCOYzEc4kcmKlUXhMOhMag4QQTgBWCyYVSpYgAOlJ5KoJhFABFpV9IqgAO6chQKHzhYCYhDwo5sQwIgh04VyikQAFLOEI8IANzEeH5EqlmB12VIX0IxIQa1o/I5WHRDiYMXoRy8mSMR2ImAAjl48ImER705kBKLxZKZVK8CxpYngIqlSAvl8jicdtgjgAVBAY3UxcaELyNXGU9EogzoznEYA%2BRgER2CQVMBPJ1PpgC0ma92YY1dr9dEDGnutowvChox/GILCYXYpiJdtDdmKuXtxRMX3oEzw%2BNZeiP3cJYbHwJ4xaEEErCmeTzTimaaYGwggIhAioWL2qTSlg6B/AQ0rAIw8RiOW9rSrhq5vrh0pHAAkpuurIAgRyiEoCg4haGKJmB84PsuRyYBeXi/uagpUGISjAvhdZCkc6CoLeTLCqIsYYvRRxSqgwAPCwArshidC0HOh7fqqoIfAAEpq7HxMSRaohchrGqJ4nMlRqAonQ7BVqCAQaCYLlua5HyzI4yA3EwOrxAQEASaBs63iYADsFgmWSNoAJpLMCJwRUqiXOa57nubp7xxFJSi5pm0qFsWjFoRhDy0OWlbZa%2BQnET2zbEl4baOAQnbZN2mIQaiv6DsOkFjoiRCcqF4EDYJ9YakG/JiBqTAXAi6ZeDu75Tq27ZtV2vF0OsmC%2Bi%2Ba4fvJimsA6cK0BpWl8klTIiXgTDAEy7bIKNiboE57xNsaG6qXJiZOqOfLsuZGoPNy%2BofoxYUZvmS4dcNU7/psqRAUS6ERMQeAvQYlToEwOnvBCYIffB/YYqc5w3swbCNi8XxI5gKO2X2mADqcSqveFUUxfKEANolOzRZFSoCdl3lY35AVtMFNnIqirOYBTQJ1gLIKfF5LQ%2BZLSjSxJq6vmK5YU1ijCsBiDa1hzyvYBAiVRfrNaO4mbXEJuUPgQikXRdaVB86ryUi4LDvC7bXyqx9kJqpqRwan%2BhiYsQ5kWsa9xYwA1jHhCURqWfEgQ6AgCAx5p7cmSPddYp3OW%2B3vIeMdGhRTUxK17XxqTvWtP1o6wsXt7HUp52XSQ2kfaRuZsC0JZJiwY7lsJxreUDFontV6vvLV9ZyAw9A6ri9HEDn%2BUKNcyB4DQfI4vH%2Bx2fLA4mya82/dJ/JEBNiKzII90DlO7cDsfmCn3PvyPcsETbSnuI8WeyoCYb0bI1OIdInROCJGeKc/dTrGi8AwLANAIjAKoKvDeb85yck3OED%2BjgTwdTPHPX%2BLYT5n15MAzcoCbzgIeE8I2B03w1nwImBwtBzIkERHwgBBBBEiUZq4BEOYr5IhvmieoFw5w4PCEDVAMRtBiOvNcRsl9sETWIYaE878WifyoTmNAy1%2BQXF5MGEa%2BAqBUHiN3JaO4341ljomRsRwEBXGZM2Hy9JzJy0UegAmXwFCCgEBiFgeBgAIGFMnBgGdl5WnzoXXuNwy54KVm%2BKa1in7YMwLgoGCCmBILWDKL4BlY4AzIOPVm%2BMIEMAULKeJK5lR5y4kcOuWScnXTOA2F4q834AFlDB43RAoEAPjjRyPRCdBkwjGAjgJPDG8e9mxEmXpuZATp2SoE8BEj4kz3ywR%2BChXasoGE0FEI0Z4XTjzYJ6a0DEqi8HalGdwoSsl/z4AecYggOIz6MhshPHEiZik7Maug5S6lNLDz5Cc94GoG6UVMpeW8aS84FyLkwEu2SOmDLcNbB%2BJcERwsHoio8yKPpfQRL3BE/ALqaghiFOWDlvmfEyhlTy3xNYS38jrIKIV3bpmStFE21MWwZPxYSgZ6Aq5nGtklYW4d0oeS1dldUWosDXGwRDHMoLxh4v6cS96Kq3w/Q5OEdExBrjohmuOC8V4TY139FY%2BxoYFYOCBrGCG4rbwsWyERCwnZhIu1abiCNaS/QswGhY6NiD3ln2cZjeM5TKnrG5eKWg0c/kYDUiwswexKSESpDsVwoKc4XWZqkBymcLTcpIW4AwbdfgpKOBACE9oc4WiOHOU17NTCWGsF7MwGhSCBGFoOy8LVLVKnnaOqwY7soxAjSFftSBqxfFbe2g09wsEZx7X23OQ784jvHWOqKk7p0BGFnuo4YyhAADU3BHCdFwJI0odgAA4m2UTBKalA16rC3qnWTcYj6PgkIAOJuA/V7KQ0ouCoaOJOswkgNAig0BFOinaM5cBFNKDQpGMMaCwzhrDs7t2DuHTsJUK7x0QfvTBtep6jhJGpDsZAyAYgaEwCKAI6AROSCwFwCKf6NB/swGYTA2GNAaGQNJ79TAkhMDMFwP99KjTjnwPc7FloPzLrA5%2BeWiauyOPTcy4gdkPwBPiG0r4RMPq7FBbxWg%2BUwRggTl4RW2U6G1kpgas2tM6z0wEMjK0HTIzRnZpzT23NpVhZA/K0uFrlVuGGRFwWAdRZr3Fr5YVgUZYxcenFxWVqVYFciYK4rUtRXMmDobZUxsqZhYtgl62od7YHUdk7Vm6w3Yzg9pKnRpsaZpfNeXJVLIhl0zy4%2BoO/WA6hw%2BBqteuxq0Kmynah%2B4RbbjedsNo4kgzBqpShwFYtBOAil4H4bgvBUCcDcGZqJ6xNiVp4KQAgmhrsrDTiAAIEVpQuTMH%2BiKOwkj/vB6W/QnBJAPf%2B6QF7HBeAzKnX9jgWgVhwFgDARAKAb4OTIBQCAnL6AJGAAoZgqRaTMj4HQe1MyIAxBRzEcIrQLicB%2B5z5gicADymixG894MiAagvt485x7wLAG7gBuHpDMp7pAsDPOAOIWXavwIODwADFXWhgiqAAZ2LYP27X1BR/Oh4icPBYBR6nFgYvSD1JiBkTASoIKGE1/qf7KwqAGBp6%2B3kGpBcGpd/wQQIgxDsCkDIQQigVDqG17oboB7QNjssPoPAbZ4ArDxA8zgF7F3McsIEZ79TMZYBmZAFYdgxHZBcNgqYXRgjYPmCMaoaQMhZAEK3vQRQ%2B8ME75UUYMx6iN4EP0SYnhOhjEn3r6fExBhqIWOP2wK%2BB8zBX6PxYXB69VM2MsBHHB7ukEe0btHRxVB/oCHOAIkgjjAD492o9KT7QQFwIQYRuwD%2B8Gx1x1IAgCQDWAIFSE7HIEoEp3iEiDNk4Fv3v0f2f1fwgHfzTiWF4D5B/2rz0Cj2EFEHEC4G6HwKTzUBRzT1IFBi5DFxuzu2R21zR0F07AgKAkpEQIfyfxfxejQMI0/w8Hsip2%2B0wN%2B390BxAB2B2F/UkJkNkNkKCFuw4CR1IGd2pGlAihFChwigCC4B2EowihkxBwvxRzRwxxACx392AMJxAOJ0EPiCgIpxJypxQAPTuEIyZx3Cc0oHZ213525xdz8KFxFwcBdwl1HCl0ERR3ly8EV2Vxd3Vx9y1yN1ET1wNxRwlFN3RBd0t0UKNxt253ty2CNydxdzdw9y9w10vCMEsMDwegUBD0wDDwj1V3wJj3EDMBFAT3kCUHINT38H0B90z1XRsHnVr1tlRxRmyBVxLyvSzwsECHo3znnXZlMzmMr3iGry9Hzx6Cnz8AgFcG33b3QD3w3yH0aEOLOOyBOO7wbyXyaC3zn2mB2LuJn1X3KC7zGAeLyDb1mDaGuJPw%2Bw2DjzoLPwYKv04Bvzv04KogPTfz4O7W/0JGEIAMsJsLANYIcJgOIDgLYAQKhOQIOB9zhOPREOwMJDdDwNkDaLjxINkDIJTyN10DMCoLBloNP3P0v2e04GYPAIjTPA4IJNhN4JJO7QEIbSEN2DMBEMAIB1IGbCYCwASHGMUOUNUL/TB0kEkD/S1MkD0KkBBwinw05NR04DMIsNlzx2sKQCxMxKcNGEJKMDcOPQ8JZ28I5y50TgCI9IuGFy0RCNVzCMEAiJl2SMwAVyVwuhVx%2BwSKMCSLl110cDSO1wyOQDN2yIAlyN4HyLtyLUd0xmd1VzKKUAqMSL9wtL4CD3qND3D0YEjypMIPYA6K6PpIoJAGkAzzLxGNzzGIL0mKfGLwYyYzA0nXWOIE2J7OeMaGb3cEeLb1CDXw%2BO6EuP71nMH170aH%2BIn16GXwGEONuMaFeM3M313NXJ3zmAXLH2qEP0%2B2BPZLBK5I4EhKQKfwdINGFI/wRIBV/1LX/1EPLLRJYMgPJyxJxK2AFJfKFPQNJK/NwNpOjwbP8E6NIJ6IZJ0DbJZJoKexBI5JMO5MArYKfOhNfOJI/IgDFNJ0rUkGlLELlMwAVNGGVMR14FUKkMfy0x0Mo0kACB0ICBh2MMYNNNsHML/KAPxyJxtOArtISCdGQG5BuC/SSBuH/RuFUEfxdK8LZ3dIFxlz529N9NFwDLskl2lyiLDJiIjK83iO91jKKPjP4UTK2OTJN1TKyNVxyOt1z1twuEKLzKLFKPiHd2LOst92qPLNqOD2rOaJ%2B1aIQqbOQuT1bJ2AGOqM7JzzzzrwmKLw4GAwyU7KgwIFHPHO2P3Kb32Jb1PKOKPOXJyAquqqPJKp3Nn2%2BIX23PuPPPeMvM%2BJPOarPL%2BIvP32vKBJP0UJwoEsfNUqfxYAUBks/W/V/QAy/xguRJEuu2ANAPwttLsOxPgI4EQJNGmpegUulHyqwJgopLgoINjwkGZPit6MZIkIwtSDZJGvvJNI4B5NYM6gmv2pmqOv/VFKksrR2GootJWHlMVMoBBNVP8EkGlCSBFGEykEkCkG4w0CMONNMKEvNNx3EJ2DMF/SSAii4ECDRt0MJsNNPx2FesxplKWBBLMGpsEtppWHqWXDbKAA
*/
