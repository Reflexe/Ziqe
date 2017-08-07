/**
 * @file LinuxUsbApi.c
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
#include "LinuxUsbApi.h"

#include <linux/usb.h>
#include <linux/module.h>

#include "driver_usb_ids.h"

struct zq_usb_callback {
    ZqLinuxUsbDisconnectCallback onDisconnect;
    ZqLinuxUsbProbeCallback onProbe;
    ZqKernelAddress argument;
};

/* A generic usb device driver  */
struct zq_usb_device_driver {
    struct usb_driver usb_driver;
    struct zq_usb_callback callback;
};
/* Convert a `usb_driver` into a `zq_usb_device_driver` */
#define to_zq_usb_device_driver(device_driver) \
    (container_of (device_driver, struct zq_usb_device_driver, usb_driver))

/* Convert a `usb_interface` into a `usb_driver` and then to a
 * `zq_usb_device_driver` */
#define usb_intf_to_zq_usb_device_driver(intf) \
    (to_zq_usb_device_driver (to_usb_driver (intf->dev.driver)))

/*
   Wrappers around usb_device_driver's callbacks,
   required in order to make sure it will provide an additional argument
   and provide a stable API

   TOCHECK: There would be an way to eliminate this overhead by changing this API
            to take the same prototype as the regular callbacks which
            should be stable. The internal implementation of UsbDeviceManager
            should be private anyway.
*/

static ZqError
forward_probe_callback (struct usb_interface *intf,
                        const struct usb_device_id *id)
{
    struct zq_usb_device_driver *driver = usb_intf_to_zq_usb_device_driver (intf);

    return driver->callback.onProbe (driver->callback.argument, intf);
}

static void
forward_disconnect_callback (struct usb_interface *intf)
{
    struct zq_usb_device_driver *driver = usb_intf_to_zq_usb_device_driver (intf);

    driver->callback.onDisconnect (driver->callback.argument,
                                   intf);
}

ZqLinuxUsbHandle
ZQ_SYMBOL (ZqLinuxUsbRegisterDevice) (const char *name,
                                      ZqLinuxUsbProbeCallback probe_callback,
                                      ZqLinuxUsbDisconnectCallback disconnect_callback,
                                      ZqKernelAddress argument)
{
    struct zq_usb_device_driver *zq_driver = ZQ_SYMBOL (ZqAllocate) (sizeof (struct zq_usb_device_driver));
    struct usb_driver *driver = &zq_driver->usb_driver;
    int res;

    memset (driver, '\0', sizeof (*driver));

    driver->name = name;
    driver->probe = forward_probe_callback;
    driver->disconnect = forward_disconnect_callback;

    zq_driver->callback.onDisconnect = disconnect_callback;
    zq_driver->callback.onProbe = probe_callback;
    zq_driver->callback.argument = argument;

    driver->id_table = driver_usb_table;

    driver->no_dynamic_id = 1;
    driver->disable_hub_initiated_lpm = 1;

    res = usb_register (driver);
    if (res)
        return NULL;

    return (ZqLinuxUsbHandle) driver;
}

void ZQ_SYMBOL (ZqLinuxUsbUnregisterDevice) (ZqLinuxUsbHandle handle) {
    usb_deregister (handle);
    ZQ_SYMBOL (ZqDeallocate) (handle);
}

void ZQ_SYMBOL (ZqLinuxUsbSetInterfaceData) (ZqLinuxUsbInterface interface, ZqKernelAddress data)
{
    usb_set_intfdata (interface, data);
}

ZqKernelAddress ZQ_SYMBOL (ZqLinuxUsbGetInterfaceData) (ZqLinuxUsbInterface interface)
{
    return usb_get_intfdata(interface);
}
