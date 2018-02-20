/**
 * @file UsbDeviceManager.hpp
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
#ifndef USBDEVICEMANAGER_HPP
#define USBDEVICEMANAGER_HPP

#include "Base/Macros.hpp"
#include "Base/Expected.hpp"
#include "Base/ScopedContainer.hpp"

#include "OS/IDeviceManager.hpp"
#include "OS/DriverContext.hpp"
#include "OS/UsbDevice.hpp"

#include "Internal/Context.hpp"

#include <libusb-1.0/libusb.h>

ZQ_BEGIN_NAMESPACE
namespace OS {

class UsbDeviceManager
{
    ZQ_DEFINE_D_FORWARDING_CONSTRUCTOR(UsbDeviceManager)
    ZQ_ALLOW_EXPECTED();

public:
    class ICallback {
    public:
        ZQ_ALLOW_COPY_AND_MOVE (ICallback)
        ICallback() = default;
        virtual ~ICallback();

        virtual void onDeviceAttached(UsbDevice &context) = 0;
        virtual void onDeviceDetached(UsbDevice &context) = 0;
    };


    static Base::Expected<UsbDeviceManager, libusb_error>
            Create (const char *name,
                    const Base::RawPointer<DriverContext> &driver,
                    Base::UniquePointer<ICallback> &&callback);

    ZQ_ALLOW_COPY_AND_MOVE (UsbDeviceManager)

    ~UsbDeviceManager();

private:
    UsbDeviceManager();

    struct HotplugDeleter {
        HotplugDeleter(Base::RawPointer<DriverContext> driverContext)
            : mDriverContext{driverContext}
        {
        }

        void operator () (const libusb_hotplug_callback_handle &handle)
        {
            ::libusb_hotplug_deregister_callback (mDriverContext->getUsbContext(), handle);
        }

        Base::RawPointer<DriverContext> mDriverContext;
    };

    struct Data {
        Base::RawPointer<DriverContext> mDriverContext;
        Base::MovableScopedContainer<libusb_hotplug_callback_handle, HotplugDeleter> mHotplugHandle;
        Base::UniquePointer<ICallback> mCallback;
    } d;
};

} // namespace OS
ZQ_END_NAMESPACE

#endif // USBDEVICEMANAGER_HPP
