/*!
@file
Defines `boost::hana::ext::boost::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_TUPLE_HPP
#define BOOST_HANA_EXT_BOOST_TUPLE_HPP

#include <boost/hana/fwd/ext/boost/tuple.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/sequence.hpp>

#include <boost/tuple/tuple.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<ext::boost::Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& tuple)
        { return detail::std::forward<Xs>(tuple).get_head(); }
    };

    template <>
    struct tail_impl<ext::boost::Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& tuple)
        { return detail::std::forward<Xs>(tuple).get_tail(); }
    };

    template <>
    struct is_empty_impl<ext::boost::Tuple> {
        static constexpr auto apply(::boost::tuples::null_type const&)
        { return _true{}; }

        template <typename H, typename T>
        static constexpr auto apply(::boost::tuples::cons<H, T> const&)
        { return _false{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct prepend_impl<ext::boost::Tuple> {
        template <typename X, typename Xs>
        static constexpr auto apply(X x, Xs xs) {
            return ::boost::tuples::cons<X, Xs>{
                detail::std::move(x),
                detail::std::move(xs)
            };
        }
    };

    template <>
    struct empty_impl<ext::boost::Tuple> {
        static constexpr auto apply()
        { return ::boost::tuples::null_type{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Sequence(ext::boost::Tuple)>
        : detail::std::true_type
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_HPP
