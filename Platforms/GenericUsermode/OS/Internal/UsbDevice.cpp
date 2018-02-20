/**
 * @file UsbDevice.cpp
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
#include "UsbDevice.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {
namespace Internal {
namespace LibUsb {

Base::Expected<UsbDevice, libusb_error> UsbDevice::Create(libusb_device *usb_device) {
    libusb_device_handle *handle;

    auto rt = libusb_open (usb_device, &handle);
    if (rt != LIBUSB_SUCCESS)
        return {Base::Error(static_cast<libusb_error> (rt))};

    return {Base::makeDataInit(Base::TakeUniquePointer(handle))};
}

} // namespace LibUsb
} // namespace Internal
} // namespace OS
ZQ_END_NAMESPACE
