/**
 * @file ThreadOwnerClient.h
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
#ifndef ZIQE_THREADOWNERCLIENT_H
#define ZIQE_THREADOWNERCLIENT_H

#include "Base/IOStreamInterface.h"

namespace Ziqe {

class ThreadOwnerClient : private IOStreamInterface::Callback
{
public:
    struct Callback {
        Callback() = default;
        virtual ~Callback() = 0;
        ALLOW_COPY_AND_MOVE (Callback)

        virtual void onSystemCallResultReceived (ZqRegisterType result) = 0;
    };

    ThreadOwnerClient(UniquePointer<Callback> &&callback);

    void doSystemCall (ZqSystemCallIDType id,
                       const UglyPointer<ZqRegisterType> parameters,
                       SizeType parametersLength);

private:
    UniquePointer<Callback> mCallback;
};

} // namespace Ziqe

#endif // ZIQE_THREADOWNERCLIENT_H
