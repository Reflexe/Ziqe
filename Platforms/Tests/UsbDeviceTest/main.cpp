/**
 * @file main.cpp
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

#include "PerDriver/EntryPoints.hpp"
#include "OS/AbstractDriverContext.hpp"
#include "MyUsbDeviceManager.hpp"

struct Data : Ziqe::OS::DriverContext::IDestructable
{
    Data(Ziqe::OS::UsbDeviceManager &&manager)
        : deviceManager{Ziqe::Base::move(manager)}
    {
        ZQ_LOG("Data\n");
    }

    ~Data() override
    {
        ZQ_LOG("~Data\n");
    }

    Ziqe::OS::UsbDeviceManager deviceManager;
};

ZQ_BEGIN_C_DECL
void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnLoad) (Ziqe::Base::RawPointer<Ziqe::OS::DriverContext> pointer)
{
    using namespace Ziqe;

    // TODO: allow expected to have a costom allocator
    // to allocate the var in place instead of moving it again and again.
    auto maybeMyDeviceManager = MyUsbDeviceManager::Create ();
    if (! maybeMyDeviceManager) {
        ZQ_WARNING("Couldn't initialize my usb device manager!\n");
        return;
    }

    auto maybeUsbDeviceManager = Ziqe::OS::UsbDeviceManager::Create ("MyUsbDeviceTest ",
                                                                     pointer,
                                                                     Ziqe::Base::makeUnique<MyUsbDeviceManager>(Ziqe::Base::move(maybeMyDeviceManager.get())));
    if (! maybeUsbDeviceManager) {
        ZQ_WARNING("Couldn't initialize usb device manager!\n");
        return;
    }

   pointer->setUserData (Base::UniquePointer<OS::DriverContext::IDestructable>{new Data(Base::move(maybeUsbDeviceManager.get ()))});
}

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnUnload) (Ziqe::Base::RawPointer<Ziqe::OS::DriverContext> pointer)
{
    ZQ_LOG("Unload\n");
}

ZQ_END_C_DECL
