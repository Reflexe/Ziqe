/**
 * @file GlobalThread.hpp
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
#ifndef ZIQE_GLOBALTHREAD_H
#define ZIQE_GLOBALTHREAD_H

#include "Base/Types.hpp"
#include "Base/LocalThread.hpp"
#include "Base/LocalProcess.hpp"

#include "Core/ProcessPeersClient.hpp"
#include "Core/ThreadOwnerClient.hpp"

namespace Ziqe {

class GlobalThread
{
public:
    GlobalThread(LocalThread &localThread);

    typedef GlobalThreadID ID;

    // An handler for the system call hook.
    ZqRegisterType onSystemCall(ZqSystemCallIDType systemCallID,
                                const Base::RawArray<const ZqRegisterType> &parameters);


private:
    // TODO: finish ProcessPeersClient and ThreadOwnerClient
#if 0
    ProcessPeersClient mProcessPeersClient;

    ThreadOwnerClient mThreadOwnerClient;
#endif
};

} // namespace Ziqe

#endif // ZIQE_GLOBALTHREAD_H
