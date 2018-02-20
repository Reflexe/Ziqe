/**
 * @file CppEntryPoints.cpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2017 Shmuel Hazan
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

#include "Base/Macros.hpp"
#include "Base/SharedPointer.hpp"

#include "PerDriver/EntryPoints.hpp"

#include "OS/AbstractDriverContext.hpp"

ZQ_BEGIN_C_DECL

int ZQ_PER_DRIVER_UNIQUE_SYMBOL(CppForwardOnLoad) (void *ptr) {
    using namespace Ziqe;

    auto maybeDriverContext = OS::DriverContext::Create ();
    if (! maybeDriverContext)
        return maybeDriverContext.getError ();

    auto driverContext = new OS::DriverContext(Base::move(maybeDriverContext.get ()));
    *static_cast<OS::DriverContext**>(ptr) = driverContext;

    ZQ_PER_DRIVER_UNIQUE_SYMBOL (ZqOnLoad) (driverContext);

    return 0;

}

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(CppForwardOnUnload) (void *ptr) {
    using namespace Ziqe;

    OS::DriverContext* pointer = *static_cast<OS::DriverContext**>(ptr);

    ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnUnload) (pointer);

    //pointer->setUserData ({});
    delete pointer;
}

ZQ_END_C_DECL
