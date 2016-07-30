/**
 * @file PeerConnection.h
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
#ifndef ZIQE_PEERCONNECTION_H
#define ZIQE_PEERCONNECTION_H


namespace Ziqe {

class PeerConnectionCallback
{
public:
    virtual ~PeerConnectionCallback () = 0;

    virtual void onGivePage () = 0;

    virtual void onTakePage () = 0;

    virtual void on () = 0;

    virtual void onMemoryRequest () = 0;

    virtual void onConnectionClosed () = 0;
};

class PeerConnection
{
public:
    PeerConnection();
};

} // namespace Ziqe

#endif // ZIQE_PEERCONNECTION_H
