/**
 * @file GlobalProcess.cpp
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
#include "GlobalProcess.h"

#include "Base/ProcessManager.h"

namespace Ziqe {

GlobalProcess::GlobalProcess(UniquePointer<ProcessPeersServer> &&server,
                             UniquePointer<ProcessPeersClient> &&client,
                             UniquePointer<NetworkProtocolPool> &&networkPool)
    : mServer{std::move (server)}, mClient{std::move (client)}
{
    mServerThread.runAnyFunction (&GlobalProcess::processServerMain,
                                  new ProcessServerParameter{std::move (server),
                                                             std::move(networkPool)});
}

void GlobalProcess::processServerMain(ProcessServerParameter *parameter) {
    UniquePointer<ProcessServerParameter> local{parameter};

    auto currentThread = ProcessManager::getCurrentThread ();

    while (! currentThread.shouldExit ())
    {
        local->networkPool->run (*local->networkPool);
    }
}

} // namespace Ziqe
