load("//BuildTools:driver_functions.bzl", 'generate_per_driver_file')

# runs per driver
def common_per_driver_config(**args):
    result_dict = generate_per_driver_file(
        driver_name = args['name'],
        template    = '//Platforms/Common:PerDriver/in/config.h',
        substitutions = {
            '%{DRIVER_NAME}': args['name'],
        }
    )

    return result_dict
