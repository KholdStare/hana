/*!
@file
Forward declares `boost::hana::Ring`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_RING_HPP
#define BOOST_HANA_FWD_RING_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Ring` concept represents `Group`s that also form a `Monoid`
    //! under a second binary operation that distributes over the first.
    //!
    //! A [Ring][1] is an algebraic structure built on top of a `Group`
    //! which requires a monoidal structure with respect to a second binary
    //! operation. This second binary operation must distribute over the
    //! first one. Specifically, a `Ring` is a triple `(S, +, *)` such that
    //! `(S, +)` is a `Group`, `(S, *)` is a `Monoid` and `*` distributes
    //! over `+`, i.e.
    //! @code
    //!     x * (y + z) == (x * y) + (x * z)
    //! @endcode
    //!
    //! The second binary operation is often written `*` with its identity
    //! written `1`, in reference to the `Ring` of integers under
    //! multiplication. The method names used here refer to this exact ring.
    //!
    //!
    //! Superclasses
    //! ------------
    //! `Monoid`, `Group`
    //!
    //!
    //! Laws
    //! ----
    //! For all objects `x`, `y`, `z` of a `Ring` `R`, the following laws must
    //! be satisfied:
    //! @code
    //!     mult(x, mult(y, z)) == mult(mult(x, y), z)          // associativity
    //!     mult(x, one<R>()) == x                              // right identity
    //!     mult(one<R>(), x) == x                              // left identity
    //!     mult(x, plus(y, z)) == plus(mult(x, y), mult(x, z)) // distributivity
    //! @endcode
    //!
    //!
    //! Minimal complete definintion
    //! ----------------------------
    //! `one` and `mult` satisfying the above laws
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. For non-boolean arithmetic data types\n
    //! A data type `T` is arithmetic if `std::is_arithmetic<T>::%value` is
    //! true. For a non-boolean arithmetic data type `T`, a model of `Ring` is
    //! automatically defined by using the provided `Group` model and setting
    //! @code
    //!     mult(x, y) = (x * y)
    //!     one<T>() = static_cast<T>(1)
    //! @endcode
    //!
    //! @note
    //! The rationale for not providing a Ring model for `bool` is the same
    //! as for not providing Monoid and Group models.
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operator is provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     * -> mult
    //! @endcode
    //!
    //!
    //! Structure-preserving functions
    //! ------------------------------
    //! Let `A` and `B` be two `Ring`s. A function `f : A -> B` is said to
    //! be a [Ring morphism][2] if it preserves the ring structure between
    //! `A` and `B`. Rigorously, for all objects `x, y` of data type `A`,
    //! @code
    //!     f(plus(x, y)) == plus(f(x), f(y))
    //!     f(mult(x, y)) == mult(f(x), f(y))
    //!     f(one<A>()) == one<B>()
    //! @endcode
    //! Because of the `Ring` structure, it is easy to prove that the
    //! following will then also be satisfied:
    //! @code
    //!     f(zero<A>()) == zero<B>()
    //!     f(negate(x)) == negate(f(x))
    //! @endcode
    //! which is to say that `f` will then also be a `Group` morphism.
    //! Functions with these properties interact nicely with `Ring`s,
    //! which is why they are given such a special treatment.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Ring_(mathematics)
    //! [2]: http://en.wikipedia.org/wiki/Ring_homomorphism
    struct Ring { };

    //! Associative operation of a `Ring`.
    //! @relates Ring
    //!
    //! @param x, y
    //! Two `Ring` elements to combine with the `Ring` binary operation.
    //!
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! The `mult` method is "overloaded" to handle distinct data types
    //! with certain properties. Specifically, `mult` is defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `Ring`s when taken individually
    //! 3. `to<C> : A -> B` and `to<C> : B -> C` are `Ring`-embeddings, as
    //!    determined by the `is_embedding` metafunction.
    //!
    //! The definition of `mult` for data types satisfying the above
    //! properties is obtained by setting
    //! @code
    //!     mult(x, y) = mult(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/ring.cpp mult
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto mult = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct mult_impl;

    struct _mult {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return mult_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _mult mult{};
#endif

    //! Identity of the `Ring` multiplication.
    //! @relates Ring
    //!
    //! @tparam R
    //! The data type (a `Ring`) of the returned identity.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/ring.cpp one
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename R>
    constexpr auto one = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename R, typename = void>
    struct one_impl;

    template <typename R>
    struct _one {
        constexpr decltype(auto) operator()() const {
            return one_impl<R>::apply();
        }
    };

#ifdef BOOST_HANA_CONFIG_HAS_VARIABLE_TEMPLATES
    template <typename R>
    constexpr _one<R> one{};
#else
    template <typename R>
    constexpr decltype(auto) one()
    { return _one<R>{}(); }
#endif
#endif

    //! Elevate a ring element to its `n`th power.
    //! @relates Ring
    //!
    //! Specifically, `power(r, n)`, is equivalent to multiplying `r` with
    //! itself `n` times using the `Ring` multiplication. If the power is
    //! equal to `zero`, the `Ring` identity (`one`) is returned.
    //!
    //! @param r
    //! A `Ring` element that is elevated to its `n`th power.
    //!
    //! @param n
    //! An integral `Constant` representing the power to which `r` is elevated.
    //!
    //!
    //! @note
    //! Only the data type of `r` is used for tag-dispatching.
    //!
    //! Example
    //! -------
    //! @snippet example/ring.cpp power
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto power = [](auto&& r, auto&& n) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename R, typename = void>
    struct power_impl;

    struct _power {
        template <typename R, typename N>
        constexpr decltype(auto) operator()(R&& r, N&& n) const {
            return power_impl<typename datatype<R>::type>::apply(
                detail::std::forward<R>(r),
                detail::std::forward<N>(n)
            );
        }
    };

    constexpr _power power{};
#endif

    namespace operators {
        template <>
        struct of<Ring>
            : decltype(mult)
        { };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RING_HPP
