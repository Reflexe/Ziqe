/**
 * @file IteratorTools.h
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
#ifndef ZIQE_ITERATORTOOLS_H
#define ZIQE_ITERATORTOOLS_H

#include "Types.h"
#include "Macros.h"

#include <utility>

namespace Ziqe {

template<class InputIterator>
class MoveIterator
{
    InputIterator mInput;

public:
    MoveIterator(const InputIterator &iterator)
        : mInput{iterator}
    {
    }

    ALLOW_COPY_AND_MOVE (MoveIterator)

    decltype (std::move(*mInput)) &&operator* ()
    {
        return std::move(*mInput);
    }

    MoveIterator &operator++ () {
        ++mInput;
        return *this;
    }

    MoveIterator &operator-- () {
        --mInput;

        return *this;
    }

    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (MoveIterator, mInput)
};

template<class InputIterator>
SizeType countRange (const InputIterator &begin, const InputIterator &end) {
    SizeType count = 0;

    for (auto iterator = begin; iterator != end; ++iterator)
    {
        ++count;
    }

    return count;
}

template <class InputIterator>
MoveIterator<InputIterator> makeMoveIterator(const InputIterator &inputIterator)
{
    return MoveIterator<InputIterator>{inputIterator};
}

} // namespace Ziqe

#endif // ZIQE_ITERATORTOOLS_H
