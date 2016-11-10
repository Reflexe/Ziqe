/**
 * @file Thread.hpp
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
#ifndef THREAD_HPP
#define THREAD_HPP

#include "Common/ProcessPeersClient.hpp"

#include "ThreadClient.hpp"

namespace Ziqe {
namespace Host {

class Thread
{
public:
    Thread();

private:
    ProcessPeersClient mProcessPeersClient;

    ThreadClient mThreadClient;
};

} // namespace Host
} // namespace Ziqe

#endif // THREAD_HPP
