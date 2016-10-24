/**
 * @file main.cpp
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
 *
 */

#include "Base/SystemCalls.hpp"
#include "Base/ProcessManager.hpp"

#include "Core/ZiqeProtocol/GlobalPeers.hpp"

#include "Core/GlobalThreadManager.hpp"
#include "Core/GlobalProcessManager.hpp"

#include "ZiqeAPI/EntryPoints.h"

class DriverMain
{
public:
    DriverMain()
    {
    }

    ~DriverMain()
    {
    }

private:
    Ziqe::GlobalThreadManager mThreadManager;
};

namespace {
DriverMain *gDriverMain;
}

void ZqAPIInit ()
{
    gDriverMain = new DriverMain;
}

void ZqAPIExit ()
{
    delete gDriverMain;
}

ZqBool systemCallHook (const ZqSystemCallIDType id,
                       const ZqRegisterType *params,
                       ZqRegisterType *result);

namespace {
Ziqe::GlobalThreadManager *gThreadManager;
Ziqe::GlobalProcessManager *gProcessManager;
}

ZqBool systemCallHook (const ZqSystemCallIDType id,
                       const ZqRegisterType *params,
                       ZqRegisterType *result)
{
    auto localThread = Ziqe::ProcessManager::getCurrentThread ();

    auto globalThread = gThreadManager->localToGlobalThread (localThread);

    // Don't hook this thread.
    if (! globalThread)
        return ZQ_FALSE;

    // Compare the current memory map with the previous, and send it to the owner.
    // We can use the "changed pages" as an hint.

    return ZQ_TRUE;
}



//int main (int argc, char *argv[]) {
//    using namespace Ziqe;

//    auto current = ProcessManager::getCurrentThread ();
//    SharedPointer<NetworkProtocol> mNetworkProtocol;

//    gThreadManager = new GlobalThreadManager{};
//    gProcessManager = new GlobalProcessManager{makeUnique<GlobalPeers>(mNetworkProtocol),
//                                               gThreadManager};

//    SystemCalls::setSystemCallHook (&systemCallHook);

//    GlobalPeersServer globalServer{mNetworkProtocol,
//                                   UniquePointer<GlobalPeersServer::Callback>{new GlobalPeersWorker}};

//    while (! current.shouldExit ())
//    {
//        globalServer.run ();
//    }

//    SystemCalls::unsetSystemCallHook ();

//    delete gThreadManager;
//    delete gProcessManager;

//    return 0;
//}
