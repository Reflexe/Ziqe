/**
 * @file MessagesGenerator.h
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

#include "Core/ZiqeProtocol/Message.h"
#include "Core/ZiqeProtocol/MemoryMap.h"

#include "Core/GlobalThread.h"

#include "Base/FieldWriter.h"

namespace Ziqe {

class MessagesGenerator
{
public:
    MessagesGenerator();

    typedef Vector<Byte> MessageContainer;
    typedef uint64_t IdentiferType;


    template<class...Args>
    MessageContainer makeVector (Message::Type type, Args&&...args)
    {
        FieldWriter writer;
        writer.writeT(static_cast<uint16_t>(type),
                  std::forward<Args>(args)...);

        return writer.getVector().getVector ();
    }

    MessageContainer makeGetMemory (ZqAddress address,
                                    SizeType length) {
        return makeVector(Message::Type::GetMemory,
                          static_cast<QWord>(address),
                          static_cast<QWord>(length));
    }

    MessageContainer makeGetMemoryResult (const SharedVector<Byte> &data,
                                          ZqAddress address)
    {
        return makeVector(Message::Type::GetMemoryResult,
                          static_cast<QWord>(address),
                          static_cast<QWord>(data.size ()),
                          data);
    }

    MessageContainer makeGetMemoryResult (Word data,
                                          ZqAddress address)
    {
        return makeVector(Message::Type::GetMemoryResult,
                          static_cast<QWord>(address),
                          static_cast<QWord>(sizeof (data)),
                          data);
    }

    MessageContainer makeGetMemoryResult (DWord data,
                                          ZqAddress address)
    {
        return makeVector(Message::Type::GetMemoryResult,
                          static_cast<QWord>(address),
                          static_cast<QWord>(sizeof (data)),
                          data);
    }

    MessageContainer makeGetMemoryResult (QWord data,
                                          ZqAddress address)
    {
        return makeVector(Message::Type::GetMemoryResult,

                          static_cast<QWord>(address),
                          static_cast<QWord>(sizeof (data)),
                          data);
    }

    /// Write Memory
    MessageContainer makeWriteMemory (const SharedVector<Byte> &vector,
                                      ZqAddress address)
    {
        return makeVector(Message::Type::WriteMemory,
                          static_cast<QWord>(address),
                          static_cast<QWord>(vector.size ()),
                          vector);
    }

    MessageContainer makeWriteMemory (Word data,
                                      ZqAddress address)
    {
        return makeVector(Message::Type::WriteMemory,
                          static_cast<QWord>(address),
                          static_cast<QWord>(sizeof (data)),
                          data);
    }

    MessageContainer makeWriteMemory (DWord data,
                                      ZqAddress address)
    {
        return makeVector(Message::Type::WriteMemory,
                          static_cast<QWord>(address),
                          static_cast<QWord>(sizeof (data)),
                          data);
    }

    MessageContainer makeWriteMemory (QWord data,
                                      ZqAddress address)
    {
        return makeVector(Message::Type::WriteMemory,
                          static_cast<QWord>(address),
                          static_cast<QWord>(sizeof (data)),
                          data);
    }

    /// Give Page
    MessageContainer makeGivePage (ZqAddress address)
    {
        return makeVector(Message::Type::GivePage,
                          static_cast<QWord>(address));
    }

    /// Stop Thread
    MessageContainer makeStopThread (GlobalThreadID threadID)
    {
        return makeVector(Message::Type::StopThread,
                          static_cast<QWord>(threadID));
    }

    /// Cont Thread
    MessageContainer makeContinueThread (GlobalThreadID threadID)
    {
        return makeVector(Message::Type::ContiniueThread,
                          static_cast<QWord>(threadID));
    }

    /// Kill Thread
    MessageContainer makeKillThread (GlobalThreadID threadID)
    {
        return makeVector(Message::Type::KillThread,
                          static_cast<QWord>(threadID));
    }

    /// Hello
    MessageContainer makeProcessPeerHello (GlobalThreadID threadID)
    {
        return makeVector(Message::Type::ProcessPeerHello,
                          threadID);
    }

    /// Goodbye
    MessageContainer makeProcessPeerGoodbye (GlobalThreadID threadID)
    {
        return makeVector(Message::Type::ProcessPeerGoodbye,
                          threadID);
    }

    MessageContainer makeDoSystemCall (uint64_t systemCallID,
                                       UglyArray<ZqRegisterType> arguments,
                                       const MemoryMap &memoryMap)
    {
        return makeVector(Message::Type::DoSystemCall,
                          static_cast<uint8_t>(arguments.size ()),
                          arguments,
                          memoryMap.toVector ());
    }

    MessageContainer makeSystemCallResult (uint64_t result,
                                           const MemoryMap &memoryMap)
    {
        return makeVector(Message::Type::SystemCallResult,
                          result,
                          memoryMap.toVector ());
    }

    MessageContainer makeGetAndReserveMemory (uint16_t count)
    {
        return makeVector(Message::Type::GetAndReserveMemory,
                          count);
    }

    MessageContainer makeRunThreadPeerLookup (IdentiferType identifer)
    {
        return makeVector(Message::Type::RunThreadPeerLookup,
                          identifer);
    }

    MessageContainer makeRunThreadPeerPropose (IdentiferType identifer)
    {

    }

    MessageContainer makeRunThreadPeerAcceptPropose (IdentiferType identifer);
};

} // namespace Ziqe

#endif // ZIQE_MESSAGESGENERATOR_H
