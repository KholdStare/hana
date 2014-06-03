/*!
@file
Defines the @ref Functional module.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_HPP
#define BOOST_HANA_FUNCTIONAL_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    namespace functional_detail {
        template <typename F, unsigned Arity>
        struct curry_impl;

        template <typename F, typename G>
        struct argwise_impl;
    }

    //! @defgroup Functional Functional
    //! General purpose function objects.
    //! @{

    //! The identity function -- returns its argument unchanged.
    //!
    //! ### Example
    //! @snippet example/functional/id.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto id = [](auto x) {
        return x;
    };

    //! Returns a constant function returning `x` regardless of the
    //! argument(s) it is invoked with.
    //!
    //! ### Example
    //! @snippet example/functional/always.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto always = [](auto x) {
        return [=](auto ...y) { return x; };
    };

    //! Invokes `f` with `xs...` as arguments.
    //!
    //! ### Example
    //! @snippet example/functional/apply.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto apply = [](auto f, auto ...xs) {
        return f(xs...);
    };

    /*!
    Allows argument-wise function composition.

    Specifically, `argwise(f)` is a function object equivalent to `f`
    and also a `Functor` with the following definition for `fmap`:
    @code
        fmap(g, argwise(f))(xs...) = f(g(xs)...)
    @endcode

    ### Example
    @snippet example/functional/argwise.cpp main

    @hideinitializer
     */
    BOOST_HANA_CONSTEXPR_LAMBDA auto argwise = [](auto f) {
        return functional_detail::argwise_impl<decltype(f), decltype(id)>{f, id};
    };

    /*!
    Returns the composition of two functions.

    @note
    `compose` is an associative operation; `compose(f, compose(g, h))`
    is equivalent to `compose(compose(f, g), h)`.

    ### Example
    @snippet example/functional/compose.cpp main

    @internal
    ### Proof of associativity (`.` is `compose`)
    @code{haskell}
        f . (g . h) $ x xs... == f ((g . h) x) xs...
                              == f (g (h x)) xs...

        (f . g) . h $ x xs... == (f . g) (h x) xs...
                              == f (g (h x)) xs...
    @endcode
     */
    BOOST_HANA_CONSTEXPR_LAMBDA auto compose = [](auto f, auto g) {
        return [=](auto x, auto ...xs) { return f(g(x), xs...); };
    };

    /*!
    Returns a curriable function of the given `arity`.

    Specifically, `curry<n>(f)` is a function which can be curried with up to
    `n` arguments. However, unlike with usual currying, a curried function may
    still be called with several arguments at a time; `curry<n>(f)(x1, ..., xN)`
    is equivalent to `curry<n>(x1)...(xN)`.

    ### Example
    @snippet example/functional/curry.cpp main

    @note
    As expected, `curry<0>(f)` is a function which can be curried with up to
    `0` arguments. Hence, it may only be called as follows: `curry<0>(f)()`.
    @snippet example/functional/curry.cpp curry0

    @todo
    Consider making `curry<0>(f)` equivalent to `f`. It might be more general
    or less useful if it creates subtle pitfalls.

    @hideinitializer
     */
    template <unsigned arity>
    BOOST_HANA_CONSTEXPR_LAMBDA auto curry = [](auto f) {
        return functional_detail::curry_impl<decltype(f), arity>{f};
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto curry<0> = [](auto f) {
        return [=] { return f(); };
    };

    /*!
    Computes the least fixed point of a function.

    Specifically, `fix(f, xs...)` is equivalent to `f(fix(f), xs...)`, where
    `fix(f)` is `fix` partially applied to `f`. `fix` is an implementation of
    the Y-combinator, also called the fixed-point combinator. It encodes the
    idea of recursion, and in fact any recursive function can be written in
    terms of it.

    ### Example
    @snippet example/functional/fix.cpp main
     */
    constexpr struct {
        template <typename F>
        constexpr auto operator()(F f) const {
            auto fst = [](auto a, ...) { return a; };
            //! @todo Find a simpler way to make `f` dependent.
            return [=](auto ...xs) {
                return f((*this)(fst(f, xs...)), xs...);
            };
        }
    } fix{};

    //! Returns a function invoking `f` with its two first arguments reversed.
    //!
    //! ### Example
    //! @snippet example/functional/flip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto flip = [](auto f) {
        return [=](auto x, auto y, auto ...z) { return f(y, x, z...); };
    };

    //! Returns `f` partially applied to `xs...`.
    //!
    //! ### Example
    //! @snippet example/functional/partial.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto partial = [](auto f, auto ...xs) {
        return [=](auto ...ys) { return f(xs..., ys...); };
    };

    /*!
    Allows creating simple functions inline.

    Common operators are overloaded to return a function object calling
    the corresponding operator on its arguments. Operators may also be
    partially applied to one argument to create Haskell-like "sections".

    ### Overloaded operators
    - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`, unary `-`
    - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    - Logical: `||`, `&&`, `!`
    - Member access: `*` (dereference)


    ### Example
    @snippet example/functional/placeholder.cpp main

    @todo
    Consider allowing placeholder expressions to be called with more arguments
    than required. The extraneous arguments could just be discarded as with
    `boost::bind`. This could be useful to implement e.g. `length` as a
    wrapper over `foldl`.
     */
    constexpr struct { } _{};

    //! @}

#define BOOST_HANA_PLACEHOLDER_BINARY_OP(op)                                \
    template <typename X>                                                   \
    constexpr auto operator op (X x, decltype(_))                           \
    { return [=](auto y) { return x op y; }; }                              \
                                                                            \
    template <typename Y>                                                   \
    constexpr auto operator op (decltype(_), Y y)                           \
    { return [=](auto x) { return x op y; }; }                              \
                                                                            \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (decltype(_), decltype(_)) \
    { return [](auto x, auto y) { return x op y; }; }                       \
/**/

#define BOOST_HANA_PLACEHOLDER_UNARY_OP(op)                                 \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (decltype(_))              \
    { return [](auto x) { return op x; }; }                                 \
/**/
    // Arithmetic
    BOOST_HANA_PLACEHOLDER_UNARY_OP(+)
    BOOST_HANA_PLACEHOLDER_UNARY_OP(-)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(+)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(-)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(*)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(/)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(%)
    BOOST_HANA_PLACEHOLDER_UNARY_OP(~)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(&)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(|)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(^)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(<<)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(>>)

    // Comparison
    BOOST_HANA_PLACEHOLDER_BINARY_OP(==)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(!=)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(<)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(<=)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(>)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(>=)

    // Logical
    BOOST_HANA_PLACEHOLDER_BINARY_OP(||)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(&&)
    BOOST_HANA_PLACEHOLDER_UNARY_OP(!)

    // Member access
    BOOST_HANA_PLACEHOLDER_UNARY_OP(*)
#undef BOOST_HANA_PREFIX_PLACEHOLDER_OP
#undef BOOST_HANA_BINARY_PLACEHOLDER_OP

    namespace functional_detail {
        struct Argwise;

        template <typename F, typename G>
        struct argwise_impl {
            F f;
            G g;
            using hana_datatype = Argwise;

            template <typename ...Xs>
            constexpr auto operator()(Xs ...xs) const
            { return f(g(xs)...); }
        };


        template <typename F, unsigned Arity>
        struct curry_impl {
            F f;

            template <typename ...Args>
            constexpr auto operator()(Args ...args) const
            { return curry<Arity - sizeof...(Args)>(partial(f, args...))(); }

            constexpr auto operator()() const
            { return *this; }
        };
    }

    template <>
    struct Functor<functional_detail::Argwise> {
        template <typename H, typename F, typename G>
        static constexpr auto fmap_impl(H h, functional_detail::argwise_impl<F, G> aw) {
            return functional_detail::argwise_impl<F, decltype(compose(h, aw.g))>{
                aw.f, compose(h, aw.g)
            };
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_HPP