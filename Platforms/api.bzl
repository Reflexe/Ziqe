load(':Common/api.bzl', 'common_per_driver_config')
load('//BuildTools:platform_settings.bzl', 'platform_to_platform_setting', 'get_label_for_platform')
load(':PlatformFunctions.bzl', 'platform_functions')
load(':BuildTools/PlatformTargets.bzl', 'get_target_per_platform')

def target_for_platform(platform_function_key, platform_name, **args):
    temp_args = dict(args)

    temp_args['name'] = get_target_per_platform(name=args['name'], platform=platform_name)

    temp_args.setdefault('zq_deps', [])
    for target_name in temp_args.get('platform_deps', []):
        temp_args['zq_deps'] = temp_args['zq_deps'] + [get_label_for_platform(platform=platform_name, target=target_name)]

    temp_args.pop('platform_deps', None)

    temp_args['deps'] = temp_args.setdefault('deps', [])
    for label_without_platform in temp_args.get('zq_deps', []):
        temp_args['deps'] = temp_args['deps'] + [get_target_per_platform(name=label_without_platform, platform=platform_name)]

    temp_args.pop('zq_deps', None)

    # Create this platform's target.
    function = platform_functions[platform_name].get(platform_function_key)
    if function == None:
        print('Platform {} does\'t have platform function {}. Skipping.'.format(platform_name,
                                                                                platform_function_key))
    else:
        function(**temp_args)

# Platform deps: //Platforms/$platform:$target
def create_target_per_every_platform(platform_function_key, **args):
    args = dict(args)

    for platform_name, platform_function in platform_functions.items():
        target_for_platform(platform_function_key, platform_name, **args)


def zq_library_for_platform (platform_name, **args):
    target_for_platform('zq_library', platform_name, **args)

def zq_driver_for_platform (platform_name, **args):
    target_for_platform('zq_driver', platform_name, **args)

def zq_library_no_auto_deps(**args):
    create_target_per_every_platform ('zq_library', **args)

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
            zq_deps = args.get('zq_deps', []) + ['//Base:Base'],
        )


        per_driver_target_name = args_name + '_per_driver'
        create_target_per_every_platform(
            'zq_per_driver',
            name=per_driver_target_name,
            hdrs = [per_driver_config['target_name']],
            includes = [per_driver_config['output_path']],
            zq_deps = [cc_per_driver_library] + args.get('zq_deps', []) + ['//Base:Base'],
            # TODO: just use Common:PerDriver?
            platform_deps = ['OS', 'PerDriver'],

            driver_args = args,
        )

        args.pop('srcs', None)
        args.pop('classes', None)

    args['includes'] = args.get('includes', []) + [per_driver_config['target_name']]
    args['zq_deps']  = [per_driver_target_name, cc_per_driver_library] + args.get('zq_deps', [])

    create_target_per_every_platform('zq_driver', **args)
