/*!
@file
Forward declares `boost::hana::is_a` and `boost::hana::is_an`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CORE_IS_A_HPP
#define BOOST_HANA_FWD_CORE_IS_A_HPP

#include <boost/hana/detail/static_constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Returns whether the data type of an object matches a given data type.
    //!
    //! Given a data type `D` and a C++ type `T`, `is_a<D, T>` is a
    //! compile-time Logical representing whether the data type of `T`
    //! is exactly `D`. In other words, it is equivalent to
    //! @code
    //!     std::is_same<D, datatype_t<T>>
    //! @endcode
    //!
    //! For convenience, an alternate syntax is provided for using `is_a`.
    //! Specifically, `is_a<D>` is a function object returning whether the
    //! argument it is passed is of data type `D`. In other words,
    //! @code
    //!     is_a<D>(x) == is_a<D, decltype(x)>
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/is_a.cpp is_a
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename DataType, typename optional_T>
    constexpr auto is_a = see-documentation;
#else
    template <typename DataType, typename ...T>
    struct _is_a;

    namespace {
        template <typename DataType, typename ...T>
        constexpr auto const& is_a = detail::static_constexpr<_is_a<DataType, T...>>;
    }
#endif

    //! @ingroup group-core
    //! Equivalent to `is_a`; provided for consistency with the rules of the
    //! English language.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename DataType, typename ...T>
    constexpr auto is_an = is_a<DataType, T...>;
#else
    namespace {
        template <typename DataType, typename ...T>
        constexpr auto const& is_an = is_a<DataType, T...>;
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CORE_IS_A_HPP
