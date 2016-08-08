/**
 * @file GlobalThread.h
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
#ifndef ZIQE_GLOBALTHREAD_H
#define ZIQE_GLOBALTHREAD_H

#include "Base/Types.h"

#include "Core/ThreadOwnerClientWorker.h"
#include "Core/GlobalThreadID.h"

namespace Ziqe {

/**
 * @brief The GlobalThread class
 *
 * Every global thread have a connection to the thread owner.
 * The thread owner have a "ThreadOwnerServer" that currently is only syscall server.
 */
class GlobalThread
{
public:
    GlobalThread(UniquePointer<ThreadOwnerClientWorker> &&threadOwnerClient);

    ThreadOwnerClientWorker &getThreadOwnerClient ()
    {
        return *mThreadOwnerClient;
    }

private:
    UniquePointer<ThreadOwnerClientWorker> mThreadOwnerClient;
};

} // namespace Ziqe

#endif // ZIQE_GLOBALTHREAD_H
