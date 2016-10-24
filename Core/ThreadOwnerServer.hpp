/**
 * @file ThreadOwnerServer.h
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
#ifndef ZIQE_THREADOWNERSERVER_H
#define ZIQE_THREADOWNERSERVER_H

#include "Base/Types.hpp"
#include "Base/LocalThread.hpp"

#include "Protocol/MessageStream.hpp"

namespace Ziqe {
class ThreadOwnerServer final
{
public:
    ThreadOwnerServer();
    void run();

private:
    void onDoSystemCall(const ZqSystemCallIDType systemCallID,
                        Base::RawPointer<const ZqRegisterType> parameters,
                        const SizeType parametersLength, Protocol::MemoryRevision &remoteRevision,
                        Protocol::MemoryRevision::ID previousRemoteRevisionID);

    void onMessageReceived(const Protocol::Message &type,
                           Protocol::MessageStream::Callback::MessageFieldReader &fieldReader,
                           const Protocol::MessageStream &messageStream);


    LocalThread mLocalThread;
};
} // namespace Ziqe
#endif // ZIQE_THREADOWNERSERVER_H
