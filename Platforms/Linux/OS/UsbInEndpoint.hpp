/**
 * @file UsbInEndpoint.hpp
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
#ifndef USBINENDPOINT_HPP
#define USBINENDPOINT_HPP

#include "Base/Macros.hpp"
#include "Base/Expected.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {

class UsbInEndpoint
{
public:
    typedef int Reference;
    typedef int Address;

    class Callback {
        Callback()
        {
        }

        ZQ_ALLOW_COPY_AND_MOVE (Callback)
        virtual ~Callback();

        virtual void onDataReceived (UsbInEndpoint &endpoint) = 0;

    private:
        Reference mEndpoint;
    };

    UsbInEndpoint(Address address);
};

} // namespace OS
ZQ_END_NAMESPACE

#endif // USBINENDPOINT_HPP
