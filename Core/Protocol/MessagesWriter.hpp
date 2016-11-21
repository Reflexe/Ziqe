/**
 * @file MessagesWriter.hpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2016 Shmuel Hazan
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
#ifndef MESSAGESWRITER_HPP
#define MESSAGESWRITER_HPP

#include "Protocol/Message.hpp"
#include "Protocol/MemoryRevision.hpp"

#include "Common/Types.hpp"

#include "Base/FieldWriter.hpp"

namespace Ziqe {
namespace Protocol {


template<class FieldWriter>
class MessagesWriter
{
    FieldWriter &mFieldWriter;

public:
    MessagesWriter(FieldWriter &fieldWriter)
        : mFieldWriter{fieldWriter}
    {
    }

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

    template<class ...Args>
    void writeMessage (const Message &message, Args&&...args)
    {
        mFieldWriter.writeT (static_cast<Message::MessageTypeInteger>(message.getType ()),
                             Base::forward<Args>(args)...);
    }

    template<class MessageType>
    void write (const MessageType &message)
    {
        writeMessage (message);
    }

    // Overloads for each message type.
    //void write()

    template<Message::Type type>
    void write(MessageWithThreadID<type> &message) {
        writeMessage (message, message.getThreadID ());
    }


    template<Message::Type type>
    void write(MessageWithThreadIDs<type> &message) {
        writeMessage (message, message.getThreadIDs ());
    }

    /// Stop Thread
    static MessageContainer makeStopThread (HostedThreadID threadID)
    {
        return makeVector(Message::Type::StopThread,
                          static_cast<uint64_t>(threadID));
    }

    static MessageContainer makeStopThreadOK ()
    {
        return makeVector(Message::Type::StopThreadOK);
    }

    /// Cont Thread
    static MessageContainer makeContinueThread (HostedThreadID threadID)
    {
        return makeVector(Message::Type::ContinueThread,
                          static_cast<uint64_t>(threadID));
    }

    static MessageContainer makeContinueThreadOK ()
    {
        return makeVector(Message::Type::ContinueThreadOK);
    }

    /// Kill Thread
    static MessageContainer makeKillThread (HostedThreadID threadID)
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
    static MessageContainer makeProcessPeerHello (const Base::RawArray<HostedThreadID> &threads)
    {
        return makeVector(Message::Type::ProcessPeerHello,
                          threads);
    }

    /// Goodbye
    static MessageContainer makeProcessPeerGoodbye (const Base::RawArray<HostedThreadID> &threadsID)
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
        return makeVector(Message::Type::DoSystemCallResult,
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


} // namespace Protocol
} // namespace Ziqe

#endif // MESSAGESWRITER_HPP
