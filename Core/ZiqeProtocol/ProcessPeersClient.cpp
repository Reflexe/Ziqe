/**
 * @file ProcessPeersClient.cpp
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
#include "ProcessPeersClient.h"

#include "MessagesGenerator.h"

namespace Ziqe {

ProcessPeersClient::ProcessPeersClient()
{
    sayHello ();
}

ProcessPeersClient::~ProcessPeersClient()
{
    sayGoodbye ();
}

UniquePointer<ProcessPeersClient> ProcessPeersClient::runThread(LocalThread &localThread)
{

}

SharedVector<Byte> ProcessPeersClient::getMemory(ZqAddress address, SizeType size) {
    DEBUG_CHECK (mCurrentTask == nullptr);

    sendMessageToProcessPeers (MessagesGenerator::makeGetMemory (address,
                                                                 size));

    mCurrentTask = new GetMemoryTask{};
    waitUntilTaskComplete ();

    UniquePointer<Task> task{std::move(mCurrentTask)};
    auto pointer = static_cast<GetMemoryTask*>(task.get ());

    return std::move(pointer->mMemory);
}

void ProcessPeersClient::writeMemory(const SharedVector<Byte> &data, ZqAddress address)
{
    sendMessageToProcessPeers (MessagesGenerator::makeWriteMemory (data,
                                                                  address));
}

void ProcessPeersClient::killRemoteThread(GlobalThreadID threadID)
{
   sendThreadMessage (threadID, MessagesGenerator::makeKillThread (threadID));
}

void ProcessPeersClient::stopRemoteThread(GlobalThreadID threadID)
{
    sendThreadMessage (threadID, MessagesGenerator::makeStopThread (threadID));
}

void ProcessPeersClient::continiueRemoteThread(GlobalThreadID threadID)
{
    sendThreadMessage (threadID, MessagesGenerator::makeContinueThread (threadID));
}

ZqRegisterType ProcessPeersClient::doSystemCall(ZqSystemCallIDType id, const UglyArray<ZqRegisterType> parameters, MemoryRevision &revision)
{
    sendThreadOwnerMessage (MessagesGenerator::makeDoSystemCall (id,
                                                                 parameters,
    {}));

    DEBUG_CHECK (mCurrentTask == nullptr);

    mCurrentTask = new DoSystemCallTask{};
    waitUntilTaskComplete ();

    UniquePointer<Task> task{std::move(mCurrentTask)};
    auto pointer = static_cast<DoSystemCallTask*>(task.get ());
    auto result = pointer->result;

    // localProcess.applyMemoryMap (std::move(pointer.memoryMap));

    return result;
}

ZqAddress ProcessPeersClient::getAndReserveMemory(SizeType pagesCount) {
    DEBUG_CHECK (mCurrentTask == nullptr);

    sendThreadOwnerMessage (MessagesGenerator::makeGetAndReserveMemory (pagesCount));

    mCurrentTask = new GetAndReserveMemoryTask{};
    waitUntilTaskComplete ();

    UniquePointer<Task> task{std::move(mCurrentTask)};
    auto pointer = static_cast<GetAndReserveMemoryTask*>(task.get ());

    return pointer->address;
}

void ProcessPeersClient::sayHello()
{
    sendMessageToProcessPeers (MessagesGenerator::makeProcessPeerHello (mMyGlobalThreadID));
}

void ProcessPeersClient::sayGoodbye()
{
    sendMessageToProcessPeers (MessagesGenerator::makeProcessPeerGoodbye (mMyGlobalThreadID));
}

void ProcessPeersClient::onGetMemoryResultReceived(const SharedVector<Byte> &memory)
{

}

void ProcessPeersClient::onRunThreadOKReceived()
{

}

void ProcessPeersClient::onSystemCallResultReceived(ZqRegisterType result)
{

}

} // namespace Ziqe
