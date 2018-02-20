/**
 * @file UsbDevice.hpp
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
#ifndef OS_USBDEVICE_HPP
#define OS_USBDEVICE_HPP

#include "Base/Macros.hpp"
#include "Base/Expected.hpp"

#include "OS/IDevice.hpp"

#include "Internal/UsbDevice.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {

class UsbDevice : public IDeviceConnection
{
    ZQ_DEFINE_D_FORWARDING_CONSTRUCTOR (UsbDevice)
    ZQ_ALLOW_EXPECTED ();

public:
    static Base::Expected<UsbDevice, libusb_error>
            Create (libusb_device *device);

private:
    struct Data {
        Internal::LibUsb::UsbDevice mUsbDevice;
    }d;

};

} // namespace OS
ZQ_END_NAMESPACE

#endif // OS_USBDEVICE_HPP
