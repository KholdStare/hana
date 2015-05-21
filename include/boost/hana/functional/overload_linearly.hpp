/*!
@file
Defines `boost::hana::overload_linearly`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_OVERLOAD_LINEARLY_HPP
#define BOOST_HANA_FUNCTIONAL_OVERLOAD_LINEARLY_HPP

#include <boost/hana/detail/static_constexpr.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/declval.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Call the first function that produces a valid call expression.
    //!
    //! Given functions `f1, ..., fn`, `overload_linearly(f1, ..., fn)` is
    //! a new function that calls the first `fk` producing a valid call
    //! expression with the given arguments. Specifically,
    //! @code
    //!     overload_linearly(f1, ..., fn)(args...) == fk(args...)
    //! @endcode
    //!
    //! where `fk` is the _first_ function such that `fk(args...)` is a valid
    //! expression.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functional.cpp overload_linearly
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto overload_linearly = [](auto&& f1, auto&& f2, ..., auto&& fn) {
        return [perfect-capture](auto&& ...x) -> decltype(auto) {
            return forwarded(fk)(forwarded(x)...);
        };
    };
#else
    template <typename F, typename G>
    struct _overload_linearly {
        F f;
        G g;

    private:
        template <typename ...Args, typename =
            decltype(detail::std::declval<F const&>()(detail::std::declval<Args>()...))>
        constexpr F const& which(int) const& { return f; }

        template <typename ...Args, typename =
            decltype(detail::std::declval<F&>()(detail::std::declval<Args>()...))>
        constexpr F& which(int) & { return f; }

        template <typename ...Args, typename =
            decltype(detail::std::declval<F&&>()(detail::std::declval<Args>()...))>
        constexpr F which(int) && { return static_cast<F&&>(f); }

        template <typename ...Args>
        constexpr G const& which(long) const& { return g; }

        template <typename ...Args>
        constexpr G& which(long) & { return g; }

        template <typename ...Args>
        constexpr G which(long) && { return static_cast<G&&>(g); }

    public:
        template <typename ...Args>
        constexpr decltype(auto) operator()(Args&& ...args) const&
        { return which<Args...>(int{})(static_cast<Args&&>(args)...); }

        template <typename ...Args>
        constexpr decltype(auto) operator()(Args&& ...args) &
        { return which<Args...>(int{})(static_cast<Args&&>(args)...); }

        template <typename ...Args>
        constexpr decltype(auto) operator()(Args&& ...args) &&
        { return which<Args...>(int{})(static_cast<Args&&>(args)...); }
    };

    struct _make_overload_linearly {
        template <typename F, typename G>
        constexpr _overload_linearly<
            typename detail::std::decay<F>::type,
            typename detail::std::decay<G>::type
        > operator()(F&& f, G&& g) const {
            return {static_cast<F&&>(f), static_cast<G&&>(g)};
        }

        template <typename F, typename G, typename ...H>
        constexpr decltype(auto) operator()(F&& f, G&& g, H&& ...h) const {
            return (*this)(static_cast<F&&>(f),
                    (*this)(static_cast<G&&>(g), static_cast<H&&>(h)...));
        }
    };

    namespace {
        constexpr auto const& overload_linearly =
                            detail::static_constexpr<_make_overload_linearly>;
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_OVERLOAD_LINEARLY_HPP
