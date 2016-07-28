/**
 * @file NetworkPacket.h
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
#ifndef ZIQE_NETWORKPACKET_H
#define ZIQE_NETWORKPACKET_H

#include "Base/SharedVector.h"
#include "Base/Types.h"

namespace Ziqe {

class NetworkPacket
{
public:
    NetworkPacket();
    virtual ~NetworkPacket() = 0;

    virtual const SharedVector<Byte> getData () const = 0;
};

} // namespace Ziqe

#endif // ZIQE_NETWORKPACKET_H
