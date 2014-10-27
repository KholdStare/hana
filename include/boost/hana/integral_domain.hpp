/*!
@file
Defines `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_INTEGRAL_DOMAIN_HPP

#include <boost/hana/fwd/integral_domain.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `quot` and `mod`
    struct IntegralDomain::mcd { };

    namespace operators {
        //! Equivalent to `mod`.
        //! @relates boost::hana::IntegralDomain
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<IntegralDomain, datatype_t<X>>::value ||
            enable_operators<IntegralDomain, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator%(X&& x, Y&& y)
        { return mod(detail::std::forward<X>(x), detail::std::forward<Y>(y)); }

        //! Equivalent to `quot`.
        //! @relates boost::hana::IntegralDomain
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<IntegralDomain, datatype_t<X>>::value ||
            enable_operators<IntegralDomain, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator/(X&& x, Y&& y)
        { return quot(detail::std::forward<X>(x), detail::std::forward<Y>(y)); }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_HPP