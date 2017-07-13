/**
 * @file IDeviceManager.hpp
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
#ifndef IDEVICEMANAGER_HPP
#define IDEVICEMANAGER_HPP

#include "Utils/SharedPointer.hpp"

#include "OS/IDevice.hpp"
#include "OS/DriverContext.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {

class IDeviceManager
{
public:
    IDeviceManager();
    virtual ~IDeviceManager();

    virtual void onDeviceAttached (Utils::UniquePointer<IDevice> &device) = 0;
    virtual void onDeviceDetached (Utils::UniquePointer<IDevice> &device) = 0;
};

} // namespace OS
ZQ_END_NAMESPACE

#endif // IDEVICEMANAGER_HPP
