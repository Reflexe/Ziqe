/**
 * @file Atomic.h
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
#ifndef ZIQE_ATOMIC_H
#define ZIQE_ATOMIC_H

#include "Mutex.h"

#include <utility>

namespace Ziqe {

template<class T>
class Atomic
{
public:
    template<class ...Args>
    Atomic(Args&&... args)
        : mValue{std::forward<Args>(args)...}
    {
    }

    struct GenericAtomicType {
        GenericAtomicType(T &value, Mutex &mutex)
            : mValue{value}, mLock{Mutex::Lock{mutex}}
        {
        }

        T &get ()
        {
            return mValue;
        }

    private:
        T &mValue;

        mutable Mutex::Lock mLock;
    };

    struct GenericConstAtomicType {
        GenericConstAtomicType(const T &value, Mutex &mutex)
            : mValue{value}, mLock{Mutex::Lock{mutex}}
        {
        }

        const T &get ()
        {
            return mValue;
        }

    private:
        const T &mValue;

        mutable Mutex::Lock mLock;
    };


    // Undefined beheviour if the return value of lock is get used after
    // *this gets destroyed.
    GenericAtomicType lock ()
    {
        return GenericAtomicType{mValue, mMutex};
    }

    GenericConstAtomicType lock () const
    {
        return GenericConstAtomicType{mValue, mMutex};
    }

private:
    T mValue;

    mutable Mutex mMutex;
};

} // namespace Ziqe

#endif // ZIQE_ATOMIC_H
