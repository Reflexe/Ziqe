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

#define ALLOW_COPY(name) name(const name &) = default; name &operator= (const name &) = default;
#define ALLOW_MOVE(name) name(name &&)      = default; name &operator= (name &&)      = default;

#define ALLOW_COPY_AND_MOVE(name) ALLOW_COPY(name) ALLOW_MOVE(name)

#define DISALLOW_COPY(name) name(const name &) = delete; name &operator= (const name &) = delete;
#define DISALLOW_MOVE(name) name(name &&)      = delete; name & operator= (name &&)     = delete;

#define DISALLOW_COPY_AND_MOVE(name) DISALLOW_COPY(name) DISALLOW_MOVE(name)

#define DEFINE_EQUAL_BY_MEMBER(type, name) bool operator ==(const type &other) const{ return name == other.name; }
#define DEFINE_NOT_EQUAL_BY_MEMBER(type, name) bool operator !=(const type &other) const{ return name != other.name; }

#define DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER(type, name) DEFINE_EQUAL_BY_MEMBER(type, name) DEFINE_NOT_EQUAL_BY_MEMBER(type, name)

namespace Ziqe {

template<class T>
const T &max(const T &one, const T &two)
{
    return (one > two) ? one : two;
}

template<class T>
const T &min(const T &one, const T &two)
{
    return (one < two) ? one : two;
}

template<class T>
T &max(T &one, T &two)
{
    return (one > two) ? one : two;
}

template<class T>
T &min(T &one, T &two)
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
#define MakeStaticVariable(expr) StaticVariable<decltype(expr), (expr)>()
template<class T, T sValue>
class StaticVariable
{
};

/**
  * Another way to declare and use template variables: but as a template parameters.
  *
  * Use it when you can use the template parameter (everywhere except template constructors).
  */
#define MakeTemplateVariable(expr) decltype(expr), (expr)
#define TemplateVariable(name) class _T##name, _T##name name


}



#endif // MACROS_H
