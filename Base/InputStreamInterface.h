/**
 * @file InputStreamInterface.h
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
#ifndef ZIQE_INPUTSTREAMINTERFACE_H
#define ZIQE_INPUTSTREAMINTERFACE_H

#include "Base/Types.h"
#include "SharedVector.h"

namespace Ziqe {

class InputStreamInterface
{
public:
    typedef SharedVector<Byte> DataType;

    // Any input stream should provide this callback.
    class Callback {
    public:
        Callback() = default;
        virtual ~Callback () = 0;
        ALLOW_COPY_AND_MOVE (Callback)

        virtual void onDataReceived (const DataType &data) = 0;
    };

protected:
    InputStreamInterface();
    virtual ~InputStreamInterface() = 0;

    ALLOW_COPY_AND_MOVE (InputStreamInterface)
};

} // namespace Ziqe

#endif // ZIQE_INPUTSTREAMINTERFACE_H
