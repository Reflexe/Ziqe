load("//BuildTools:test.bzl", "cc_class_library")

cc_class_library (
    name = 'OS',
    classes = [
        'OS/IDevice',
        'OS/IDeviceManager',
        'OS/ILoadableModule',
    ],
    visibility = ['//Platforms:__subpackages__'],
)    
cc_class_library(
    name = 'ZqAPI',

    hdrs = [
        'ZqAPI/Macros.h',
        'ZqAPI/EntryPoints.h'
    ],
    visibility = ['//Platforms:__subpackages__'],

    includes = ['.'],
)

