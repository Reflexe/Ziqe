/**
 * @file MyUsbDeviceManager.cpp
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
#include "MyUsbDeviceManager.hpp"

#include "Utils/Logger.hpp"

MyUsbDeviceManager::MyUsbDeviceManager(const Ziqe::Utils::SharedPointer<Ziqe::OS::DriverContext> &driver)
    : UsbDeviceManager{"MyUsbDeviceManager", driver}
{
    ZQ_LOG ("MyUsbDeviceManager");
}

void MyUsbDeviceManager::onDeviceAttached(Ziqe::OS::IDevice &context)
{
    ZQ_LOG ("onDeviceAttached");
}

void MyUsbDeviceManager::onDeviceDetached(Ziqe::OS::IDevice &context)
{
    ZQ_LOG ("onDeviceDetached");
}
