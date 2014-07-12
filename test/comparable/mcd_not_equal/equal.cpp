/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>

#include "minimal.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(equal(comparable<0>, comparable<0>));
    BOOST_HANA_STATIC_ASSERT(!(equal(comparable<0>, comparable<1>)));

    using operators::operator==;
    BOOST_HANA_STATIC_ASSERT(comparable<0> == comparable<0>);
    BOOST_HANA_STATIC_ASSERT(!(comparable<0> == comparable<1>));
}