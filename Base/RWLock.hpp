/**
 * @file RWLock.hpp
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
#ifndef ZIQE_RWLOCK_H
#define ZIQE_RWLOCK_H

#include "ZiqeAPI/RWLock.h"
#include "Base/Macros.hpp"
#include "Base/Memory.hpp"

namespace Ziqe {
namespace Base {

class RWLock
{
public:
    RWLock()
    {
        ZqRWLockInit (&mLock);
    }

    ~RWLock() {
        if (mLock == ZQ_UNDEFINED_RWLOCK)
            return;

        ZqRWLockDeinit (&mLock);
    }

    ZQ_DISALLOW_COPY (RWLock)

    RWLock (RWLock &&other)
        : mLock{other.mLock}
    {
        other.mLock = ZQ_UNDEFINED_RWLOCK;
    }

    RWLock &operator= (RWLock &&other) {
        ZqRWLockDeinit (&mLock);

        mLock = other.mLock;
        other.mLock = ZQ_UNDEFINED_RWLOCK;

        return *this;
    }

    class ScopedReadLock
    {
    public:
        ScopedReadLock (RWLock &lock)
            : mLockedLock{&lock}
        {
            ZqRWLockLockRead (&mLockedLock->mLock);
        }

        ~ScopedReadLock() {
            if (mLockedLock)
                ZqRWLockUnlockRead (&mLockedLock->mLock);
        }

        ZQ_DISALLOW_COPY (ScopedReadLock)

        ScopedReadLock &operator =(ScopedReadLock &&) = delete;

        ScopedReadLock(ScopedReadLock &&other)
            : mLockedLock{other.mLockedLock}
        {
            other.mLockedLock = nullptr;
        }

    private:
        RWLock *mLockedLock;
    };

    class ScopedWriteLock
    {
    public:
        ScopedWriteLock (RWLock &lock)
            : mLockedLock{&lock}
        {
            ZqRWLockLockWrite (&mLockedLock->mLock);
        }

        ~ScopedWriteLock() {
            if (mLockedLock)
                ZqRWLockUnlockWrite (&mLockedLock->mLock);
        }

        ZQ_DISALLOW_COPY (ScopedWriteLock)

        ScopedWriteLock &operator =(ScopedWriteLock &&) = delete;

        ScopedWriteLock(ScopedWriteLock &&other)
            : mLockedLock{other.mLockedLock}
        {
            other.mLockedLock = nullptr;
        }

    private:
        RWLock *mLockedLock;
    };
private:
    friend ScopedWriteLock;
    friend ScopedReadLock;

    ZqRWLock mLock;
};

template<class T, class PointerType=DummyPointer<RWLock>>
class RWLocked
{
public:
    template<class... Args>
    RWLocked(RWLock &lock, Args&&... values)
        : mLock{&lock}, mValue{Base::forward<Args> (values)...}
    {
    }

    template<class... Args>
    RWLocked(Args&&... values)
        : mValue{Base::forward<Args> (values)...}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (RWLocked)
    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (RWLocked, mValue)

    const Pair<const T&, RWLock::ScopedReadLock> getRead () {
        return Pair<const T&, RWLock::ScopedReadLock>{mValue,
                    *mLock};
    }

    Pair<T&, RWLock::ScopedWriteLock> getWrite () {
        return Pair<T&, RWLock::ScopedWriteLock>{mValue,
                    RWLock::ScopedWriteLock{*mLock}};
    }

private:
    PointerType mLock;
    T mValue;
};

template<class T>
using SharedRWLocked=SharedPointer<RWLocked<T>>;

template<class T>
using RawRWLocked=RawPointer<RWLocked<T>>;

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_RWLOCK_H
