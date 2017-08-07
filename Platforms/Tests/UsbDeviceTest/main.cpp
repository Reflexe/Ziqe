/**
 * @file main.cpp
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

#include "ZqAPI/EntryPoints.h"
#include "MyUsbDeviceManager.hpp"

struct Data
{
    MyUsbDeviceManager device;
};

void ZQ_SYMBOL(ZqOnLoad) (void *private_data_ptr)
{
    *static_cast<Data**>(private_data_ptr) = new Data{{Ziqe::Utils::makeShared<Ziqe::OS::DriverContext>()}};
}

void ZQ_SYMBOL(ZqOnUnload) (void *private_data_ptr)
{
    delete *static_cast<Data**>(private_data_ptr);
}
