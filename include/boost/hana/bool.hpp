/*!
@file
Defines `boost::hana::IntegralConstant` and includes `Logical` and
`Comparable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOL_HPP
#define BOOST_HANA_BOOL_HPP

#include <boost/hana/fwd/bool.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/eval_base.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/detail/std/size_t.hpp>

// provided models; the rest is included in <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/comparable.hpp>

// operators
#include <boost/hana/fwd/group.hpp>
#include <boost/hana/fwd/integral_domain.hpp>
#include <boost/hana/fwd/monoid.hpp>
#include <boost/hana/fwd/orderable.hpp>
#include <boost/hana/fwd/ring.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // integral_constant
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    namespace ic_detail {
        template <typename T, T N, typename = detail::std::make_integer_sequence<T, N>>
        struct go;

        template <typename T, T N, T ...i>
        struct go<T, N, detail::std::integer_sequence<T, i...>> {
            using swallow = T[];

            template <typename F>
            static constexpr void with_index(F&& f)
            { (void)swallow{T{}, ((void)f(_integral_constant<T, i>{}), i)...}; }

            template <typename F>
            static constexpr void without_index(F&& f)
            { (void)swallow{T{}, ((void)f(), i)...}; }
        };

        template <typename T, T v>
        template <typename F>
        constexpr void _with_index<T, v>::operator()(F&& f) const
        { go<T, ((void)sizeof(&f), v)>::with_index(static_cast<F&&>(f)); }

        template <typename T, T v>
        template <typename F>
        constexpr void _times<T, v>::operator()(F&& f) const
        { go<T, ((void)sizeof(&f), v)>::without_index(static_cast<F&&>(f)); }

        // avoid link-time error
        template <typename T, T v>
        constexpr _with_index<T, v> _times<T, v>::with_index;
    }

    // avoid link-time error
    template <typename T, T v>
    constexpr ic_detail::_times<T, v> _integral_constant<T, v>::times;

    template <typename T, T v>
    struct datatype<_integral_constant<T, v>> {
        using type = IntegralConstant<T>;
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators (most of them are provided by the concepts)
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename T>
        struct of<IntegralConstant<T>>
            : operators::of<
                  Comparable, Orderable
                , Logical
                , Monoid, Group, Ring,

                // We make the operators dependent to avoid instantiating
                // `operators::of<>` before it is specialized by concepts.
                // This is required because `bool.hpp` is sometimes included
                // before concepts are fully defined.
                detail::dependent_on_t<sizeof(T) == 1, IntegralDomain>
            >
        { };
    }

#define BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(op)                          \
    template <typename U, U u, typename V, V v>                             \
    constexpr _integral_constant<decltype(u op v), (u op v)>                \
    operator op(_integral_constant<U, u>, _integral_constant<V, v>)         \
    { return {}; }                                                          \
    /**/

#define BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP(op)                           \
    template <typename U, U u>                                              \
    constexpr _integral_constant<decltype(op u), (op u)>                    \
    operator op(_integral_constant<U, u>)                                   \
    { return {}; }                                                          \
    /**/

    // Arithmetic
    BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP(+)

    // Bitwise
    BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP(~)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(&)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(|)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(^)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(<<)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(>>)

#undef BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP
#undef BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP


    //////////////////////////////////////////////////////////////////////////
    // User-defined literal
    //////////////////////////////////////////////////////////////////////////
    namespace ic_detail {
        constexpr int to_int(char c)
        { return static_cast<int>(c) - 48; }

        template <detail::std::size_t N>
        constexpr long long parse(const char (&arr)[N]) {
            long long number = 0, base = 1;
            for (detail::std::size_t i = 0; i < N; ++i) {
                number += to_int(arr[N - 1 - i]) * base;
                base *= 10;
            }
            return number;
        }
    }

    namespace literals {
        template <char ...c>
        constexpr auto operator"" _c()
        { return _llong<ic_detail::parse<sizeof...(c)>({c...})>{}; }
    }

    //////////////////////////////////////////////////////////////////////////
    // Model of Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value_impl<IntegralConstant<T>> {
        template <typename C>
        static constexpr auto apply()
        { return C::value; }
    };

    template <typename T, typename C>
    struct to_impl<IntegralConstant<T>, C, when<
        _models<Constant, C>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        static_assert(detail::std::is_integral<T>{},
        "trying to convert a Constant to an IntegralConstant of a non-integral "
        "type; boost::hana::IntegralConstant may only hold integral types");

        template <typename X>
        static constexpr auto apply(X const&) {
            constexpr T v = hana::value<X>();
            return _integral_constant<T, v>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Optimizations
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct eval_if_impl<IntegralConstant<T>> {
        template <typename Cond, typename Then, typename Else>
        static constexpr decltype(auto)
        apply(Cond const&, Then&& t, Else&& e) {
            return eval_if_impl::apply(bool_<static_cast<bool>(Cond::value)>,
                    static_cast<Then&&>(t), static_cast<Else&&>(e));
        }

        template <typename Then, typename Else>
        static constexpr decltype(auto)
        apply(decltype(true_) const&, Then&& t, Else&&)
        { return hana::eval(static_cast<Then&&>(t)); }

        template <typename Then, typename Else>
        static constexpr decltype(auto)
        apply(decltype(false_) const&, Then&&, Else&& e)
        { return hana::eval(static_cast<Else&&>(e)); }
    };

    template <typename T>
    struct if_impl<IntegralConstant<T>> {
        template <typename Cond, typename Then, typename Else>
        static constexpr decltype(auto)
        apply(Cond const&, Then&& t, Else&& e) {
            return if_impl::apply(bool_<static_cast<bool>(Cond::value)>,
                    static_cast<Then&&>(t), static_cast<Else&&>(e));
        }

        //! @todo We could return `Then` instead of `auto` to sometimes save
        //! a copy, but that would break some code that would return a
        //! reference to a Type object. I think the code that would be broken
        //! should be changed, but more thought needs to be given.
        template <typename Then, typename Else>
        static constexpr auto
        apply(decltype(true_) const&, Then&& t, Else&&)
        { return static_cast<Then&&>(t); }

        template <typename Then, typename Else>
        static constexpr auto
        apply(decltype(false_) const&, Then&&, Else&& e)
        { return static_cast<Else&&>(e); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BOOL_HPP
