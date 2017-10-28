load ('//Platforms/Linux:repo_api.bzl', 'zq_linux_config')
load ('//Platforms/GenericUsermode:repo_api.bzl', 'zq_generic_config')

platform_functions = {
    'Linux': {
        'zq_config'  : zq_linux_config,
    },
    'GenericUsermode': {
        'zq_config': zq_generic_config,
    },
}
