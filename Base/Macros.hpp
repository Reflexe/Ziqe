/**
 * @file Macros
 * @author shrek0 (shrek0.tk@gmail.com)
 *
 * Ziqe: copyright (C) 2016 shrek0
 *
 * Ziqe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ziqe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MACROS_H
#define MACROS_H

#include "Platforms/Macros.h"

#include "Base/Types.hpp"

#define implements

#define ZQ_ALLOW_COPY(name) name(const name &) = default; name &operator= (const name &) = default;
#define ZQ_ALLOW_MOVE(name) name(name &&)      = default; name &operator= (name &&)      = default;

#define ZQ_ALLOW_COPY_AND_MOVE(name) ZQ_ALLOW_COPY(name) ZQ_ALLOW_MOVE(name)

#define ZQ_ALLOW_MAKE_UNIQUE() template<class T, class ...Args> friend ::Ziqe::Base::UniquePointer<T> makeUnique(Args&&...)
#define ZQ_ALLOW_EXPECTED() template<class A, class B> friend class ::Ziqe::Base::Expected

#define ZQ_DISALLOW_COPY(name) name(const name &) = delete; name &operator= (const name &) = delete;
#define ZQ_DISALLOW_MOVE(name) name(name &&)      = delete; name & operator= (name &&)     = delete;

#define ZQ_DISALLOW_COPY_AND_MOVE(name) ZQ_DISALLOW_COPY(name) ZQ_DISALLOW_MOVE(name)

#define ZQ_DEFINE_EQUAL_BY_MEMBER(type, name) bool operator ==(const type &other) const{ return name == other.name; }
#define ZQ_DEFINE_NOT_EQUAL_BY_MEMBER(type, name) bool operator !=(const type &other) const{ return name != other.name; }

#define ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER(type, name) ZQ_DEFINE_EQUAL_BY_MEMBER(type, name) ZQ_DEFINE_NOT_EQUAL_BY_MEMBER(type, name)

#define ZQ_ARG(...) __VA_ARGS__
#define ZQ_DEFINE_CONST_AND_NON_CONST(ReturnValueConst, ReturnValueNonConst, name, args, ...) ReturnValueConst name args const __VA_ARGS__ \
    ReturnValueNonConst name args __VA_ARGS__

ZQ_BEGIN_NAMESPACE
namespace Base {

/**
  @brief Safely ignore variables without compiler warnings.
 */
template<class... Args>
constexpr void IgnoreUnused (Args...)
{
}

//template<class T>
//constexpr const T &max(const T &one, const T &two)
//{
//    return (one > two) ? one : two;
//}

//template<class T>
//constexpr const T &min(const T &one, const T &two)
//{
//    return (one < two) ? one : two;
//}

template<class T>
constexpr T &max(T &one, T &two)
{
    return (one > two) ? one : two;
}

template<class T>
constexpr T &min(T &one, T &two)
{
    return (one < two) ? one : two;
}

/**
 * An holder for a build time known value. Used in places
 * that we want to get a template variable as a parameter.
 *
 * Example:
 * \code
 * template<int n>
 * void printN(StaticVariable<int, n>)
 * {
 *     std::cout << n;
 * }
 *
 * printN(MakeStaticVariable(6));
 * \endcode
 */
#define ZQ_MakeStaticVariable(expr) StaticVariable<decltype(expr), (expr)>()


template<class T, T sValue>
class StaticVariable
{
};

/**
  * Another way to declare and use template variables: but as a template parameters.
  *
  * Use it when you can use the template parameter (everywhere except template constructors).
  */
#define ZQ_MakeTemplateVariable(expr) decltype(expr), (expr)
#define ZQ_TemplateVariable(name) class _T##name, _T##name name

/**
 * @brief An sizeof for parameter pack (NOT sizeof...).
 * \code
 * SizeType argsSize = 0;
 * for (arg in args)
 *     argsSize += sizeof (arg);
 * return argsSize;
 * \endcode
 */
template<class T, class ...Args>
constexpr SizeType sizeOfArgs (const T &,
                               const Args &...args)
{
    return sizeof (T) + sizeOfArgs (args...);
}

constexpr SizeType sizeOfArgs ()
{
    return 0;
}

template<bool value, class T=void>
struct _EnableIf
{
};

template<class T>
struct _EnableIf<true, T>
{
    typedef T type;
};

template<bool value, class T=void>
using EnableIf=_EnableIf<value, T>;

/**
  @brief std::is_same
 */
template<class T1, class T2>
struct IsSame
{
    static constexpr bool value = false;
};

template<class T>
struct IsSame<T, T>
{
    static constexpr bool value = true;
};

// Would fail if the first argument is invalid (SFINAE)
template<class T>
inline_hint constexpr bool EnableIfExists (const T &)
{
    return true;
}

/**
  @brief Remove `const` identifer from a type.
 */
template<class T>
struct _RemoveConst
{
    typedef T type;
};

template<class T>
struct _RemoveConst<const T>
{
    typedef T type;
};

template<class T>
using RemoveConst=_RemoveConst<T>;

/**
 * @brief A little helper to copy a lvalue and return the copy as a prvalue.
 */
template<class T>
inline_hint T copy (const T &value)
{
    return value;
}

/**
  @brief Remove Reference from a type.
 */
template<class T> struct remove_reference      { typedef T type; };
template<class T> struct remove_reference<T&>  { typedef T type;};
template<class T> struct remove_reference<T&&> { typedef T type; };

/**
  @brief Return xvalue from everything.
 */
template<class T>
inline_hint
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(t);
}

/**
  @brief std::forward   Perfect forwarding.
 */
template<class T>
inline_hint
constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template<class T>
inline_hint constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
{
    return static_cast<T&&>(t);
}

/**
  @brief Return an invalid (not-dereferencable) T.
 */
template<class T>
inline_hint constexpr T declval ()
{
    using NoReferenceT=typename Base::remove_reference<T>::type;
    using PVoidType=void*;

    // Remove references from T, convert nullptr to a pointer to the result.
    // and then convert it again to T.
    return static_cast<T>(*static_cast<NoReferenceT*>(PVoidType()));
}

/**
  @brief A generic IsEqual with the () operator, uses @tparam T 's != operator.
 */
template<class T>
struct IsEqual
{
    bool operator () (const T &one,
                             const T &other)
    {
        return !(one != other);
    }
};

/**
  @brief A generic dereference IsEqual, uses @tparam T 's * operator for dereference
         and @tparam T 's != operator.
 */
template<class T>
struct DerefrenceIsEqual
{
    bool operator () (const T &one,
                      const T &other)
    {
        return !(*one != *other);
    }
};

/// @brief An generic IsEqual.
template<class T>
struct IsLessThan
{
    bool operator () (const T &one,
                      const T &other)
    {
        return (one < other);
    }
};

template<class T>
struct DerefrenceIsLessThan
{
    bool operator () (const T &one,
                      const T &other)
    {
        return (*one < *other);
    }
};

template<class T1, class T2>
struct Pair
{
    T1 first;
    T2 second;
};

template<class T1, class T2, class T3>
struct Triple
{
    T1 first;
    T2 second;
    T3 third;
};

template<class T1, class T2, class T3>
Triple<T1, T2, T3> makeTriple (T1&& first, T2&& second, T3&& third)
{
    return Triple<T1, T2, T3>{Base::forward<T1>(first), Base::forward<T2>(second), Base::forward<T3>(third)};
}

template<typename F, typename... Args>
void pack_foreach(F f, Args&&... args) {
    int unpack[] = {(f(Base::forward<Args>(args)), 0)..., 0};

    IgnoreUnused (unpack);
}

template<class ResultType, class Arg, class...Args>
ResultType plusArgs (const Arg &arg, const Args&&... args)
{
    return arg + plusArgs<ResultType>(Base::forward<Args>(args)...);
}

template<class T>
inline_hint void swap (T &first, T &second) {
    T temp{move (first)};

    first = move (second);
    second = move (temp);
}

// Check for .swap member.
template<class T,
         bool = EnableIfExists(declval<T&>().swap (declval<T&>()))>
inline_hint void swap (T &first, T &second)
{
    first.swap (second);
}

} // namespace Base
ZQ_END_NAMESPACE

#endif // MACROS_H
