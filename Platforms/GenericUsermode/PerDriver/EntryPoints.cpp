/**
 * @file EntryPoints.c
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
#include "PerDriver/EntryPoints.hpp"

#include "OS/AbstractDriverContext.hpp"

int main(int argc, char *argv[]) {
    using namespace Ziqe;

    auto maybeDriverContext = OS::DriverContext::Create ();
    if (! maybeDriverContext)
        return maybeDriverContext.getError ();

    ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnLoad) (&maybeDriverContext.get ());

     while (true) {
          libusb_handle_events(maybeDriverContext.get ().getUsbContext());
     }

    ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnUnload) (&maybeDriverContext.get ());

    return 0;
}
