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

#include "ZiqeAPI/Macros.h"

#include "Base/Types.hpp"

#include <utility>

#define implements

#define ZQ_ALLOW_COPY(name) name(const name &) = default; name &operator= (const name &) = default;
#define ZQ_ALLOW_MOVE(name) name(name &&)      = default; name &operator= (name &&)      = default;

#define ZQ_ALLOW_COPY_AND_MOVE(name) ZQ_ALLOW_COPY(name) ZQ_ALLOW_MOVE(name)

#define ZQ_ALLOW_MAKE_UNIQUE() template<class T, class ...Args> friend ::Ziqe::Base::UniquePointer<T> makeUnique(Args&&...)

#define ZQ_DISALLOW_COPY(name) name(const name &) = delete; name &operator= (const name &) = delete;
#define ZQ_DISALLOW_MOVE(name) name(name &&)      = delete; name & operator= (name &&)     = delete;

#define ZQ_DISALLOW_COPY_AND_MOVE(name) ZQ_DISALLOW_COPY(name) ZQ_DISALLOW_MOVE(name)

#define ZQ_DEFINE_EQUAL_BY_MEMBER(type, name) bool operator ==(const type &other) const{ return name == other.name; }
#define ZQ_DEFINE_NOT_EQUAL_BY_MEMBER(type, name) bool operator !=(const type &other) const{ return name != other.name; }

#define ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER(type, name) ZQ_DEFINE_EQUAL_BY_MEMBER(type, name) ZQ_DEFINE_NOT_EQUAL_BY_MEMBER(type, name)

#define ZQ_ARG(...) __VA_ARGS__
#define ZQ_DEFINE_CONST_AND_NON_CONST(ReturnValueConst, ReturnValueNonConst, name, args, ...) ReturnValueConst name args const __VA_ARGS__ \
    ReturnValueNonConst name args __VA_ARGS__

namespace Ziqe {
namespace Base {

/**
  @brief Safely ignore variables without compiler warnings.
 */
template<class... Args>
constexpr void IgnoreUnused (Args...)
{
}

template<class T>
constexpr const T &max(const T &one, const T &two)
{
    return (one > two) ? one : two;
}

template<class T>
constexpr const T &min(const T &one, const T &two)
{
    return (one < two) ? one : two;
}

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
 * that we want to get a temlate variable as a parameter.
 *
 * Example:
 * \code
 * template<int n>
 * void printN()
 * {
 *     std::cout << n;
 * }
 *
 * // Not really a parameter,
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

//template<class ...Args>
//constexpr bool _ExpandArgs (Args...)
//{
//    return false;
//}
//#define ExpandArgs(expr) _ExpandArgs((expr, 0)...)

// Do you have a better way?
//#define ExpandArgs(args) for (args...; false; )

template<typename F, typename... Args>
void pack_foreach(F f, Args&&... args) {
    int unpack[] = {(f(std::forward<Args>(args)), 0)..., 0};

    ZQ_UNUSED (unpack);
}

template<class ResultType, class Arg, class...Args>
ResultType plusArgs (const Arg &arg, const Args&&... args)
{
    return arg + plusArgs<ResultType>(std::forward<Args>(args)...);
}

template<bool value>
struct EnableIf
{
};
template<>
struct EnableIf<true>
{
    typedef bool type;
};

/**
 * @brief A little helper to copy a lvalue and return the copy as a prvalue.
 */
template<class T>
T copy (const T &value)
{
    return value;
}

using std::move;
using std::forward;

/// @brief An generic IsEqual.
template<class T>
struct IsEqual
{
    bool operator () (const T &one,
                      const T &other)
    {
        return !(one != other);
    }
};

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

}
}

#endif // MACROS_H
