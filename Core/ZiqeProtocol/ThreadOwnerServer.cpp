/**
 * @file ThreadOwnerServer.cpp
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
#include "ThreadOwnerServer.h"

#include "Message.h"

#include "Base/FieldReader.h"

namespace Ziqe {

ThreadOwnerServer::ThreadOwnerServer(UniquePointer<Callback> &&callback)
    : mCallback{callback}
{
}

void ThreadOwnerServer::onDataReceived(const InputStreamInterface::DataType &data) {
    FieldReader reader{copy(data)};

    if (! Message::isValidReader (reader))
        return;

    Message message{reader};

    if (! message.isThreadP2PMessage ())
        return;

    switch (message.getType ())
    {
    // 64Bit: syscall id.
    // 8Bit : arg number.
    // ???? : arg data.
    // ???? : syscall data (memory).
    case Message::Type::DoSystemCall: {
        if (! reader.haveBytes (8+1))
            return;

        ZqSystemCallIDType syscallID = reader.readT<uint64_t>();
        uint8_t            argsLength = reader.readT<uint8_t>();

        if (! reader.haveBytes (static_cast<SizeType>(argsLength) * sizeof (ZqRegisterType)))
            return;

        auto currentByte = reader.getCurrent ();
        reader.skipBytes (static_cast<SizeType>(argsLength) * sizeof (ZqRegisterType));

        // TODO: what to do with the memory?
        mCallback->onDoSystemCall (syscallID,
                                   currentByte,
                                   argsLength);

        break;
    }
    default:
        // Message::Type::SystemCallResult
        return;
    }
}

} // namespace Ziqe
