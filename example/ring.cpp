/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    {
        //! [mult]
        BOOST_HANA_CONSTANT_ASSERT(mult(int_<3>, int_<5>) == int_<15>);
        BOOST_HANA_CONSTEXPR_ASSERT(mult(4, 2) == 8);
        //! [mult]
    }
    {
        //! [one]
        BOOST_HANA_CONSTANT_ASSERT(one<Integral> == int_<1>);
        BOOST_HANA_CONSTEXPR_ASSERT(one<long> == 1l);
        //! [one]
    }
    {
        //! [power]
        BOOST_HANA_CONSTANT_ASSERT(power(int_<3>, int_<2>) == int_<3 * 3>);
        BOOST_HANA_CONSTEXPR_ASSERT(power(2, int_<4>) == 16);
        //! [power]
    }
}