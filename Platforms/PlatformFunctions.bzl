load('//Platforms/Linux:api.bzl', 'zq_linux_driver', 'zq_linux_library')
load('//Platforms/Linux:repo_api.bzl', 'zq_linux_config')

load('//Platforms/GenericUsermode:api.bzl', 'zq_generic_driver', 'zq_generic_library')
load('//Platforms/GenericUsermode:repo_api.bzl', 'zq_generic_config')


platform_functions = {
    'Linux': {
        'zq_driver'  : zq_linux_driver,
        'zq_library' : zq_linux_library,
    },
    'GenericUsermode': {
        'zq_driver' :  zq_generic_driver,
        'zq_library':  zq_generic_library,
    },
}
