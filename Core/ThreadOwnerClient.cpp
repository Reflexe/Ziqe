/**
 * @file ThreadOwnerClient.cpp
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
#include "ThreadOwnerClient.hpp"

#include "MessagesGenerator.hpp"

namespace Ziqe {
namespace Protocol {

ThreadOwnerClient::ThreadOwnerClient(Base::UniquePointer<MessageStream> &&stream)
    : mThreadOwnerStream{Base::move (stream)}
{

}

ZqRegisterType ThreadOwnerClient::doSystemCall(ZqSystemCallIDType id,
                                               const Base::RawArray<ZqRegisterType> parameters,
                                               MemoryRevision &revision)
{
    sendThreadOwnerMessage (MessagesGenerator::makeDoSystemCall (id,
                                                                 parameters,
                                                                 revision));
    waitUntilTaskComplete (mSystemCallTask);

    return mSystemCallTask.result;
}

ZqUserAddress ThreadOwnerClient::getAndReserveMemory(SizeType bytesCount) {
    sendThreadOwnerMessage (MessagesGenerator::makeGetAndReserveMemory (bytesCount));

    waitUntilTaskComplete (mGetAndReserveMemoryTask);

    return mGetAndReserveMemoryTask.address;
}

void ThreadOwnerClient::onMessageReceived(const Message &type,
                                          MessageStream::MessageFieldReader &fieldReader,
                                          const Net::Stream &stream)
{
    if (mCurrentTaskType == Task::Type::DoSystemCall) {
        // TODO: memory revision
    } else if (mCurrentTaskType == Task::Type::GetAndReserveMemory) {
    } else {
        return;
    }
}

} // namespace Ziqe
} // namespace Protocol
