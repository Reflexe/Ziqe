/**
 * @file UsbDeviceManager.hpp
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
#ifndef USBDEVICEMANAGER_HPP
#define USBDEVICEMANAGER_HPP

#include "OS/Memory.hpp"

#include "OS/IDeviceManager.hpp"
#include "OS/DriverContext.hpp"
#include "OS/UsbDevice.hpp"

#include "Base/Vector.hpp"
#include "Base/Expected.hpp"
#include "Base/ZQObject.hpp"
#include "Base/ScopedContainer.hpp"

#include "PerDriver/C_API/LinuxUsbApi.h"

ZQ_BEGIN_NAMESPACE
namespace OS {

/**
 * @brief The UsbDeviceManager class
 *
 * Terminology:
 *  IDevice: An active connection between a computer and a device.
 *  IDeviceManager: An event observer for devices connections.
 *
 * MAYBE: rename IDevice to IDeviceConnection / IActiveDevice.
 */
class UsbDeviceManager
{
    ZQ_DEFINE_D_FORWARDING_CONSTRUCTOR(UsbDeviceManager)
    ZQ_ALLOW_EXPECTED();
public:
    ZQ_ALLOW_MOVE(UsbDeviceManager)

    class ICallback {
    public:
        ICallback() = default;

        virtual void onDeviceAttached(UsbDevice &context) = 0;
        virtual void onDeviceDetached(UsbDevice &context) = 0;
    };

    static Base::Expected<UsbDeviceManager, int>
            Create(const char *name,
                   const Base::RawPointer<DriverContext> &driver,
                   Base::UniquePointer<ICallback> &&callback);

    ~UsbDeviceManager();

private:
    UsbDeviceManager(const char *name,
                     const Base::RawPointer<DriverContext> &driver);

    struct DriverDeleter {
        void operator() (ZqKernelAddress address) {
            if (address != nullptr)
                ZQ_SYMBOL (ZqLinuxUsbUnregisterDevice) (address);
        }
    };

    struct Data {
        Base::MovableScopedContainer<ZqKernelAddress, DriverDeleter> mDriver;

        Base::UniquePointer<ICallback> mCallback;
    }d;
};

} // namespace OS
ZQ_END_NAMESPACE

#endif // USBDEVICEMANAGER_HPP
