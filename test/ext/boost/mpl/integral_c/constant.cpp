/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/mpl/integral_c.hpp>
#include <test/laws/constant.hpp>
using namespace boost::hana;


template <typename T>
void tests() {
    // value
    {
        static_assert(value(::boost::mpl::integral_c<T, 0>{}) == 0, "");
        static_assert(value(::boost::mpl::integral_c<T, 1>{}) == 1, "");
    }

    // laws
    {
        Constant_laws(
            ::boost::mpl::integral_c<T, 0>{},
            ::boost::mpl::integral_c<T, 1>{},
            ::boost::mpl::integral_c<T, 2>{},
            ::boost::mpl::integral_c<T, 3>{}
        );
    }
}

int main() {
    tests<int>();
    tests<long>();
}