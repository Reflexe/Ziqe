load('//Platforms/Linux:api.bzl', 'linux_functions')
load ('//Platforms/GenericUsermode:api.bzl', 'generic_functions')

platform_functions = {
    'Linux': linux_functions,
    'GenericUsermode': generic_functions,
}
