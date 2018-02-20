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
#ifndef INTERNAL_USBDEVICE_HPP
#define INTERNAL_USBDEVICE_HPP

#include "Base/ZQObject.hpp"
#include "Base/UniquePointer.hpp"
#include "Base/Expected.hpp"

#include <libusb-1.0/libusb.h>

ZQ_BEGIN_NAMESPACE
namespace OS {
namespace Internal {
namespace LibUsb {

class UsbDevice
{
    ZQ_DEFINE_D_FORWARDING_CONSTRUCTOR (UsbDevice)
    ZQ_ALLOW_EXPECTED();

    struct Deleter {
        void operator() (libusb_device_handle *dev) {
            libusb_close (dev);
        }
    };

public:
    static Base::Expected<UsbDevice, libusb_error>
            Create(libusb_device *usb_device);

private:
    struct Data {
        Base::UniquePointer<libusb_device_handle, Deleter> mDevice;
    } d;

};

} // namespace LibUsb
} // namespace Internal
} // namespace OS
ZQ_END_NAMESPACE

#endif // INTERNAL_USBDEVICE_HPP
