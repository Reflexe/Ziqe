load("//Platforms:api.bzl", "zq_library_no_auto_deps")
package(default_visibility = ['//Platforms:__subpackages__'])

zq_library_no_auto_deps (
    name = 'OS',
    classes = [
        'OS/IDevice',
        'OS/IDeviceManager',
        'OS/ILoadableModule',
        'OS/AbstractDriverContext',
    ],
    zq_deps = [
        '//Base:Base',
    ],

    includes = ['.'],
)    

zq_library_no_auto_deps(
    name = 'CppCore',

    hdrs = [
        'CppCore/Macros.h',
    ],

    includes = ['.'],
)

zq_library_no_auto_deps(
    name = 'PerDriver',

    hdrs = [
        'PerDriver/EntryPoints.hpp',
        'PerDriver/Macros.h',
    ],

    includes = ['.'],
)

exports_files(["PerDriver/in/config.h"])
