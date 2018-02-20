/**
 * @file UsbDeviceManager.cpp
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
#include "UsbDeviceManager.hpp"

#include "PerDriver/UsbIds.hpp"
#include "UsbDevice.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {

int libusb_callback (struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data)
{
    auto &callback = *static_cast<UsbDeviceManager::ICallback*>(user_data);

    auto maybeUsbDevice = UsbDevice::Create (dev);
    if (! maybeUsbDevice) {
        ZQ_WARNING("Couldn't open a usb device");
        Base::Logger::logWarning (libusb_strerror(maybeUsbDevice.getError ()));

        return maybeUsbDevice.getError ();
    }

    if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
        callback.onDeviceAttached (maybeUsbDevice.get ());

    } else {
        callback.onDeviceDetached (maybeUsbDevice.get ());
    }

    Base::IgnoreUnused(ctx);

    return LIBUSB_SUCCESS;
}

Base::Expected<UsbDeviceManager, libusb_error>
        UsbDeviceManager::Create(const char *name,
                                 const Base::RawPointer<DriverContext> &driver,
                                 Base::UniquePointer<UsbDeviceManager::ICallback> &&callback)
{
    libusb_hotplug_callback_handle handle;

    auto id = GenericUserspace::PerDriver::Usb::GetUsbIds ();
    auto rv = ::libusb_hotplug_register_callback (driver->getUsbContext(),
                                                  static_cast<libusb_hotplug_event>(
                                                      LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED
                                                    | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
                                                  LIBUSB_HOTPLUG_NO_FLAGS,
                                                  id[0].vendor,
            id[0].product,
            LIBUSB_HOTPLUG_MATCH_ANY,
            &libusb_callback,
            callback.get (),
            &handle);

    if (rv)
        return {Base::Error(static_cast<libusb_error> (rv))};

    decltype(Data::mHotplugHandle) scopedHandle{Base::move(handle), HotplugDeleter{driver}};

    return {Base::makeDataInit(Base::move(driver), Base::move(scopedHandle), Base::move(callback))};
}

UsbDeviceManager::~UsbDeviceManager ()
{
}

UsbDeviceManager::ICallback::~ICallback()
{
}

} // namespace OS
ZQ_END_NAMESPACE
