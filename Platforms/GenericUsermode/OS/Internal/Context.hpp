/**
 * @file Context.hpp
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
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Base/ZQObject.hpp"
#include "Base/Expected.hpp"
#include "Base/UniquePointer.hpp"

#include <libusb-1.0/libusb.h>

ZQ_BEGIN_NAMESPACE
namespace OS {
namespace Internal {
namespace LibUsb {

class Context
{
    ZQ_DEFINE_D_FORWARDING_CONSTRUCTOR(Context)

    struct Deleter {
        void operator() (libusb_context *context)
        {
            ::libusb_exit(context);
        }
    };

    ZQ_ALLOW_EXPECTED();

public:
    ZQ_ALLOW_MOVE (Context)
    ZQ_DISALLOW_COPY (Context)

    operator libusb_context* ()
    {
        return d.mContext.get ();
    }

    static Base::Expected<Context, libusb_error>
            Create();

    struct Data {
        Base::UniquePointer<libusb_context, Deleter> mContext;
    } d;
};

} // namespace LibUsb
} // namespace Internal
} // namespace OS
ZQ_END_NAMESPACE

#endif // CONTEXT_HPP
