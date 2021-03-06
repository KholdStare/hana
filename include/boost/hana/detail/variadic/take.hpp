/*!
@file
Defines `boost::hana::detail::variadic::take`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_TAKE_HPP
#define BOOST_HANA_DETAIL_VARIADIC_TAKE_HPP

#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/detail/variadic/split_at.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/reverse_partial.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    struct _take_impl2 {
        template <typename F, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, Xs&& ...xs) const {
            return static_cast<F&&>(f)(static_cast<Xs&&>(xs)...);
        }
    };

    struct _take_impl1 {
        template <typename ...Xs>
        constexpr auto operator()(Xs&& ...xs) const {
            return hana::always(
                reverse_partial(_take_impl2{},
                    static_cast<Xs&&>(xs)...)
            );
        }
    };

    template <detail::std::size_t n>
    struct _take {
        template <typename ...Xs>
        constexpr decltype(auto) operator()(Xs&& ...xs) const {
            return split_at<n>(static_cast<Xs&&>(xs)...)(_take_impl1{});
        }
    };

    template <detail::std::size_t n>
    constexpr _take<n> take{};
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_TAKE_HPP
