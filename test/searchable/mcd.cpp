/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <boost/hana/foreign.hpp> //! @todo Get rid of this.

#include <test/cnumeric/comparable.hpp>
#include <test/cnumeric/constant.hpp>
#include <test/numeric/comparable.hpp>
#include <test/numeric/logical.hpp>
#include <test/seq/searchable.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

template <int i>
constexpr auto c_comparable = test::cnumeric<int, i>;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto searchable = test::seq;
    BOOST_HANA_CONSTEXPR_LAMBDA auto comparable = test::numeric;
    BOOST_HANA_CONSTEXPR_LAMBDA auto logical = test::numeric;
    constexpr struct { } undefined{};

    // all
    {
        BOOST_HANA_CONSTANT_ASSERT(all(searchable(), undefined));

        BOOST_HANA_CONSTEXPR_ASSERT(
            all(searchable(comparable(0)), is(comparable(0)))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            not_(all(searchable(comparable(0)), is(comparable(1))))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(not_(
            all(searchable(comparable(0), comparable(1)), is(comparable(0)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(
            all(searchable(comparable(0), comparable(0)), is(comparable(0)))
        );
    }

    // all_of
    {
        BOOST_HANA_CONSTANT_ASSERT(
            all_of(searchable())
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            all_of(searchable(logical(true)))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            not_(all_of(searchable(logical(false))))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            all_of(searchable(logical(true), logical(true)))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(not_(
            all_of(searchable(logical(true), logical(false)))
        ));
    }

    // any
    {
        BOOST_HANA_CONSTANT_ASSERT(not_(
            any(searchable(), undefined)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(
            any(searchable(comparable(0)), is(comparable(0)))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            not_(any(searchable(comparable(0)), is(comparable(1))))
        );
    }

    // any_of
    {
        BOOST_HANA_CONSTANT_ASSERT(not_(
            any_of(searchable())
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(
            any_of(searchable(logical(true)))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            not_(any_of(searchable(logical(false))))
        );
    }

    // none
    {
        BOOST_HANA_CONSTANT_ASSERT(none(searchable(), undefined));

        BOOST_HANA_CONSTEXPR_ASSERT(
            none(searchable(comparable(0)), is(comparable(1)))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            not_(none(searchable(comparable(0)), is(comparable(0))))
        );
    }

    // none_of
    {
        BOOST_HANA_CONSTANT_ASSERT(none_of(searchable()));

        BOOST_HANA_CONSTEXPR_ASSERT(
            none_of(searchable(logical(false)))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            not_(none_of(searchable(logical(true))))
        );
    }

    // elem
    {
        BOOST_HANA_CONSTEXPR_ASSERT(
            elem(searchable(comparable(0)), comparable(0))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            not_(elem(searchable(comparable(0)), comparable(1)))
        );
    }

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(searchable(c_comparable<0>), is(c_comparable<0>)),
            just(c_comparable<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(searchable(c_comparable<0>), is(c_comparable<1>)),
            nothing
        ));
    }

    // lookup
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            lookup(searchable(c_comparable<0>), c_comparable<0>),
            just(c_comparable<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            lookup(searchable(c_comparable<0>), c_comparable<1>),
            nothing
        ));
    }

    // in
    {
        BOOST_HANA_CONSTEXPR_ASSERT(
            comparable(0) ^in^ searchable(comparable(0))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(not_(
            comparable(1) ^in^ searchable(comparable(0))
        ));
    }

    // subset
    {
        BOOST_HANA_CONSTEXPR_ASSERT(subset(
            searchable(comparable(0)),
            searchable(comparable(0))
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(subset(
            searchable(comparable(0)),
            searchable(comparable(1)))
        ));
    }
}