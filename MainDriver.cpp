/**
 * @file MainDriver
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

#include "OS/IDevice.hpp"
#include "OS/IDeviceManager.hpp"

#include "ZqAPI/EntryPoints.h"

#include "Utils/Vector.hpp"

ZQ_BEGIN_NAMESPACE

class MyTestDevice : public OS::IDevice
{
public:
    MyTestDevice()
    {
    }

    ~MyTestDevice()
    {
    }
};

/**
 * A device manager should be able to be registered as a even receiver
 * of some sort of devices.
 *
 * For example:
 *  A device manager that receives all of the USB devices
 *  before all the other drivers.
 */
class MyTestDeviceManager : public OS::IDeviceManager
{
public:
    /**
     * @brief
     * @param device
     */
    virtual void onDeviceAttached(OS::IDevice &device) override;

    virtual void onDeviceDetached(OS::IDevice &device) override;

private:
    /**
     * @brief Currently attached devices.
     */
    Utils::Vector<Utils::RawPointer<OS::IDevice>> mAttchedDevices;
};

void MyTestDeviceManager::onDeviceAttached(OS::IDevice &device) {
    ZQ_ASSERT (device.getDeviceClass() == OS::IDevice::Class::Usb);
}

void MyTestDeviceManager::onDeviceDetached(OS::IDevice &device)
{

}

ZQ_END_NAMESPACE

struct DriverContext {
    Ziqe::MyTestDeviceManager deviceManager;
};

void ZQ_SYMBOL(ZqOnLoad) (void *private_data_ptr) {
    using namespace Ziqe;

    auto pPrivateData = static_cast<DriverContext**>(private_data_ptr);
    *pPrivateData = new DriverContext;

    // Here should be a registerManager or something.
}

void ZQ_SYMBOL (ZqOnUnload) (void *private_data_ptr) {
    using namespace Ziqe;

    auto pPrivateData = static_cast<DriverContext**>(private_data_ptr);

    delete *pPrivateData;
}
