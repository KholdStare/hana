/*!
@file
Defines `boost::hana::curry`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_CURRY_HPP
#define BOOST_HANA_FUNCTIONAL_CURRY_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/partial.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Curry a function up to the given number of arguments.
    //!
    //! [Currying][Wikipedia.currying] is a technique in which we consider a
    //! function taking multiple arguments (or, equivalently, a tuple of
    //! arguments), and turn it into a function which takes a single argument
    //! and returns a function to handle the remaining arguments. To help
    //! visualize, let's denote the type of a function `f` which takes
    //! arguments of types `X1, ..., Xn` and returns a `R` as
    //! @code
    //!     (X1, ..., Xn) -> R
    //! @endcode
    //!
    //! Then, currying is the process of taking `f` and turning it into an
    //! equivalent function (call it `g`) of type
    //! @code
    //!     X1 -> (X2 -> (... -> (Xn -> R)))
    //! @endcode
    //!
    //! This gives us the following equivalence, where `x1`, ..., `xn` are
    //! objects of type `X1`, ..., `Xn` respectively:
    //! @code
    //!     f(x1, ..., xn) == g(x1)...(xn)
    //! @endcode
    //!
    //! Currying can be useful in several situations, especially when working
    //! with higher-order functions.
    //!
    //! This `curry` utility is an implementation of currying in C++.
    //! Specifically, `curry<n>(f)` is a function such that
    //! @code
    //!     curry<n>(f)(x1)...(xn) == f(x1, ..., xn)
    //! @endcode
    //!
    //! Note that the `n` has to be specified explicitly because the existence
    //! of functions with variadic arguments in C++ make it impossible to know
    //! when currying should stop.
    //!
    //! Unlike usual currying, this implementation also allows a curried
    //! function to be called with several arguments at a time. Hence, the
    //! following always holds
    //! @code
    //!     curry<n>(f)(x1, ..., xk) == curry<n - k>(f)(x1)...(xk)
    //! @endcode
    //!
    //! Of course, this requires `k` to be less than or equal to `n`; failure
    //! to satisfy this will trigger a static assertion. This syntax is
    //! supported because it makes curried functions usable where normal
    //! functions are expected.
    //!
    //! Another "extension" is that `curry<0>(f)` is supported: `curry<0>(f)`
    //! is a nullary function; whereas the classical definition for currying
    //! seems to leave this case undefined, as nullary functions don't make
    //! much sense in purely functional languages.
    //! @snippet example/functional/curry.cpp curry0
    //!
    //!
    //! ### Example
    //! @snippet example/functional/curry.cpp main
    //!
    //! @todo
    //! Consider making `curry<0>(f)` equivalent to `f`. It might be more
    //! general or less useful if it creates subtle pitfalls.
    //!
    //! [Wikipedia.currying]: http://en.wikipedia.org/wiki/Currying
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t arity>
    constexpr auto curry = [](auto f) {
        return unspecified;
    };
#else
    namespace curry_detail {
        template <detail::std::size_t arity>
        struct curry_impl;
    }

    template <detail::std::size_t arity>
    constexpr curry_detail::curry_impl<arity> curry{};

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto curry<0> = [](auto f) {
        return [f(detail::std::move(f))]() -> decltype(auto) { return f(); };
    };

    namespace curry_detail {
        template <detail::std::size_t arity>
        BOOST_HANA_CONSTEXPR_LAMBDA auto curry_or_call = curry<arity>;

        template <>
        BOOST_HANA_CONSTEXPR_LAMBDA auto curry_or_call<0> = apply;

        template <detail::std::size_t arity>
        struct curry_impl {
            template <typename F>
            constexpr auto operator()(F f) const {
                return [f(detail::std::move(f))](auto&& ...x) {
                    static_assert(sizeof...(x) <= arity,
                    "too many arguments provided to boost::hana::curry");

                    return curry_or_call<arity - sizeof...(x)>(
                        partial(f, detail::std::forward<decltype(x)>(x)...)
                    );
                };
            };
        };
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_CURRY_HPP