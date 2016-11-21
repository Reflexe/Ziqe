/**
 * @file ThreadClient.cpp
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
#include "ThreadClient.hpp"

#include "Protocol/MessagesGenerator.hpp"

namespace Ziqe {
namespace Host {

ThreadClient::ThreadClient(Base::UniquePointer<Protocol::MessageStream> &&stream)
    : mThreadOwnerStream{Base::move (stream)}
{

}

ZqRegisterType ThreadClient::doSystemCall(ZqSystemCallIDType id,
                                               const Base::RawArray<ZqRegisterType> parameters,
                                               Protocol::MemoryRevision &revision)
{
    sendThreadOwnerMessage (Protocol::MessagesGenerator::makeDoSystemCall (id,
                                                                           parameters,
                                                                           revision));
    waitUntilTaskComplete (mSystemCallTask);

    return mSystemCallTask.result;
}

ZqUserAddress ThreadClient::getAndReserveMemory(SizeType bytesCount) {
    sendThreadOwnerMessage (Protocol::MessagesGenerator::makeGetAndReserveMemory (bytesCount));

    waitUntilTaskComplete (mGetAndReserveMemoryTask);

    return mGetAndReserveMemoryTask.address;
}

void ThreadClient::onMessageReceived(const Protocol::Message &type,
                                          Protocol::MessageStream::MessageFieldReader &fieldReader,
                                          const Protocol::MessageStream &messageStream)
{
    if (mCurrentTaskType == Task::Type::DoSystemCall) {
        // TODO: memory revision
    } else if (mCurrentTaskType == Task::Type::GetAndReserveMemory) {
    } else {
        return;
    }
}

} // namespace Host
} // namespace Ziqe
