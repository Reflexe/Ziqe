/**
 * @file SpinLock.hpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Main: copyright (C) 2016 Shmuel Hazan
 *
 * Main is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Main is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ZIQE_SPINLOCK_H
#define ZIQE_SPINLOCK_H

#include "ZiqeAPI/SpinLock.h"
#include "Base/Macros.hpp"
#include "Base/Memory.hpp"

namespace Ziqe {

class SpinLock
{
public:
    SpinLock()
    {
        ZqSpinLockInit (&mLock);
    }

    ~SpinLock() {
        if (mLock == ZQ_UNDEFINED_SPINLOCK)
            return;

        ZqSpinLockDeinit (&mLock);
    }

    ZQ_DISALLOW_COPY (SpinLock)

    SpinLock (SpinLock &&other)
        : mLock{other.mLock}
    {
        other.mLock = ZQ_UNDEFINED_SPINLOCK;
    }

    SpinLock &operator= (SpinLock &&other) {
        if (mLock != ZQ_UNDEFINED_SPINLOCK)
            ZqSpinLockDeinit (&mLock);

        mLock = other.mLock;
        other.mLock = ZQ_UNDEFINED_SPINLOCK;

        return *this;
    }

    class ScopedLock
    {
    public:
        ScopedLock (SpinLock &lock)
            : mLockedLock{&lock}
        {
            ZqSpinLockLock (&mLockedLock->mLock);
        }

        ~ScopedLock() {
            if (mLockedLock)
                ZqSpinLockUnlock (&mLockedLock->mLock);
        }

        ZQ_DISALLOW_COPY (ScopedLock)

        ScopedLock &operator =(ScopedLock &&) = delete;

        ScopedLock(ScopedLock &&other)
            : mLockedLock{other.mLockedLock}
        {
            other.mLockedLock = nullptr;
        }

    private:
        SpinLock *mLockedLock;
    };


private:
    friend ScopedLock;

    ZqSpinLock mLock;
};

template<class T, class PointerType=Base::RawPointer<SpinLock>>
class SpinLocked
{
public:
    template<class... Args>
    SpinLocked(PointerType &&lock, Args&&... values)
        : mLock{Base::move (lock)}, mValue{Base::forward<Args> (values)...}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (SpinLocked)
    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (SpinLocked, mValue)

    const Base::Pair<T&, SpinLock::ScopedLock> get () {
        return Base::Pair<T&, SpinLock::ScopedLock>{mValue,
                    *mLock};
    }

private:
    PointerType mLock;
    T mValue;
};

template<class T>
using UniqueSpinLocked=SpinLocked<T, Base::UniquePointer<SpinLock>>;

} // namespace Ziqe

#endif // ZIQE_SPINLOCK_H
