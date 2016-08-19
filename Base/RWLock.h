/**
 * @file RWLock.h
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
#ifndef ZIQE_RWLOCK_H
#define ZIQE_RWLOCK_H

#include "ZiqeAPI/RWLock.h"
#include "Base/Macros.h"
#include "Base/Memory.h"

namespace Ziqe {

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

    DISALLOW_COPY (RWLock)

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

        DISALLOW_COPY (ScopedReadLock)

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

        DISALLOW_COPY (ScopedWriteLock)

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

template<class T>
class RWLocked
{
public:
    template<class... Args>
    Locked(const UglyPointer<RWLock> &lock, Args&&... values)
        : mLock{lock}, mValue{std::forward<Args> (values)}
    {
    }

    ALLOW_MOVE (Locked)
    DISALLOW_COPY (Locked)
    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (RWLocked, mValue)

    const Pair<T&, RWLock::ScopedReadLock> &getRead () {
        return Pair<T&, RWLock::ScopedReadLock>{RWLock::ScopedReadLock{mLock},
                                                mValue};
    }

    Pair<T&, RWLock::ScopedWriteLock> &getWrite () {
        return Pair<T&, RWLock::ScopedWriteLock>{RWLock::ScopedWriteLock{mLock},
                                                 mValue};
    }

private:
    UglyPointer<RWLock> mLock;
    T mValue;
};

} // namespace Ziqe

#endif // ZIQE_RWLOCK_H
