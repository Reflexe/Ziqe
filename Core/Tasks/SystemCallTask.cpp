/**
 * @file SystemCallTask.cpp
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
#include "SystemCallTask.h"

namespace Ziqe {

SystemCallTask::SystemCallTask(const SharedPointer<Process> &process,
                               const SharedPointer<ProcessMemory> &memoryMap)
    : mProcess{process}, mMemoryMap{memoryMap}
{
}

bool SystemCallTask::run() {
    if (! isValidMemoryMap (*mProcess, *mMemoryMap)) {
        // Request a new one.

        return true;
    }

    // run the system call, set a callback to
    // run when it done, and send a broadcast with the new memory map.

    return true;
}

inline bool SystemCallTask::isValidMemoryMap(const Process &process,
                                             const ProcessMemory &memoryMap)
{
    return memoryMap.getRevision () <= process.getProcessMemory ().getRevision ();
}

} // namespace Ziqe
