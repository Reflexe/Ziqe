/**
 * @file SystemCallTask.h
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
#ifndef ZIQE_SYSTEMCALLTASK_H
#define ZIQE_SYSTEMCALLTASK_H

#include "Base/Memory.h"
#include "Base/SystemCalls.h"
#include "Base/SharedVector.h"

#include "Base/LocalProcess.h"
#include "Base/LocalThread.h"

#include "Core/ProcessMemory.h"
#include "Core/Task.h"

namespace Ziqe {

// System calls in Ziqe all sepical: there're also a sync point.
// In other words, after a syscall, s
class SystemCallTask final : public Task
{
public:
    SystemCallTask(SharedVector<ZqRegisterType> &&systemCallParameters,
                   SystemCalls::SystemCallID systemCallID,
                   const SharedPointer<LocalProcess> &process,
                   const SharedPointer<LocalThread> &thread,
                   const SharedPointer<ProcessMemory> &memoryMap);

    virtual bool run () override;

private:
    void onSystemCallFinished (ZqRegisterType result);

    bool isValidMemoryMap(const LocalProcess &process, const ProcessMemory &memoryMap);

    SharedVector<ZqRegisterType> mSystemCallParameters;

    SystemCalls::SystemCallID mSystemCallID;

    SharedPointer<LocalProcess> mProcess;
    SharedPointer<LocalThread> mThread;

    SharedPointer<ProcessMemory> mMemoryMap;
};

} // namespace Ziqe

#endif // ZIQE_SYSTEMCALLTASK_H
