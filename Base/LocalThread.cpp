/**
 * @file LocalThread.cpp
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
#include "LocalThread.hpp"

#include "Checks.hpp"
#include "Macros.hpp"

ZQ_BEGIN_NAMESPACE

LocalThread::LocalThread(ThreadID threadID)
    : mThreadID{threadID}
{
}

ZqThreadInfo &LocalThread::getThreadInfo() const{
    if (! mIsThreadInfoUpdated) {
        ZQ_ASSERT (ZqGetThreadInfo (mThreadID, &mThreadInfo) == ZQ_TRUE);
        mIsThreadInfoUpdated = true;
    }

    return mThreadInfo;
}

template<class ClassType, void (ClassType::*Function) ()>
void runMemberFunction (ClassType *c)
{
    (c->*Function) ();
}

//void LocalThread::run(Callback<void ()> &&toCall) {
//    Callback<void ()> *newCallback = new Callback<void ()> (Base::move (toCall));

//    return runAnyFunction(&runMemberFunction<Callback<void ()>, &Callback<void ()>::operator ()>,
//                          newCallback);
//}

void LocalThread::runFunction(void (*function)(ZqRegisterType), ZqRegisterType parameter) {
    ZqThreadCall (mThreadID,
                  reinterpret_cast<ZqRegisterType>(function),
                  parameter);
}

ZQ_END_NAMESPACE
