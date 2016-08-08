/**
 * @file GlobalProcess.h
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
#ifndef ZIQE_GLOBALPROCESS_H
#define ZIQE_GLOBALPROCESS_H

#include "Base/Types.h"
#include "Base/LinkedList.h"

#include "Core/GlobalProcessID.h"

#include "Core/ZiqeProtocol/ProcessPeersClient.h"
#include "Core/ProcessPeersWorker.h"

namespace Ziqe {

/**
 * @brief The GlobalProcess class
 *
 * Every global process is a server and a client of all the other process peers' server.
 */
class GlobalProcess
{
public:
    GlobalProcess(UniquePointer<ProcessPeersServer> &&server,
                  UniquePointer<ProcessPeersClient> &&client,
                  UniquePointer<NetworkProtocolPool> &&networkPool);

    ProcessPeersClient &getClient()
    {
        return *mClient;
    }

private:
    struct ProcessServerParameter {
        /**
         * @brief mServer  Handles other process peer's requests (e.g. read memory request)
         */
        UniquePointer<ProcessPeersServer> mServer;

        UniquePointer<NetworkProtocolPool> networkPool{pNetworkPool};

    };

    static void processServerMain (ProcessServerParameter *parameter);

    LocalThread mServerThread;

    /**
     * @brief mClient  Handles local requests for other process peers.
     */
    UniquePointer<ProcessPeersClient> mClient;

};

} // namespace Ziqe

#endif // ZIQE_GLOBALPROCESS_H
