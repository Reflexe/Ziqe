/**
 * @file LocalThread.h
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
#ifndef ZIQE_LOCALTHREAD_H
#define ZIQE_LOCALTHREAD_H

#include "Base/Types.h"
#include "Base/Callback.h"

#include "ZiqeAPI/Process.h"

#include "Core/Thread.h"

namespace Ziqe {

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

    void run(Callback<void ()> &&toCall);

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

} // namespace Ziqe

#endif // ZIQE_LOCALTHREAD_H
