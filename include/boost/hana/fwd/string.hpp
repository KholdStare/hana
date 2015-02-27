/*!
@file
Forward declares `boost::hana::String`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_STRING_HPP
#define BOOST_HANA_FWD_STRING_HPP

#include <boost/hana/config.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time string.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! For most purposes, a `String` is functionally equivalent to a tuple
    //! holding `Constant`s of underlying type `char`.
    //!
    //! 1. `Comparable` (operators provided)\n
    //! Two `String`s are equal if and only if they have the same number of
    //! characters and characters at corresponding indices are equal.
    //! @snippet example/string.cpp comparable
    //!
    //! 2. `Orderable` (operators provided)\n
    //! The strict weak ordering implemented for `Orderable` is the usual
    //! lexicographical comparison of strings.
    //! @snippet example/string.cpp orderable
    //!
    //! 3. `Foldable`\n
    //! Folding a `String` is equivalent to folding the sequence of its
    //! characters.
    //! @snippet example/string.cpp foldable
    //!
    //! 4. `Iterable` (operators provided)\n
    //! Iterating over a `String` is equivalent to iterating over the sequence
    //! of its characters.
    //! @snippet example/string.cpp iterable
    //!
    //! 5. `Searchable`\n
    //! Searching through a `String` is equivalent to searching through the
    //! sequence of its characters.
    //! @snippet example/string.cpp searchable
    //!
    //!
    //! > #### Rationale for `String` not being a `Constant`
    //! > The underlying type held by a `String` could be either `char const*`
    //! > or some other constexpr-enabled string-like container. In the first
    //! > case, `String` can not be a `Constant` because the models of several
    //! > concepts would not be respected by the underlying type, causing
    //! > `value` not to be structure-preserving. Providing an underlying
    //! > value of constexpr-enabled string-like container type like
    //! > `std::string_view` would be great, but that's a bit complicated
    //! > for the time being.
    //!
    //!
    //! @todo
    //! Right now, we use a different template specialization to store each
    //! different string. This could also work by using an anonymous type,
    //! but there seems to be a bug in Clang right now that makes this fail
    //! in some circumstances: http://llvm.org/bugs/show_bug.cgi?id=20625.
    //! Using an anonymous type could have compile-time performance benefits,
    //! so this avenue should be explored once the bug is fixed.
    //!
    //! @todo
    //! The `BOOST_HANA_STRING` macro does not appear near `String` in the
    //! documentation.
    struct String { using value_type = char const*; };

    //! Create a compile-time string from a parameter pack of characters.
    //! @relates String
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/string.cpp string
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <char ...s>
    constexpr unspecified-type string{};
#else
    template <char ...s>
    struct _string;

#ifdef BOOST_HANA_CONFIG_HAS_VARIABLE_TEMPLATES
    template <char ...s>
    constexpr _string<s...> string{};
#endif
#endif

    //! Create a compile-time string from a string literal `s`.
    //! @relates String
    //!
    //! This macro is a more convenient alternative to `string` for creating
    //! compile-time strings. However, since this macro uses a lambda
    //! internally, it can't be used in an unevaluated context.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/string.cpp BOOST_HANA_STRING
#ifdef BOOST_HANA_DOXYGEN_INVOKED
#   define BOOST_HANA_STRING(s) unspecified
#else
    // defined in boost/hana/string.hpp
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_STRING_HPP
