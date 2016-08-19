/**
 * @file ThreadOwnerWorker.h
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
#ifndef ZIQE_THREADOWNERWORKER_H
#define ZIQE_THREADOWNERWORKER_H

#include "Core/ZiqeProtocol/ThreadOwnerServer.h"
#include "Core/GlobalProcess.h"

#include "Base/RWLock.h"

namespace Ziqe {

class ThreadOwnerWorker final : private ThreadOwnerServer::Callback
{
public:
    ThreadOwnerWorker();

private:
    void onDoSystemCall (const ZqSystemCallIDType systemCallID,
                         UglyPointer<const ZqRegisterType> parameters,
                         const SizeType parametersLength,
                         MemoryRevision &remoteRevision,
                         MemoryRevision::ID previousRemoteRevision) override;

    RWLocked<MemoryRevisionTree> mRevisionTree;
};

} // namespace Ziqe

#endif // ZIQE_THREADOWNERWORKER_H
