/**
 * @file ThreadOwnerServer.h
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
#ifndef ZIQE_THREADOWNERSERVER_H
#define ZIQE_THREADOWNERSERVER_H

#include "Base/Types.hpp"
#include "Base/LocalThread.hpp"

#include "Protocol/MessageServer.hpp"

namespace Ziqe {

class ThreadOwnerServer
{
public:
    ThreadOwnerServer(Base::UniquePointer<Protocol::MessageServer> &&server);

    void run ();

private:
    Base::UniquePointer<Protocol::MessageServer> mServer;

};
} // namespace Ziqe
#endif // ZIQE_THREADOWNERSERVER_H
