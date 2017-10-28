load(':Common/api.bzl', 'common_per_driver_config')
load('//BuildTools:platform_settings.bzl', 'platform_to_platform_setting', 'get_label_for_platform')
load(':PlatformFunctions.bzl', 'platform_functions')

_TARGET_PER_PLATFORM_NAME_FORMAT = "{name}_{platform}"

# Platform deps: //Platforms/$platform:$target
def create_target_per_platform(platform_function_key, **args):
    args = dict(args)
    args_name = args['name']

    #platform_condition_to_target = {
        #'//conditions:default': args_name + '_Linux'
    #}



    for platform_name, platform_function in platform_functions.items():
        temp_args = dict(args)

        temp_args['name'] = _TARGET_PER_PLATFORM_NAME_FORMAT.format(name=args_name, platform=platform_name)

        ## Build a select() dict (platform -> platform_zq_driver_target)
        #platform_condition = '//Platforms:' + platform_to_platform_setting(platform_name)
        #platform_condition_to_target[platform_condition] = args['name']

        temp_args.setdefault('zq_deps', [])
        for target_name in temp_args.get('platform_deps', []):
            temp_args['zq_deps'] = (temp_args['zq_deps'] + [get_label_for_platform(platform=platform_name, target=target_name)])

        temp_args.pop('platform_deps', None)

        temp_args['deps'] = temp_args.setdefault('deps', [])
        for label_without_platform in temp_args.get('zq_deps', []):
            temp_args['deps'] = (temp_args['deps'] + [_TARGET_PER_PLATFORM_NAME_FORMAT.format(name=label_without_platform, platform=platform_name)])

        temp_args.pop('zq_deps', None)

        # Create this platform's target.
        platform_function[platform_function_key] (**temp_args)

    #native.alias(
        #name = args_name,
        #visibility = args.get('visibility', None),
        #actual = select(platform_condition_to_target),
    #)


def zq_library_no_auto_deps(**args):
    create_target_per_platform ('zq_library', **args)

def zq_library(**args):
    args['platform_deps'] = ['OS', 'CppCore', 'PerDriver']
    zq_library_no_auto_deps(**args)


def zq_driver(**args):
    args = dict(args)

    args_name = args['name']
    per_driver_config = common_per_driver_config(name=args_name)

    if 'srcs' in args or 'classes' in args:
        cc_per_driver_library = args_name + '_cc_lib'

        zq_library(
            name = cc_per_driver_library,
            srcs = args.get('srcs'),
            hdrs = [per_driver_config['target_name']],
            includes = [per_driver_config['output_path']],
            classes = args.get('classes'),
            zq_deps = args.get('zq_deps', []) + ['//Utils:Utils'],
        )

        args.pop('srcs', None)
        args.pop('classes', None)

    args['includes'] = args.get('includes', []) + [per_driver_config['target_name']]
    args['zq_deps']  = args.get('zq_deps', []) + [cc_per_driver_library]

    create_target_per_platform('zq_driver', **args)
