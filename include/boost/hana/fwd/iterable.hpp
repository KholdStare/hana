/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ITERABLE_HPP
#define BOOST_HANA_FWD_ITERABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/static_constexpr.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    namespace operators {
        template <typename Derived>
        struct Iterable_ops;
    }

    //! @ingroup group-concepts
    //! The `Iterable` concept represents data structures supporting external
    //! iteration.
    //!
    //! Intuitively, an `Iterable` can be seen as a kind of container whose
    //! elements can be pulled out one at a time. An `Iterable` also provides
    //! a way to know when the _container_ is empty, i.e. when there are no
    //! more elements to pull out.
    //!
    //! Whereas `Foldable` represents data structures supporting internal
    //! iteration with the ability to accumulate a result, the `Iterable`
    //! concept allows inverting the control of the iteration. This is more
    //! flexible than `Foldable`, since it allows iterating over only some
    //! part of the structure. This, in turn, allows `Iterable` to work on
    //! infinite structures, while trying to fold such a structure would
    //! never finish.
    //!
    //!
    //! @anchor Iterable-lin
    //! The linearization of an `Iterable`
    //! ----------------------------------
    //! Intuitively, for an `Iterable` structure `xs`, the _linearization_ of
    //! `xs` is the sequence of all the elements in `xs` as if they had been
    //! put in a (possibly infinite) list:
    //! @code
    //!     linearization(xs) = [x1, x2, x3, ...]
    //! @endcode
    //!
    //! This notion is precisely the extension of the [linearization]
    //! (@ref Foldable-lin) notion of `Foldable`s to the infinite case.
    //! This notion is useful for expressing various properties of
    //! `Iterable`s, and is used for that throughout the documentation.
    //!
    //!
    //! Compile-time `Iterable`s
    //! ------------------------
    //! A _compile-time_ `Iterable` is an `Iterable` for which `is_empty`
    //! returns a compile-time `Logical`. These structures allow iteration
    //! to be done at compile-time, in the sense that the "loop" doing the
    //! iteration can be unrolled because the total length of the structure
    //! is kown at compile-time.
    //!
    //! In particular, note that being a compile-time `Iterable` has nothing
    //! to do with being finite or infinite. For example, it would be possible
    //! to create a sequence representing the Pythagorean triples as
    //! `IntegralConstant`s. Such a sequence would be infinite, but iteration
    //! on the sequence would still be done at compile-time. However, if one
    //! tried to iterate over _all_ the elements of the sequence, the compiler
    //! would loop indefinitely, in contrast to your program looping
    //! indefinitely if the sequence was a runtime one.
    //!
    //! __In the current version of the library, only compile-time `Iterable`s
    //! are supported.__ While it would be possible in theory to support
    //! runtime `Iterable`s, doing it efficiently is the subject of some
    //! research. In particular, follow [this issue][1] for the current
    //! status of runtime `Iterable`s.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `head`, `tail` and `is_empty`
    //!
    //!
    //! Superclasses
    //! ------------
    //! 1. `Searchable`\n
    //! Any `Iterable` gives rise to a model of `Searchable`, where the keys
    //! and the values are both the elements in the structure. Searching for
    //! a key is just doing a linear search through the elements of the
    //! structure.
    //! @snippet example/iterable.cpp Searchable
    //!
    //! 2. `Foldable` for finite `Iterable`s (free model)\n
    //! Every finite `Iterable` gives rise to a model of  `Foldable`. Hence,
    //! finite `Iterable`s must satisfy additional laws to make sure that
    //! external iteration in `Iterable` and internal iteration in `Foldable`
    //! are consistent. These laws are expressed in terms of the `Foldable`'s
    //! [linearization](@ref Foldable-lin). For any finite `Iterable` `xs`
    //! with linearization `[x1, ..., xn]`, the following must be satisfied:
    //! @code
    //!     at(i, xs) == xi
    //!     is_empty(xs) <=> n == 0
    //! @endcode
    //! An equivalent way of writing this is
    //! @code
    //!     head(xs) == head(linearization(xs))
    //!              == x1
    //!
    //!     linearization(tail(xs)) == tail(linearization(xs))
    //!                             == [x2, ..., xn]
    //!
    //!     is_empty(xs)  <=>  is_empty(linearization(xs))
    //!                   <=>  n == 0
    //! @endcode
    //! This says that linearizing an `Iterable` and then iterating through
    //! it should be equivalent to just iterating through it.
    //! @note
    //! As explained above, `Iterable`s are also `Searchable`s and their
    //! models have to be consistent. By the laws presented here, it also
    //! means that the `Foldable` model for finite `Iterable`s has to be
    //! consistent with the `Searchable` model.
    //!
    //! For convenience, a default minimal complete definition for `Foldable`
    //! is provided for finite `Iterable`s via the `Iterable::fold_right_impl`
    //! and `Iterable::fold_left_impl` methods. The provided model is simple
    //! and intuitive; here is how it works:\n
    //! Let `xs` be an `Iterable` and let `xi` denote the `i`-th element in
    //! its linearization. In other words, `xs` can be linearized as
    //! `[x1, ..., xN]`, where `N` is the (finite) number of elements in `xs`.
    //! Then, right-folding `xs` with a binary operation `*` (in infix
    //! notation for legibility) is equivalent to
    //! @code
    //!     x1 * (x2 * ( ... * (xN-1 * xN)))
    //! @endcode
    //! Similarly, left-folding `xs` is equivalent to
    //! @code
    //!     (((x1 * x2) * x3) * ...) * xN
    //! @endcode
    //! In both cases, notice the side of the parentheses. Left-folding
    //! applies `*` in a left-associative manner, whereas right-folding
    //! applies it in a right-associative manner. For associative operations,
    //! i.e. operations such that for all `a`, `b` and `c`,
    //! @code
    //!     (a * b) * c = a * (b * c)
    //! @endcode
    //! this makes no difference. Also note that folds with an initial state
    //! are implemented in an analogous way, and they are provided as
    //! `Iterable::fold_{left,right}_nostate_impl`.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Tuple`, `String`, `Range`
    //!
    //!
    //! Laws
    //! ----
    //! First, we require the equality of two `Iterable`s to be related to the
    //! equality of the elements in their linearizations. More specifically,
    //! if `xs` and `ys` are two non-empty `Iterable`s of data type `It`, then
    //! @code
    //!     xs == ys  =>  head(xs) == head(ys) && tail(xs) == tail(ys)
    //! @endcode
    //! which conveys that two `Iterable`s must have the same linearization
    //! in order to have the chance of being considered equal. We then handle
    //! the empty case by saying that if any of `xs` and `ys` is empty, then
    //! @code
    //!     xs == ys  =>  is_empty(xs) && is_empty(ys)
    //! @endcode
    //!
    //! Second, since every `Iterable` is also a `Searchable`, we require the
    //! models of `Iterable` and `Searchable` to be consistent. This is made
    //! precise by the following laws. For any `Iterable` `xs` with a
    //! linearization of `[x1, x2, x3, ...]`,
    //! @code
    //!     any_of(xs, equal.to(z))  <=>  xi == z
    //! @endcode
    //! for some finite index `i`. Furthermore,
    //! @code
    //!     find_if(xs, pred) == just(the first xi such that pred(xi) is satisfied)
    //! @endcode
    //! or `nothing` if no such `xi` exists.
    //!
    //!
    //! [1]: https://github.com/ldionne/hana/issues/40
    struct Iterable {
        template <typename It> struct fold_left_impl;
        template <typename It> struct fold_right_impl;
        template <typename It> struct fold_left_nostate_impl;
        template <typename It> struct fold_right_nostate_impl;

        template <typename It> struct find_if_impl;
        template <typename It> struct any_of_impl;
    };

    //! Returns the first element of a non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `head(xs)` is equivalent to `x1`. If `xs` is empty, it is an error to
    //! use `head`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp head
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto head = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct head_impl;

    struct _head {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using It = typename datatype<Xs>::type;
            using Head = head_impl<It>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, It>{},
            "hana::head(xs) requires xs to be an Iterable");
        #endif

            return Head::apply(static_cast<Xs&&>(xs));
        }
    };

    namespace {
        constexpr auto const& head = detail::static_constexpr<_head>;
    }
#endif

    //! Returns a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `tail(xs)` is an Iterable of the same data type whose linearization is
    //! `[x2, ..., xN]`. In particular, `tail(xs)` is functionally equivalent
    //! to `drop(size_t<1>, xs)`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp tail
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto tail = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct tail_impl;

    struct _tail {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::tail(xs) requires xs to be an Iterable");
#endif
            return tail_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    namespace {
        constexpr auto const& tail = detail::static_constexpr<_tail>;
    }
#endif

    //! Returns whether the iterable is empty.
    //! @relates Iterable
    //!
    //! `is_empty` must return a compile-time `Logical` representing
    //! whether the iterable is empty.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp is_empty
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_empty = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct is_empty_impl;

    struct _is_empty {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::is_empty(xs) requires xs to be an Iterable");
#endif

            return is_empty_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    namespace {
        constexpr auto const& is_empty = detail::static_constexpr<_is_empty>;
    }
#endif

    //! Returns the `n`th element of an iterable.
    //! @relates Iterable
    //!
    //! Given a `Constant` index and an iterable, `at` returns the
    //! element located at the index in the linearization of the iterable.
    //! Specifically, given an iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `at(k, xs)` is equivalent to `xk`.
    //!
    //!
    //! @param n
    //! A (non-negative) `Constant` of an unsigned integral type representing
    //! the 0-based index of the element to return. It is an error to call
    //! `at` with an index that is either out of bounds for the iterable,
    //! not of an unsigned type or not a Constant.
    //!
    //! @param iterable
    //! The iterable in which an element is retrieved. The iterable must
    //! contain at least `n + 1` elements.
    //!
    //!
    //! Operator-form
    //! -------------
    //! For convenience, the `at` method can be applied to `Iterable`s
    //! that support it by using the `[]` operator. Hence, if `xs`
    //! supports the operator,
    //! @code
    //!     xs[n] == at(n, xs)
    //! @endcode
    //!
    //! To take advantage of this operator for a type `T`, `T` must inherit
    //! `hana::operators::Iterable_ops<T>`.
    //!
    //! @note
    //! The same operator is provided for the `find` method of the `Searchable`
    //! concept. When a data type is a model of both `Iterable` and
    //! `Searchable`, which operator is used should be documented properly.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp at
    //!
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.at.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto at = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct at_impl;

    struct _at {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::at(n, xs) requires xs to be an Iterable");
#endif
            return at_impl<typename datatype<Xs>::type>::apply(
                static_cast<N&&>(n),
                static_cast<Xs&&>(xs)
            );
        }
    };

    namespace {
        constexpr auto const& at = detail::static_constexpr<_at>;
    }
#endif

    //! Equivalent to `at`; provided for convenience.
    //! @relates Iterable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto at_c = [](auto&& iterable) -> decltype(auto) {
        return at(size_t<n>, forwarded(iterable));
    };
#else
    template <detail::std::size_t n>
    struct _at_c;

    namespace {
        template <detail::std::size_t n>
        constexpr auto const& at_c = detail::static_constexpr<_at_c<n>>;
    }
#endif

    //! Returns the last element of a non-empty and finite iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty and finite iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `last(xs)` is equivalent to `xN`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp last
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto last = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct last_impl;

    struct _last {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::last(xs) requires xs to be an Iterable");
#endif
            return last_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    namespace {
        constexpr auto const& last = detail::static_constexpr<_last>;
    }
#endif

    //! Drops the first `n` elements of an iterable and returns the rest.
    //! @relates Iterable
    //!
    //! Given an (non-negative) `Constant` `n` of an unsigned integral type
    //! and an iterable `xs` with a linearization of `[x1, x2, ...]`,
    //! `drop(n, xs)` is an iterable of the same data type whose
    //! linearization is `[xn+1, xn+2, ...]`. In particular, note that
    //! this method does not mutate the original iterable in any way.
    //!
    //! There are two different ways of calling `drop`, which correspond to
    //! different policies in case the length of the iterable is less than `n`:
    //! @code
    //!     drop(n, xs)         = drop.at_most(n, xs)
    //!     drop.at_most(n, xs) = see below
    //!     drop.exactly(n, xs) = see below
    //! @endcode
    //!
    //! In case `length(xs) < n`, the `drop.at_most` variant will simply drop
    //! the whole iterable, without failing. In contrast, the `drop.exactly`
    //! variant assumes that `length(xs) >= n`, which makes it possible to
    //! perform some optimizations.
    //!
    //! Both variants are tag-dispatched methods that can be overriden. Here
    //! is how each variant is tag-dispatched:
    //! @code
    //!     drop.at_most  ->  drop_at_most_impl
    //!     drop.exactly  ->  drop_exactly_impl
    //! @endcode
    //! `drop` is not tag dispatched, because it is just an alias to
    //! `drop.at_most`.
    //!
    //! @param n
    //! A non-negative `Constant` of an unsigned integral type representing
    //! the number of elements to be dropped from the iterable.
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct drop_exactly_impl;

    template <typename It, typename = void>
    struct drop_at_most_impl;

    struct _drop_exactly {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            using It = typename datatype<Xs>::type;
            using DropExactly = drop_exactly_impl<It>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, It>{},
            "hana::drop.exactly(n, xs) requires xs to be an Iterable");
        #endif

            return DropExactly::apply(static_cast<N&&>(n), static_cast<Xs&&>(xs));
        }
    };

    struct _drop_at_most {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            using It = typename datatype<Xs>::type;
            using DropAtMost = drop_at_most_impl<It>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, It>{},
            "hana::drop.at_most(n, xs) requires xs to be an Iterable");
        #endif

            return DropAtMost::apply(static_cast<N&&>(n), static_cast<Xs&&>(xs));
        }
    };

    template <typename ...>
    struct _drop : _drop_at_most {
        static constexpr auto const& exactly = detail::static_constexpr<_drop_exactly>;
        static constexpr auto const& at_most = detail::static_constexpr<_drop_at_most>;
    };
    template <typename ...Dummy>
    constexpr _drop_exactly const& _drop<Dummy...>::exactly;
    template <typename ...Dummy>
    constexpr _drop_at_most const& _drop<Dummy...>::at_most;

    namespace {
        constexpr auto const& drop = detail::static_constexpr<_drop<>>;
    }
#endif

    //! Equivalent to `drop`; provided for convenience.
    //! @relates Iterable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto drop_c = [](auto&& iterable) -> decltype(auto) {
        return drop(size_t<n>, forwarded(iterable));
    };
#else
    template <detail::std::size_t n>
    struct _drop_c;

    namespace {
        template <detail::std::size_t n>
        constexpr auto const& drop_c = detail::static_constexpr<_drop_c<n>>;
    }
#endif

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is not satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_while` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is not satisfied.
    //! If the iterable is not finite, the `predicate` has to return a false-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` should
    //! be dropped from the structure. In the current version of the library,
    //! `predicate` should return a compile-time `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_while
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_while = [](auto&& iterable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct drop_while_impl;

    struct _drop_while {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::drop_while(xs, pred) requires xs to be an Iterable");
#endif
            return drop_while_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    namespace {
        constexpr auto const& drop_while = detail::static_constexpr<_drop_while>;
    }
#endif

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_until` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is satisfied.
    //! If the iterable is not finite, the `predicate` has to return a true-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //! @note
    //! This is effectively equivalent to `drop_while` with a negated
    //! `predicate`.
    //!
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` and
    //! subsequent elements should be kept in the structure. In the current
    //! version of the library, `predicate` should return a compile-time
    //! `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_until
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_until = [](auto&& iterable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct drop_until_impl;

    struct _drop_until {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::drop_until(xs, pred) requires xs to be an Iterable");
#endif
            return drop_until_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    namespace {
        constexpr auto const& drop_until = detail::static_constexpr<_drop_until>;
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ITERABLE_HPP
