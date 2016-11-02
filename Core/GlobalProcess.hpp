/**
 * @file GlobalProcess.hpp
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
#ifndef ZIQE_GLOBALPROCESS_H
#define ZIQE_GLOBALPROCESS_H

#include "Base/Types.hpp"
#include "Base/LinkedList.hpp"
#include "Base/RWLock.hpp"

#include "Core/MemoryRevisionTree.hpp"

#include "Core/ProcessPeersServer.hpp"

namespace Ziqe {

class GlobalProcess
{
public:
    GlobalProcess();

private:
    Base::SharedPointer<ProcessPeersServer> mProcessServer;


};

} // namespace Ziqe

#endif // ZIQE_GLOBALPROCESS_H
