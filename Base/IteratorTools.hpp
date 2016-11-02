/**
 * @file IteratorTools.hpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2016 Shmuel Hazan
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
#ifndef ZIQE_ITERATORTOOLS_H
#define ZIQE_ITERATORTOOLS_H

#include "Types.hpp"
#include "Macros.hpp"

#include <utility>

namespace Ziqe {
namespace Base {

using Base::move;
using Base::forward;

template<class InputIterator>
class MoveIterator
{
    InputIterator mInput;

public:
    explicit MoveIterator(const InputIterator &iterator)
        : mInput{iterator}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (MoveIterator)

    decltype (Base::move (*mInput)) operator* ()
    {
        return Base::move(*mInput);
    }

    MoveIterator &operator++ () {
        ++mInput;
        return *this;
    }

    MoveIterator &operator-- () {
        --mInput;

        return *this;
    }

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (MoveIterator, mInput)
};

template <class InputIterator>
inline_hint MoveIterator<InputIterator> makeMoveIterator(const InputIterator &inputIterator)
{
    return MoveIterator<InputIterator>{inputIterator};
}

template<class _InputIterator, class _FunctionType>
class ActionIterator
{
public:
    typedef _InputIterator InputIterator;
    typedef _FunctionType FunctionType;
private:
    InputIterator mInput;
    FunctionType mFunction;

public:
    ActionIterator(const InputIterator &iterator, FunctionType &&function)
        : mInput{iterator}, mFunction{Base::move (function)}
    {
    }

    ActionIterator(const InputIterator &iterator, const FunctionType &function)
        : mInput{iterator}, mFunction{function}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (ActionIterator)

    decltype (mFunction (*mInput)) operator* ()
    {
        return mFunction (*mInput);
    }

    ActionIterator &operator++ () {
        ++mInput;
        return *this;
    }

    ActionIterator &operator-- () {
        --mInput;

        return *this;
    }

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (ActionIterator, mInput)
};

template <class InputIterator, class FunctionType>
inline_hint ActionIterator<InputIterator, FunctionType> makeActionIterator(const InputIterator &inputIterator, FunctionType &&function)
{
    return ActionIterator<InputIterator, FunctionType>{inputIterator, Base::move (function)};
}

template <class ActionIteratorType,
          class InputIterator=typename ActionIteratorType::InputIterator,
          class FunctionType =typename ActionIteratorType::FunctionType>
inline_hint ActionIterator<InputIterator, FunctionType> makeActionIteratorFromOther(const ActionIteratorType &, const InputIterator &inputIterator)
{
    return ActionIterator<InputIterator, FunctionType>{inputIterator};
}

// optimize it to use operator - when possible.
template<class InputIterator,
         typename = decltype (InputIterator::operator-())>
SizeType countRange (const InputIterator &begin, const InputIterator &end)
{
    return end-begin;
}

template<class InputIterator>
SizeType countRange (const InputIterator &begin, const InputIterator &end) {
    SizeType count = 0;

    for (auto iterator = begin; iterator != end; ++iterator)
    {
        ++count;
    }

    return count;
}

template<class InputIterator>
SizeType countRange (const Pair<InputIterator, InputIterator> &beginAndEnd)
{
    return countRange (beginAndEnd.first, beginAndEnd.second);
}

template<class InputIterator>
SizeType countRange (const Triple<InputIterator, InputIterator, SizeType> &beginEndAndSize)
{
    return beginEndAndSize.third;
}

template<typename _Iterator>
inline_hint void advance (_Iterator &iterator, DifferenceType n) {
    if (n < 0) {
        while (n++)
        {
            --iterator;
        }
    } else {
        while (n--)
        {
            ++iterator;
        }
    }
}

template<typename _Iterator>
inline_hint _Iterator next(_Iterator iterator, DifferenceType n = 1) {
    advance(iterator, n);
    return iterator;
}

template<typename _Iterator>
inline_hint _Iterator prev(_Iterator iterator, DifferenceType n = 1) {
    advance(iterator, -n);
    return iterator;
}

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_ITERATORTOOLS_H
