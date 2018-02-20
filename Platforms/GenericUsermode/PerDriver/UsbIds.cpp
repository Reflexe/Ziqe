/**
 * @file UsbIds.cpp
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
#include "UsbIds.hpp"

#include "Base/Macros.hpp"

#include "per_driver_macros.generated.h"

ZQ_BEGIN_NAMESPACE

namespace GenericUserspace {
namespace PerDriver {
namespace Usb {

static constexpr ZqUsbID g_usb_ids[] = {
    {  ZQ_GENERIC_PER_DRIVER_USB_VENDOR_ID,
      ZQ_GENERIC_PER_DRIVER_USB_PRODUCT_ID}
};


Base::RawArray<const ZqUsbID> GetUsbIds()
{
    return {g_usb_ids, ZQ_ARRAY_LENGTH (g_usb_ids)};
}

}
}
}

ZQ_END_NAMESPACE
