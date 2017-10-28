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

#include "Utils/FunctionTools.hpp"
#include "Utils/UniquePointer.hpp"

#include "CppCore/Memory.h"
#include "OS/C_API/LinuxUsbApi.h"

#include "UsbDevice.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {

static ZqError onProbe (ZqKernelAddress argument, ZqLinuxUsbInterface interface) {
    auto pDevice = Utils::makeUnique<UsbDevice>();
    auto &device = *pDevice;

    ZQ_SYMBOL(ZqLinuxUsbSetInterfaceData) (interface, pDevice.release ());

    static_cast<UsbDeviceManager*>(argument)->onDeviceAttached(device);

    return ZQ_E_OK;
}

static ZqError onDisconnect (ZqKernelAddress argument, ZqLinuxUsbInterface interface) {
    auto rawDevicePointer = ZQ_SYMBOL(ZqLinuxUsbGetInterfaceData) (interface);
    auto uniqueDevice = Utils::UniquePointer<UsbDevice>{static_cast<UsbDevice*>(rawDevicePointer)};

    static_cast<UsbDeviceManager*>(argument)->onDeviceDetached (*uniqueDevice);

    // Call the device's virtual destructor.
    return ZQ_E_OK;
}

UsbDeviceManager::UsbDeviceManager(const char *name,
                                   const Utils::SharedPointer<DriverContext> &driver) {
    mDriver = ZQ_SYMBOL (ZqLinuxUsbRegisterDevice) (name,
                                                    &onProbe,
                                                    &onDisconnect,
                                                    this);
}

UsbDeviceManager::~UsbDeviceManager()
{
    ZQ_SYMBOL (ZqLinuxUsbUnregisterDevice) (mDriver);
}

void UsbDeviceManager::onDeviceAttached(IDevice &context)
{

}

void UsbDeviceManager::onDeviceDetached(IDevice &context)
{

}

} // namespace OS
ZQ_END_NAMESPACE
