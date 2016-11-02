/**
 * @file Mutex.hpp
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
#ifndef ZIQE_MUTEX_H
#define ZIQE_MUTEX_H

#include "Base/Macros.hpp"
#include "Base/Checks.hpp"

#include "ZiqeAPI/Mutex.h"

namespace Ziqe {

class Mutex
{
public:
    Mutex()
    {
        ZqMutexInit (&mMutex);
    }

    ~Mutex() {
        if (mMutex == ZQ_UNDEFINED_MUTEX)
            return;

        // Make sure this mutex isn't locked.
        DEBUG_CHECK (ZqMutexTryLock (&mMutex) == ZQ_TRUE);
        ZqMutexDeinit (&mMutex);
    }

    ZQ_DISALLOW_COPY (Mutex)

    Mutex (Mutex &&other)
        : mMutex{other.mMutex}
    {
        other.mMutex = ZQ_UNDEFINED_MUTEX;
    }

    Mutex &operator= (Mutex &&other) {
        // Make sure this mutex isn't locked.
        DEBUG_CHECK (ZqMutexTryLock (&mMutex) == ZQ_TRUE);

        ZqMutexDeinit (&mMutex);

        mMutex = other.mMutex;
        other.mMutex = ZQ_UNDEFINED_MUTEX;

        return *this;
    }

    class ScopedLock
    {
    public:
        explicit ScopedLock (Mutex &mutex)
            : mLockedMutex{&mutex}
        {
            ZqMutexLock (&mLockedMutex->mMutex);
        }

        ~ScopedLock() {
            if (mLockedMutex)
                ZqMutexUnlock (&mLockedMutex->mMutex);
        }

        ZQ_DISALLOW_COPY (ScopedLock)

        ScopedLock &operator =(ScopedLock &&) = delete;

        ScopedLock(ScopedLock &&other)
            : mLockedMutex{other.mLockedMutex}
        {
            other.mLockedMutex = nullptr;
        }

    private:
        Mutex *mLockedMutex;
    };

private:
    friend ScopedLock;

    ZqMutex mMutex;

};

} // namespace Ziqe

#endif // ZIQE_MUTEX_H
