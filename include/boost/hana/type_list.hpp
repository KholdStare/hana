/*!
@file
Defines `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_HPP
#define BOOST_HANA_TYPE_LIST_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! `List` containing `Type`s only.
    //!
    //! @note
    //! `TypeList` may be more efficient than `List` because of
    //! optimizations. When possible, it should be preferred.
    //!
    //! @todo
    //! - Implement efficient membership testing.
    //! - Actually provide optimizations.
    struct TypeList { };

    namespace operators {
        template <typename ...xs>
        struct type_list {
            struct type {
                using hana_datatype = TypeList;

                template <template <typename ...> class f>
                using storage = f<xs...>;
            };
        };
    }

    //! Creates a list containing the given types.
    //! @relates TypeList
    //!
    //! This is functionally equivalent to `list(type<xs>...)`.
    //!
    //! ### Example
    //! @snippet example/type_list/type_list.cpp main
    template <typename ...xs>
    constexpr typename operators::type_list<xs...>::type type_list{};

    //! @details
    //! Generic instance for `Iterable`s.
    template <>
    struct Foldable::instance<TypeList> : Iterable::FoldableInstance {

    };

    //! @details
    //! The head of `type_list<x, xs...>` is `type<x>`, its tail is
    //! `type_list<xs...>` and a type list is empty if and only if
    //! it contains no types at all.
    template <>
    struct Iterable::instance<TypeList> : Iterable::mcd {
    private:
        template <typename x, typename ...xs>
        struct head_helper {
            static constexpr auto value = type<x>;
        };

        template <typename ...xs>
        struct is_empty_helper {
            static constexpr auto value = bool_<sizeof...(xs) == 0>;
        };

        template <typename x, typename ...xs>
        struct tail_helper {
            static constexpr auto value = type_list<xs...>;
        };

    public:
        template <typename Xs>
        static constexpr auto head_impl(Xs) {
            return Xs::template storage<head_helper>::value;
        }

        template <typename Xs>
        static constexpr auto tail_impl(Xs) {
            return Xs::template storage<tail_helper>::value;
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs) {
            return Xs::template storage<is_empty_helper>::value;
        }
    };

    //! @details
    //! `cons(type<x>, type_list<xs...>)` is equivalent to
    //! `type_list<x, xs...>` and `nil<TypeList>` is equivalent
    //! to `type_list<>`.
    template <>
    struct List::instance<TypeList> : List::mcd<TypeList> {
    private:
        template <typename ...xs>
        struct cons_helper {
            template <typename x>
            static constexpr auto value = type_list<x, xs...>;
        };

    public:
        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs) {
            return Xs::template storage<cons_helper>::template value<untype_t<X>>;
        }

        static constexpr auto nil_impl() {
            return type_list<>;
        }
    };

    template <>
    struct Comparable::instance<TypeList, TypeList> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs, Ys)
        { return equal(type<Xs>, type<Ys>); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_HPP