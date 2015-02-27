/*!
@file
Defines `boost::hana::flip`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_FLIP_HPP
#define BOOST_HANA_FUNCTIONAL_FLIP_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Invoke a function with its two first arguments reversed.
    //!
    //! Specifically, `flip(f)` is a function such that
    //! @code
    //!     flip(f)(x, y, z...) == f(y, x, z...)
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/functional/flip.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto flip = [](auto&& f) {
        return [perfect-capture](auto&& x, auto&& y, auto&& ...z) -> decltype(auto) {
            return forwarded(f)(forwarded(y), forwarded(x), forwarded(z)...);
        };
    };
#else
    template <typename F>
    struct _flip {
        F f;

        template <typename X, typename Y, typename ...Z>
        constexpr decltype(auto) operator()(X&& x, Y&& y, Z&& ...z) const& {
            return f(
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x),
                detail::std::forward<Z>(z)...
            );
        }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename X, typename Y, typename ...Z>
        constexpr decltype(auto) operator()(X&& x, Y&& y, Z&& ...z) & {
            return f(
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x),
                detail::std::forward<Z>(z)...
            );
        }
#endif

        template <typename X, typename Y, typename ...Z>
        constexpr decltype(auto) operator()(X&& x, Y&& y, Z&& ...z) && {
            return detail::std::move(f)(
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x),
                detail::std::forward<Z>(z)...
            );
        }
    };

    constexpr detail::create<_flip> flip{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_FLIP_HPP
