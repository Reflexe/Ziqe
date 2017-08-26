/**
 * @file MyUsbDeviceManager.hpp
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
#ifndef MYUSBDEVICEMANAGER_HPP
#define MYUSBDEVICEMANAGER_HPP

#include "OS/UsbDeviceManager.hpp"

class MyUsbDeviceManager : public Ziqe::OS::UsbDeviceManager
{
public:
    MyUsbDeviceManager(const Ziqe::Utils::SharedPointer<Ziqe::OS::DriverContext> &driver);

    virtual void onDeviceAttached (Ziqe::OS::IDevice &context) final;
    virtual void onDeviceDetached (Ziqe::OS::IDevice &context) final;
};

#endif // MYUSBDEVICEMANAGER_HPP