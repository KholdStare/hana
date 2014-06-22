/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include "minimal_logical.hpp"
using namespace boost::hana;


struct T; struct E;

int main() {
    BOOST_HANA_STATIC_ASSERT(if_(logical<true>, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(if_(logical<false>, type<T>, type<E>) == type<E>);
}