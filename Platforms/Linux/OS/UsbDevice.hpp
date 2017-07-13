/**
 * @file UsbDevice.hpp
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
#ifndef USBDEVICE_HPP
#define USBDEVICE_HPP

#include "Utils/Macros.hpp"
#include "Utils/Vector.hpp"
#include "Utils/UniquePointer.hpp"

#include "OS/IDevice.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {

inline namespace _Linux {

}

class UsbDevice : public IDevice
{
public:
    UsbDevice(void*)
    {

    }

//    class UsbPipe
//    {
//        class Callback {

//        };

//    private:

//    };



//    UsbPipe createControlPipe (UsbPipe::Callback &call);
private:

};

} // namespace OS
ZQ_END_NAMESPACE

#endif // USBDEVICE_HPP
