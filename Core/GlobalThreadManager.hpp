/**
 * @file GlobalThreadManager.hpp
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
#ifndef ZIQE_GLOBALTHREADMANAGER_H
#define ZIQE_GLOBALTHREADMANAGER_H

#include "Base/HashTable.hpp"

#include "Core/GlobalThread.hpp"
#include "Base/LocalThread.hpp"

namespace Ziqe {

class GlobalThreadManager
{
public:
    GlobalThreadManager();

    GlobalThread &getGlobalThread ();

    // Can return nullptr.
    Base::RawPointer<GlobalThread> localToGlobalThread (const LocalThread &localThread);

    template<class ...Args>
    Base::RawPointer<GlobalThread> addThread (LocalThread::ThreadID threadID, Args &&... args) {
        auto pair = mThreads.insert (threadID, std::forward<Args> (args)...);

        if (pair.first == true)
            return &(pair.second->second);
        else
            return nullptr;
    }

private:
    Base::HashTable<LocalThread::ThreadID, GlobalThread> mThreads;

};

} // namespace Ziqe

#endif // ZIQE_GLOBALTHREADMANAGER_H
