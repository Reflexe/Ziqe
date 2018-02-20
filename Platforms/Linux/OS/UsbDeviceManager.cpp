/**
 * @file UsbDeviceManager.cpp
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
#include "UsbDeviceManager.hpp"

#include "OS/UsbDevice.hpp"

#include "Base/FunctionTools.hpp"
#include "Base/UniquePointer.hpp"

#include "CppCore/Memory.h"

ZQ_BEGIN_NAMESPACE
namespace OS {

static ZqError onProbe (ZqKernelAddress argument, ZqLinuxUsbInterface interface) {
    auto pDevice = Base::makeUnique<UsbDevice>();
    auto &device = *pDevice;

    ZQ_SYMBOL(ZqLinuxUsbSetInterfaceData) (interface, pDevice.release ());

    static_cast<UsbDeviceManager::ICallback*>(argument)->onDeviceAttached(device);

    return ZQ_E_OK;
}

static ZqError onDisconnect (ZqKernelAddress argument, ZqLinuxUsbInterface interface) {
    auto rawDevicePointer = ZQ_SYMBOL(ZqLinuxUsbGetInterfaceData) (interface);
    auto uniqueDevice = Base::UniquePointer<UsbDevice>{static_cast<UsbDevice*>(rawDevicePointer)};

    static_cast<UsbDeviceManager::ICallback*>(argument)->onDeviceDetached (*uniqueDevice);

    // Call the device's virtual destructor.
    return ZQ_E_OK;
}

Base::Expected<UsbDeviceManager, int>
        UsbDeviceManager::Create(const char *name,
                                 const Base::RawPointer<DriverContext> &driver,
                                 Base::UniquePointer<ICallback> &&callback)
{
    Base::IgnoreUnused(driver);

    auto device = ZQ_SYMBOL (ZqLinuxUsbRegisterDevice) (name,
                                                        &onProbe,
                                                        &onDisconnect,
                                                        callback.get ());
   
    return {Base::makeDataInit(Base::move(device), Base::move(callback))};
}

UsbDeviceManager::~UsbDeviceManager()
{
    ZQ_LOG("Linux ~UsbDeviceManager\n");
}


} // namespace OS
ZQ_END_NAMESPACE
