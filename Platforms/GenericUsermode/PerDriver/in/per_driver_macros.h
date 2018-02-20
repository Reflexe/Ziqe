/**
 * @file per_driver_macros.h
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
#ifndef PER_DRIVER_MACROS_H
#define PER_DRIVER_MACROS_H

#define ZQ_GENERIC_PER_DRIVER_USB_PRODUCT_ID static_cast<uint16_t>( %{USB_PRODUCT_ID} )
#define ZQ_GENERIC_PER_DRIVER_USB_VENDOR_ID  static_cast<uint16_t>( %{USB_VENDOR_ID} )

#endif // PER_DRIVER_MACROS_H
