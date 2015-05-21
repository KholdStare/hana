/*!
@file
Defines the `boost::hana::Tuple` data type.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SANDBOX_LAMBDA_TUPLE_HPP
#define BOOST_HANA_SANDBOX_LAMBDA_TUPLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/variadic/at.hpp>
#include <boost/hana/detail/variadic/drop_into.hpp>
#include <boost/hana/detail/variadic/take.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/sequence.hpp>


namespace boost { namespace hana { namespace sandbox {
    struct LambdaTuple {
        struct hana {
            struct operators
                : boost::hana::operators::of<
                    Comparable, Monad, Iterable
                >
            { };
        };
    };

    template <typename Storage>
    struct _lambda_tuple
        : operators::adl,
          operators::Iterable_ops<_lambda_tuple<Storage>>
    {
        explicit constexpr _lambda_tuple(Storage&& s)
            : storage(boost::hana::detail::std::move(s))
        { }

        struct hana { using datatype = LambdaTuple; };
        Storage storage;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto lambda_tuple = [](auto ...xs) -> decltype(auto) {
        auto storage = [=](auto f) -> decltype(auto) { return f(xs...); };
        return _lambda_tuple<decltype(storage)>{hana::detail::std::move(storage)};
    };
}}} // end namespace boost::hana::sandbox

namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return static_cast<Xs&&>(xs)
                    .storage(static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F f) {
            return static_cast<Xs&&>(xs).storage(
                [f(detail::std::move(f))](auto&& ...xs) -> decltype(auto) {
                    return sandbox::lambda_tuple(f(static_cast<decltype(xs)&&>(xs))...);
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<sandbox::LambdaTuple>
        : Iterable::find_if_impl<sandbox::LambdaTuple>
    { };

    template <>
    struct any_of_impl<sandbox::LambdaTuple>
        : Iterable::any_of_impl<sandbox::LambdaTuple>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<sandbox::LambdaTuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                [](auto&& x, auto&& ...rest) -> decltype(auto) {
                    return id(static_cast<decltype(x)&&>(x));
                }
            );
        }
    };

    template <>
    struct tail_impl<sandbox::LambdaTuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                [](auto&& x, auto&& ...rest) -> decltype(auto) {
                    return sandbox::lambda_tuple(static_cast<decltype(rest)&&>(rest)...);
                }
            );
        }
    };

    template <>
    struct is_empty_impl<sandbox::LambdaTuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                [](auto const& ...xs) -> decltype(auto) {
                    return _bool<sizeof...(xs) == 0>{};
                }
            );
        }
    };

    template <>
    struct at_impl<sandbox::LambdaTuple> {
        template <typename Index, typename Xs>
        static constexpr decltype(auto) apply(Index n, Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                detail::variadic::at<value(n)>
            );
        }
    };

    template <>
    struct drop_at_most_impl<sandbox::LambdaTuple> {
        template <typename Index, typename Xs>
        static constexpr decltype(auto) apply(Index n, Xs&& xs) {
            auto m = min(n, length(xs));
            return static_cast<Xs&&>(xs).storage(
                detail::variadic::drop_into<value(m)>(sandbox::lambda_tuple)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct concat_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return static_cast<Xs&&>(xs).storage(
                [ys(static_cast<Ys&&>(ys))](auto&& ...xs) -> decltype(auto) {
                    return detail::std::move(ys).storage(
                        // We can't initialize the capture with perfect
                        // forwarding since that's not supported by the
                        // language.
                        [=](auto&& ...ys) -> decltype(auto) {
                            return sandbox::lambda_tuple(
                                detail::std::move(xs)...,
                                static_cast<decltype(ys)&&>(ys)...
                            );
                        }
                    );
                }
            );
        }
    };

    template <>
    struct prepend_impl<sandbox::LambdaTuple> {
        template <typename X, typename Xs>
        static constexpr decltype(auto) apply(X&& x, Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                [x(static_cast<X&&>(x))](auto&& ...xs) -> decltype(auto) {
                    return sandbox::lambda_tuple(
                        detail::std::move(x),
                        static_cast<decltype(xs)&&>(xs)...
                    );
                }
            );
        }
    };

    template <>
    struct append_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return static_cast<Xs&&>(xs).storage(
                [x(static_cast<X&&>(x))](auto&& ...xs) -> decltype(auto) {
                    return sandbox::lambda_tuple(
                        static_cast<decltype(xs)&&>(xs)...,
                        detail::std::move(x)
                    );
                }
            );
        }
    };

    template <>
    struct empty_impl<sandbox::LambdaTuple> {
        static BOOST_HANA_CONSTEXPR_LAMBDA decltype(auto) apply() {
            return sandbox::lambda_tuple();
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, sandbox::LambdaTuple>
        : _integral_constant<bool, true>
    { };

    template <>
    struct init_impl<sandbox::LambdaTuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return unpack(range(_size_t<0>{}, pred(length(xs))),
                on(sandbox::lambda_tuple, [&xs](auto index) -> decltype(auto) {
                    return at(index, static_cast<Xs&&>(xs));
                })
            );
        }
    };

    template <>
    struct take_at_most_impl<sandbox::LambdaTuple> {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N n, Xs&& xs) {
            auto m = min(n, length(xs));
            return static_cast<Xs&&>(xs).storage(
                detail::variadic::take<value(m)>
            )(sandbox::lambda_tuple);
        }
    };

    template <>
    struct zip_shortest_with_impl<sandbox::LambdaTuple> {
        template <typename F, typename ...Xss>
        static constexpr auto apply(F f, Xss ...tuples) {
            auto go = [=](auto index, auto ...nothing) {
                return always(f)(nothing...)(at(index, tuples)...);
            };
            auto zip_length = minimum(sandbox::lambda_tuple(length(tuples)...));
            return unpack(range(_size_t<0>{}, zip_length),
                on(sandbox::lambda_tuple, go)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<sandbox::LambdaTuple> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return sandbox::lambda_tuple(static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SANDBOX_LAMBDA_TUPLE_HPP
