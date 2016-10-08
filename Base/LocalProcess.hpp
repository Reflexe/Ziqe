/**
 * @file LocalProcess.hpp
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
#ifndef ZIQE_LOCALPROCESS_H
#define ZIQE_LOCALPROCESS_H

#include "Base/LinkedList.hpp"
#include "Base/Macros.hpp"

#include "ZiqeAPI/Process.h"

namespace Ziqe {

/// Process that runs on this computer.
class LocalProcess
{
public:
    typedef ZqProcessID ProcessID;

    LocalProcess(ProcessID processID);

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (LocalProcess, mProcessID)

    ZqProcessID getProcessID() const;

private:
    ProcessID mProcessID;

    // memory manager
};

} // namespace Ziqe

#endif // ZIQE_LOCALPROCESS_H
