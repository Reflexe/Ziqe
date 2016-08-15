/**
 * @file GlobalThread.cpp
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
#include "GlobalThread.h"

#include "ZiqeAPI/Memory.h"

#include "ThirdParty/Zydis/Zydis/Zydis.hpp"

namespace Ziqe {

GlobalThread::GlobalThread()
    : mThreadOwnerClient{std::move (threadOwnerClient)}
{

}

void GlobalThread::onPageFault(ZqAddress address, bool isWriteError) {
    /**
      * Two options:
      *  1. Write an hook after the fault instruction that will cleanup
      *     the readed / writen page.
      *  2. Process the instruction in kernel mode (here), and do what it
      *     doing in here.
      *  3.
      */

    /**
      * Final: Every page fault we'll get the page and reload it once in some time (if it updated).
      * On a system call, we'll send an memory map.
      */

    auto memory = mClient->getMemory (ZQ_PAGE_ALIGN (address),
                                      ZQ_PAGE_SIZE);

    ZqCopyToUser (ZQ_PAGE_ALIGN (address),
                  &memory[0],
                  memory.size ());
}


} // namespace Ziqe
