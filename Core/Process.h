/**
 * @file Process.h
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
#ifndef ZIQE_PROCESS_H
#define ZIQE_PROCESS_H

#include "Base/Memory.h"
#include "Base/LinkedList.h"

#include "Thread.h"
#include "ProcessMemory.h"

namespace Ziqe {

class Process
{
public:
    Process(const ProcessMemory::MemoryConfiguration &memoryConfiguration);

    /**
     * @brief updateMemoryPage Replace a page in the process's memory.
     * @param address
     * @param page
     */
    void updateMemoryPage (const ProcessMemory::Address address,
                           ProcessMemory::MemoryPage  &&page);

    const ProcessMemory &getProcessMemory() const;

private:
    /// The threads this process has.
    LinkedList<Thread> mThreads;

    /**
     * @brief mProcessMemory  The process's memory.
     */
    ProcessMemory mProcessMemory;
};

} // namespace Ziqe

#endif // ZIQE_PROCESS_H
