load('//Platforms:PlatformFunctions.bzl', 'platform_functions')

_PLATFORM_SETTING_PREFIX = 'platform_'
_PLATFORM_TARGET_FORMAT  = '//Platforms/{platform}:{target}'

def get_label_for_platform(**args):
    return _PLATFORM_TARGET_FORMAT.format(platform=args['platform'], target=args['target'])


def select_platform_target(**args):
    targets = args['targets']
    not_platform_targets = args.get('not_platform_targets', ())

    select_dict = {}

    for platform in platform_functions.keys():
        platform_targets = [get_label_for_platform(platform=platform, target=target) for target in targets]
        platform_targets.extend(not_platform_targets)

        select_dict[platform] = platform_targets

    return select_ziqe_platform(select_dict)

def platform_to_platform_setting(platform):
    return _PLATFORM_SETTING_PREFIX + platform

def define_platform_config_setting(name):
    config_setting_name = platform_to_platform_setting (name)

    native.config_setting(name=config_setting_name,
                          define_values = {
                            'ziqe_platform': name,
                          },
    )

def add_platform_config_settings ():
    for platform in platform_functions:
        define_platform_config_setting(platform)

def platform_setting_to_platform(platform_setting):
    return platform_setting[len(_PLATFORM_SETTING_PREFIX):]

def select_ziqe_platform (select_dict):
    d = {}

    for platform, content in select_dict.items():
        d[platform_to_platform_setting(platform)] = content

    return select(d)
