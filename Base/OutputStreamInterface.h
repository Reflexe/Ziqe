/**
 * @file OutputStreamInterface.h
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
#ifndef ZIQE_OUTPUTSTREAMINTERFACE_H
#define ZIQE_OUTPUTSTREAMINTERFACE_H

#include "Base/SharedVector.h"
#include "Base/Types.h"

namespace Ziqe {

class OutputStreamInterface
{
public:
    OutputStreamInterface();
    virtual ~OutputStreamInterface () = 0;
    ALLOW_COPY_AND_MOVE (OutputStreamInterface)

    virtual void sendData (const Vector<Byte> &data) = 0;
};

} // namespace Ziqe

#endif // ZIQE_OUTPUTSTREAMINTERFACE_H
