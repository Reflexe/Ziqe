/**
 * @file Process.cpp
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
#include "Process.h"

namespace Ziqe {

Process::Process(const ProcessMemory::MemoryConfiguration &memoryConfiguration)
    : mProcessMemory{memoryConfiguration}
{

}

void Process::updateMemoryPage(const ProcessMemory::Address address,
                               ProcessMemory::MemoryPage &&page)
{
    mProcessMemory.lookupPage (address) = std::move (page);
}

const ProcessMemory &Process::getProcessMemory() const
{
    return mProcessMemory;
}

} // namespace Ziqe
