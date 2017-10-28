load('//BuildTools:cc_class_library.bzl', 'cc_class_library')

def zq_generic_driver(**args):
    args.pop('usb_ids', None)

    args['srcs'] = args.get('srcs', []) + ['//Platforms/GenericUsermode:per_driver_srcs']
    args['data'] = args.get('data', []) + ['//Platforms/GenericUsermode:per_driver_srcs']

    native.cc_binary(**args)

def zq_generic_library(**args):
    cc_class_library(linkstatic=1, **args)
