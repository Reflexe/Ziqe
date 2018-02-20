load('//BuildTools:cc_class_library.bzl', 'cc_class_library')
load('//BuildTools:driver_functions.bzl', 'generate_per_driver_file')
load('//Platforms:BuildTools/PlatformTargets.bzl', 'get_target_per_platform')

PLATFORM_NAME = 'GenericUsermode'

obj_name_to_constructor = {
    'list': list,
    'int': int,
    'dict': dict,
    'None': None,
    'bool': bool,
    'depset': depset,
    'struct': struct
}

def create_new_val(**args):
    value = args.get('value')
    val = args.get('value_type')
    constructor = obj_name_to_constructor[type(value)]

    if value == None:
        return constructor()
    else:
        return constructor(value)

def copy(val):
    return create_new_val(value=val, value_type=val)

def copy_setdefault(d, key, default=None):
    result = d.setdefault(key, default)

    if result == None:
        return result
    else:
        d[key] = copy(result)
        return d[key]

def zq_generic_driver(**args):
    ids = args.pop('usb_ids', ['deadbeef'])

    # TODO: support multiple usb ids.
    result_dict = generate_per_driver_file(
        driver_name = args['name'] + '_per_driver',
        template    = '//Platforms/GenericUsermode:PerDriver/in/per_driver_macros.h',
        substitutions = {
            '%{USB_VENDOR_ID}' : '0x' + ids[0][0:4],
            '%{USB_PRODUCT_ID}': '0x' + ids[0][4:8],
        }
    )

    native.cc_binary(
        name=args['name'],
        srcs = ['//Platforms/GenericUsermode:main_driver_srcs' ,'//Platforms/GenericUsermode:per_driver_srcs',
                result_dict['output_label']]
                + args.get('srcs', []),
        includes = [result_dict['output_path']] + args.get('includes', []),
        deps = args.get('deps', []),
        copts = ['-std=c++14'],
    )

def zq_generic_library(**args):
    cc_class_library(linkstatic=1, **args)


def zq_generic_per_driver(**args):
    #for usb_id in args.pop('usb_ids', ()):
        #usb_vendor = usb_id[0:4]
        #usb_product = usb_id[4:8]

        #vendor_and_product_arr.append ('0x' + usb_vendor + ', ' + '0x' + usb_product)


    #output = ('{ {' + '}, {'.join(vendor_and_product_arr) + '} }')

    zq_generic_library(name=args['name'])


generic_functions = {
    'zq_library': zq_generic_library,
    'zq_driver' : zq_generic_driver,
    'zq_per_driver': zq_generic_per_driver,
}
