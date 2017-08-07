load("//BuildTools:test.bzl", "cc_class_library")
package(default_visibility = ['//Platforms:__subpackages__'])

cc_class_library (
    name = 'OS',
    classes = [
        'OS/IDevice',
        'OS/IDeviceManager',
        'OS/ILoadableModule',
    ],
    deps = [
        '//Utils:Utils',
    ],

    includes = ['.'],
)    

cc_class_library(
    name = 'ZqAPI',

    hdrs = [
        'ZqAPI/Macros.h',
        'ZqAPI/EntryPoints.h',
    ],

    includes = ['.'],
)
