# The Ziqe Usb Stack

## General Skeleton
The manager:
    UsbDeviceManager(usb_dev_id):
        get notifications for new devices.

Device:
    UsbInterface findInterface()
    setConfiguration
    reset
    eject
    ...

Inteface:
    UsbOutEndPoint findOutEndPoint()
    UsbInEndPoint  findInEndPoint()

UsbInEndPoint:
    read(): sync IN

    readAsync(.., Callback): async IN

    Callback:
        onDataReceived()

UsbOutEndPoint:
    write(): sync out

    writeAsync(..., Callback): async OUT

    Callback:
        onDataWritten()

Issues:
    * Device claiming?
        There should be a way in the manager to issue a claim or not.
        That would make the manager callback block.
        Implementation?

        Maybe just eject the device or unclaim it when it doesn't meet our requirments?
        I think that would be the way.

    * Buffer inteface.


# Technical details
    To claim a usb id, use the usb_ids argument for the zq_driver function
    example:
        ```
        zq_driver(name='Nothing',
                  usb_ids = [
                    'DEADBEEF',
                    'DDDDEEEE'
                  ])
        ```

    That allows each platform to implements its way of knowing
    that your driver is intersested in these device IDs.
