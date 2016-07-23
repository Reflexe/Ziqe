/**
 * @file GlobalProcessManager.h
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
#ifndef ZIQE_GLOBALPROCESSMANAGER_H
#define ZIQE_GLOBALPROCESSMANAGER_H

#include "GlobalProcess.h"
#include "GlobalThread.h"

#include "Base/LocalThread.h"
#include "Base/LocalProcess.h"

#include "Base/HashTable.h"

namespace Ziqe {

class GlobalProcessManager
{
public:
    GlobalProcessManager();

    void addProcess (const GlobalProcess &process);
    void addProcess (const GlobalProcess &globalProcess,
                     const LocalProcess &localProcess);

    void addThread (const GlobalProcess &process);

    // On failure?
    LocalProcess &getLocalProcessByGlobal (const GlobalProcess &globalProcess) {
        auto iterator = mPIDToProcess.find (globalProcess.getProcessID ());
        DEBUG_CHECK (iterator != mPIDToProcess.end ());

        return iterator->second;
    }

    LocalThread &getLocalThreadByGlobal (const GlobalThread &globalThread) {
        auto iterator = mTIDToThread.find (globalThread.getThreadID ());
        DEBUG_CHECK (iterator != mTIDToThread.end ());

        return iterator->second;
    }

private:
    HashTable<LocalProcess::ProcessID, LocalProcess> mPIDToProcess;

    HashTable<LocalThread::ThreadID, LocalThread> mTIDToThread;
};

} // namespace Ziqe

#endif // ZIQE_GLOBALPROCESSMANAGER_H
