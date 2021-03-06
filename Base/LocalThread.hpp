/**
 * @file LocalThread.hpp
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
#ifndef ZIQE_LOCALTHREAD_H
#define ZIQE_LOCALTHREAD_H

#include "Base/Types.hpp"

#include "OS/Process.h"

ZQ_BEGIN_NAMESPACE

class LocalThread
{
public:
    typedef ZqThreadID ThreadID;

    LocalThread(ThreadID threadID);

    ZqThreadInfo &getThreadInfo() const;

    ThreadID getThreadID () const
    {
        return mThreadID;
    }

    void stop ();

    void cont();
    void kill ();

//    void run(Callback<void ()> &&toCall);

    bool shouldExit () const;
    bool isStopped () const;

    template<class ParameterType>
    void runAnyFunction (void (*function)(ParameterType), ParameterType parameter)
    {
        static_assert(sizeof (ParameterType) == sizeof (ZqRegisterType),
                      "Invalid size of ParameterType");

        return runFunction (reinterpret_cast<void (*)(ZqRegisterType)>(function),
                            reinterpret_cast<ZqRegisterType>(parameter));
    }

    void runFunction (void (*function)(ZqRegisterType), ZqRegisterType parameter);

private:
    ThreadID mThreadID;

    mutable ZqThreadInfo mThreadInfo;
    mutable bool         mIsThreadInfoUpdated = false;
};

ZQ_END_NAMESPACE

#endif // ZIQE_LOCALTHREAD_H
