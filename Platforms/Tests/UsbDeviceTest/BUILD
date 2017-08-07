load("//Platforms:api.bzl", "zq_driver")
load("//BuildTools:test.bzl", "cc_class_library")


cc_class_library(
    name = 'UsbDeviceTestLib',
    classes = [
        'MyUsbDeviceManager',
        'MyUsbDevice',
       ],
     srcs = ['main.cpp'],

    deps = ['//Platforms:OS'],
)

zq_driver (name='UsbDeviceTestDriver',
           deps=[':UsbDeviceTestLib'],
           usb_ids = [
               "ABCDEFAA",
               "090c1000",
           ],
)
