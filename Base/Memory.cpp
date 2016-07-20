/**
 * @file Memory.cpp
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
#include "Memory.h"

#include "ZiqeAPI/Memory.h"

namespace Ziqe {

} // namespace Ziqe

// TODO: what to do on std::bad_alloc with no exceptions?
void *operator new(std::size_t count) {
    static_assert (sizeof (ZqRegisterType) >= sizeof (void *),
                   "Invalid ZqRegisterType");
    return reinterpret_cast<void*>(ZqAllocate (static_cast<ZqRegisterType>(count)));
}

void operator delete(void *pointer) noexcept
{
    ZqDeallocate (reinterpret_cast<ZqRegisterType>(pointer));
}
