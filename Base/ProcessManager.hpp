/**
 * @file ProcessManager.hpp
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
#ifndef ZIQE_PROCESSMANAGER_H
#define ZIQE_PROCESSMANAGER_H

#include "LocalThread.hpp"
#include "LocalProcess.hpp"

ZQ_BEGIN_NAMESPACE

class ProcessManager
{
public:
    ProcessManager();

//    static LocalThread createThread (LocalProcess &process,
//                                     ZqAddress stackAddress,
//                                     ZqAddress runAddress);

    static LocalProcess createProcess ();

    static void runThread (LocalThread &thread);

    static LocalThread getCurrentThread();

    static LocalThread getLocalThreadByID (LocalThread::ThreadID id);

private:
};

ZQ_END_NAMESPACE

#endif // ZIQE_PROCESSMANAGER_H
