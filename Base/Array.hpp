/**
 * @file Array.hpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2017 Shmuel Hazan
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
#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "Base/Macros.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

template<class _T, SizeType _sSize>
class Array
{
public:
    using Pointer = _T*;
    using Element = _T;
    using Iterator = Pointer;
    using ConstIterator = const Pointer;

    ZQ_DEFINE_CONST_AND_NON_CONST (ConstIterator, Iterator, begin, (), {
        return &mArray[0];
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (ConstIterator, Iterator, end, (), {
        return &mArray[_sSize];
    })

    ConstIterator cbegin() const
    {
        return begin();
    }

    ConstIterator cend() const
    {
        return end();
    }

    SizeType size() const
    {
        return _sSize;
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (const _T&, _T&, operator[], (SizeType index), {
        return mArray[index];
    })


//    bool operator != (const Array &other) const{
//        if (other.size () != size ())
//            return true;


//    }

//private:
    _T mArray[_sSize];
};

template<class FirstT, class... Args>
decltype(auto) makeArray(FirstT&& value, Args&&... values)
{
    return Array<FirstT, 1+sizeof...(Args)>{Base::forward<FirstT>(value),
                                            Base::forward<Args>(values)...};
}

} // namespace Base
ZQ_END_NAMESPACE

#endif // ARRAY_HPP
