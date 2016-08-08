/**
 * @file Udpv4Protocol.h
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
#ifndef ZIQE_UDPV4PROTOCOL_H
#define ZIQE_UDPV4PROTOCOL_H

#include "NetworkProtocol.h"

namespace Ziqe {

class Udpv4Protocol final : public NetworkProtocol
{
public:
    Udpv4Protocol();

    virtual UniquePointer<NetworkPacket> receivePacket () override;

    virtual UniquePointer<NetworkProtocol> createFromPacket (const NetworkPacket &packet) override;


};

} // namespace Ziqe

#endif // ZIQE_UDPV4PROTOCOL_H
