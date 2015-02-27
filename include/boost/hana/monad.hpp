/*!
@file
Defines `boost::hana::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_HPP
#define BOOST_HANA_MONAD_HPP

#include <boost/hana/fwd/monad.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename Xs, typename F, typename = typename detail::std::enable_if<
            has_operator<datatype_t<Xs>, decltype(bind)>::value
        >::type>
        constexpr decltype(auto) operator|(Xs&& xs, F&& f) {
            return hana::bind(detail::std::forward<Xs>(xs),
                              detail::std::forward<F>(f));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // bind
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct bind_impl : bind_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct bind_impl<M, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            using Monad = detail::dependent_on_t<sizeof(xs) == 1, M>;
            static_assert(!is_default<flatten_impl<Monad>>{},
            "no definition of boost::hana::bind for the given data type");

            return hana::flatten(hana::transform(detail::std::forward<Xs>(xs),
                                                 detail::std::forward<F>(f)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // flatten
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct flatten_impl : flatten_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct flatten_impl<M, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using Monad = detail::dependent_on_t<sizeof(xs) == 1, M>;
            static_assert(!is_default<bind_impl<Monad>>{},
            "no definition of boost::hana::flatten for the given data type");

            return hana::bind(detail::std::forward<Xs>(xs), id);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // mcompose
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct mcompose_impl : mcompose_impl<M, when<true>> { };

    namespace monad_detail {
        template <typename F, typename G>
        struct mcompose {
            F f; G g;
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) const& {
                return hana::bind(f(detail::std::forward<X>(x)), g);
            }
#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) & {
                return hana::bind(f(detail::std::forward<X>(x)), g);
            }
#endif
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) && {
                return hana::bind(detail::std::move(f)(detail::std::forward<X>(x)),
                                  detail::std::move(g));
            }
        };
    }

    template <typename M, bool condition>
    struct mcompose_impl<M, when<condition>> : default_ {
        template <typename F, typename G>
        static constexpr decltype(auto) apply(F&& f, G&& g) {
            return detail::create<monad_detail::mcompose>{}(
                detail::std::forward<F>(f), detail::std::forward<G>(g));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // then
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct then_impl : then_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct then_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return hana::bind(detail::std::forward<Xs>(xs),
                              hana::always(detail::std::forward<Ys>(ys)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // tap
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct tap_impl : tap_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct tap_impl<M, when<condition>> : default_ {
        template <typename F>
        struct _tap {
            F f;
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) const& {
                f(x);
                return lift<M>(detail::std::forward<X>(x));
            }
#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) & {
                f(x);
                return lift<M>(detail::std::forward<X>(x));
            }
#endif
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) && {
                detail::std::move(f)(x);
                return lift<M>(detail::std::forward<X>(x));
            }
        };

        template <typename F>
        static constexpr decltype(auto) apply(F&& f) {
            return detail::create<_tap>{}(detail::std::forward<F>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct models<Monad(M)>
        : detail::std::integral_constant<bool,
            !is_default<flatten_impl<M>>{} ||
            !is_default<bind_impl<M>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Monad::ap_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct Monad::ap_impl {
        template <typename F, typename X>
        static constexpr decltype(auto) apply(F&& f, X&& x) {
            return hana::bind(
                detail::std::forward<F>(f),
                hana::partial(transform, detail::std::forward<X>(x))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
