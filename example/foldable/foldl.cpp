/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <boost/hana/detail/assert.hpp>

#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    //! [main]
    auto to_string = [](auto x) { return (std::ostringstream{} << x).str(); };

    auto show = [=](auto x, auto y) {
        return "(" + to_string(x) + " + " + to_string(y) + ")";
    };

    BOOST_HANA_RUNTIME_ASSERT(foldl(list(2, "3", '4'), "1", show) == "(((1 + 2) + 3) + 4)");
    //! [main]
}