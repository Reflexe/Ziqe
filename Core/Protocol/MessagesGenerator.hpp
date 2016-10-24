/**
 * @file MessagesGenerator.hpp
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
#ifndef ZIQE_MESSAGESGENERATOR_H
#define ZIQE_MESSAGESGENERATOR_H

#include "Core/Protocol/Message.hpp"
#include "Core/Protocol/MemoryRevision.hpp"

#include "Core/GlobalThread.hpp"

#include "Base/FieldWriter.hpp"

namespace Ziqe {
namespace Protocol {

class MessagesGenerator
{
public:
    MessagesGenerator();

    typedef Base::Vector<uint8_t> MessageContainer;
    typedef uint64_t IdentiferType;

    template<class...Args>
    static MessageContainer makeVector (Message::Type type, Args&&...args) {
        // We are using little endian as a standard.
        Base::LittleEndianFieldWriter<Base::ExtendedVector<uint8_t>> writer;

        writer.writeT(static_cast<uint16_t>(type),
                      Base::forward<Args>(args)...);

        return Base::move (writer.getVector().getVector ());
    }

    /// Stop Thread
    static MessageContainer makeStopThread (GlobalThread::ID threadID)
    {
        return makeVector(Message::Type::StopThread,
                          static_cast<uint64_t>(threadID));
    }

    static MessageContainer makeStopThreadOK ()
    {
        return makeVector(Message::Type::StopThreadOK);
    }

    /// Cont Thread
    static MessageContainer makeContinueThread (GlobalThread::ID threadID)
    {
        return makeVector(Message::Type::ContinueThread,
                          static_cast<uint64_t>(threadID));
    }

    static MessageContainer makeContinueThreadOK ()
    {
        return makeVector(Message::Type::ContinueThreadOK);
    }

    /// Kill Thread
    static MessageContainer makeKillThread (GlobalThread::ID threadID)
    {
        return makeVector(Message::Type::KillThread,
                          static_cast<uint64_t>(threadID));
    }

    static MessageContainer makeKillThreadOK ()
    {
        return makeVector(Message::Type::KillThreadOK);
    }

    /// Hello
    /// Send the new IDs.
    static MessageContainer makeProcessPeerHello (const Base::RawArray<GlobalThreadID> &threads)
    {
        return makeVector(Message::Type::ProcessPeerHello,
                          threads);
    }

    /// Goodbye
    static MessageContainer makeProcessPeerGoodbye (const Base::RawArray<GlobalThreadID> &threadsID)
    {
        return makeVector(Message::Type::ProcessPeerGoodbye,
                          threadsID);
    }

    static MessageContainer makeDoSystemCall (uint64_t systemCallID,
                                              Base::RawArray<ZqRegisterType> arguments,
                                              const MemoryRevision &memoryRevision)
    {
        return makeVector(Message::Type::DoSystemCall,
                          systemCallID,
                          arguments);
        // TODO: memory revision pack-unpack.
    }

    static MessageContainer makeSystemCallResult (uint64_t result,
                                                  const MemoryRevision &memoryRevision)
    {
        return makeVector(Message::Type::SystemCallResult,
                          result);
        // TODO: memory revision pack-unpack.
    }

    static MessageContainer makeGetAndReserveMemory (uint64_t count)
    {
        return makeVector(Message::Type::GetAndReserveMemory,
                          count);
    }

    static MessageContainer makeRunThreadPeerLookup ()
    {
        return makeVector(Message::Type::RunThreadPeerLookup);
    }

    // TODO
    static MessageContainer makeRunThreadPeerPropose ()
    {

    }

    static MessageContainer makeRunThreadPeerAcceptPropose ();
};

} // namespace Ziqe
} // namespace Protocol

#endif // ZIQE_MESSAGESGENERATOR_H
