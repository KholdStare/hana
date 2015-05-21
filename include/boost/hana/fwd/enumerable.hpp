/*!
@file
Forward declares `boost::hana::Enumerable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ENUMERABLE_HPP
#define BOOST_HANA_FWD_ENUMERABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/static_constexpr.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! Represents data types whose values can be enumerated.
    //!
    //! `Enumerable` provides the `succ` and `pred` methods, whose names
    //! come from the [successor][1] and predecessor functions used when
    //! defining the natural numbers with the Peano axioms. Those functions
    //! allow the values of a data type to be enumerated. Note that an
    //! `Enumerable` data type `E` may contain infinitely many values;
    //! `pred` and `succ` can be implemented as long as the infinity is
    //! [countable][2] (in its mathematical sense).
    //!
    //!
    //! Laws
    //! ----
    //! For any `Enumerable x`, the following laws must be satisfied:
    //! @code
    //!     succ(pred(x)) == x
    //!     pred(succ(x)) == x
    //! @endcode
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `succ` and `pred` satisfying the above laws
    //!
    //!
    //! Free model for non-boolean arithmetic data types
    //! ------------------------------------------------
    //! A data type `T` is arithmetic if `std::is_arithmetic<T>::%value` is
    //! true. For a non-boolean arithmetic data type `T`, a model of
    //! `Enumerable` is automatically defined by setting
    //! @code
    //!     succ(x) = ++x
    //!     pred(x) = --x
    //! @endcode
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `IntegralConstant`
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Successor_function
    //! [2]: http://en.wikipedia.org/wiki/Countable_set
    struct Enumerable { };

    //! Returns the successor of a value.
    //! @relates Enumerable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/enumerable.cpp succ
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto succ = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename E, typename = void>
    struct succ_impl;

    struct _succ {
        template <typename N>
        constexpr decltype(auto) operator()(N&& n) const {
            using E = typename datatype<N>::type;
            using Succ = succ_impl<E>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Enumerable, E>{},
            "hana::succ(n) requires n to be Enumerable");
        #endif

            return Succ::apply(static_cast<N&&>(n));
        }
    };

    namespace {
        constexpr auto const& succ = detail::static_constexpr<_succ>;
    }
#endif

    //! Returns the predecessor of a value.
    //! @relates Enumerable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/enumerable.cpp pred
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto pred = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename E, typename = void>
    struct pred_impl;

    struct _pred {
        template <typename N>
        constexpr decltype(auto) operator()(N&& n) const {
            using E = typename datatype<N>::type;
            using Pred = pred_impl<E>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Enumerable, E>{},
            "hana::pred(n) requires n to be Enumerable");
        #endif

            return Pred::apply(static_cast<N&&>(n));
        }
    };

    namespace {
        constexpr auto const& pred = detail::static_constexpr<_pred>;
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ENUMERABLE_HPP
