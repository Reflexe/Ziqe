load ('@ZiqeConfig//:linux_config.bzl', 'config')
load ('//BuildTools:cc_class_library.bzl', 'cc_class_library')

linux_lastest_path = 'LinuxHeaders/4.12.3-1-ARCH'

# @brief                    Escape @string from @chars_to_escape with @escape_char.
# @param string             [str]           A string to escape.
# @param chars_to_escape    [iteratable]    Items to escape from @a string.
# @param escape_char        [str]           Char to replace any occurence of anyone from
#                                           @a chars_to_escape.
def escape(string, chars_to_escape, escape_char=None):
    # Set the default escape_char.
    if escape_char == None:
        escape_char = '\\'

    # Replace each occurence of @char with "@char+@escape_char".
    for char in chars_to_escape:
        string = string.replace (char, escape_char + char)

    return string

# @brief UNIX: generate a relative path to the workspace root from a path within the workspace.
# @param file  the file path.
def get_relative_file_path_to_workspace(file):
    return '../' * file.path.count ('/')

# @brief    Escape spaces from strings (paths)
#
# non-cross-platform way to escape spaces from strings.
# Especially useful for paths with spaces.
#
# @param A string to escape.
#
# @return 'A B C' -> 'A\ B\ C'
def escape_spaces(string):
    return escape(string, (' '))

def _generate_usb_ids_impl(ctx):
    data_file = ctx.actions.declare_file(ctx.configuration.bin_dir.path + 'data')
    ctx.actions.write(output=data_file, content=ctx.attr.input_data)


    ctx.actions.run(
        inputs = [
            data_file,
            ctx.file._script,
        ],
        outputs = [
            ctx.outputs.output
        ],
        executable = ctx.file._script,
        arguments = [
            data_file.path,
            ctx.outputs.output.path
        ],
        progress_message = 'Generating USB IDs driver_usb_ids for {}.h'.format(ctx.attr.name)
    )

generate_usb_ids = rule(
    implementation=_generate_usb_ids_impl,
    attrs = {
        '_script': attr.label(default=Label('//Platforms/Linux:Scripts/ParseUsbIDs.py'),
                                allow_single_file=True),
        'input_data': attr.string(default="[]"),
        'output': attr.output(mandatory=True),
    },
)

def _impl(ctx):
    # Add env vars to the default env.
    make_env_vars = ctx.configuration.default_shell_env

    includes_files_depset = depset()
    includes_paths_depset = depset()

    for target in ctx.attr.includes:
        includes_files_depset += target.files
        includes_paths_depset += [file.dirname for file in target.files]

    # Get the relative path from our makefile's dir to the workspace.
    CppCore_to_workspace = get_relative_file_path_to_workspace (ctx.file._makefile)

    # Get the module sources and add it the env var.
    make_env_vars['ZQ_MODULE_SOURCES'] = ''

    for f in ctx.files.srcs:
        make_env_vars['ZQ_MODULE_SOURCES'] += escape_spaces(CppCore_to_workspace + f.path) + ' '

    libs_depset = depset()
    make_env_vars['ZQ_OBJECTS'] = ''
    for dep in ctx.attr.deps:
        if 'cc' in dir(dep):
            libs_depset += dep.cc.libs.to_list()
            includes_paths_depset += dep.cc.system_include_directories
            includes_files_depset += dep.cc.transitive_headers

    for object in libs_depset.to_list():
        make_env_vars['ZQ_OBJECTS'] += escape_spaces(CppCore_to_workspace + object.path) + ' '

    # Set the target's name.
    make_env_vars['name'] = ctx.attr.name

    # Set the header's path.
    #linux_headers_path_to_workspace = get_relative_file_path_to_workspace (ctx.file.linux_headers_path)
    make_env_vars['ZQ_LINUX_HEADERS_PATH'] = escape_spaces(ctx.file.linux_headers_path.path)
    make_env_vars['ZQ_MAKEFILE_PATH']      = escape_spaces(ctx.file._makefile.dirname)


    # Add includes as compiler arguments.
    make_env_vars['ZQ_INCLUDE_PATHS'] = ''
    for d in includes_paths_depset.to_list():
        make_env_vars['ZQ_INCLUDE_PATHS'] += escape_spaces(d) + ' '

    # build the linux targets with {linux_headers_path}/build;
    # compile it to the our Makefile's path and read
    # our Makefile from stdin to make its path simpler.
    #
    #command = ('make -f "-" zq_modules < "$1" 2>&1 && tee | tee /dev/pts/0 | grep "WARNING" && { echo "Error: Found a warning" ; exit 1; } || '
    # Then, copy the built target (X.ko) to the output directory.
    #+ 'cp -r "$2/$3" "$4"')

    # Run the actual action.
    ctx.actions.run(
        inputs=ctx.files.srcs
             + ctx.files._makefile
             + ctx.files.linux_headers_path
             + ctx.files.deps
             + includes_files_depset.to_list()
             + libs_depset.to_list(),
        outputs=[ctx.outputs._output_ko],
        executable=ctx.file._script,
        arguments=['compile',
                   ctx.file._makefile.path,
                   ctx.outputs._output_ko.path],
        progress_message= "Compiling and linking Linux Module: %s" % ctx.attr.name,
        env = make_env_vars)

linux_module = rule(
    implementation=_impl,
    attrs={
        '_script': attr.label(default=Label('//Platforms/Linux:Scripts/CompileLinuxModule.sh'),
                              allow_single_file=True),
        '_makefile': attr.label(default=Label('//Platforms/Linux:CppCore/Makefile'),
                                allow_single_file=True),
        'srcs': attr.label_list(allow_files=True),
        'linux_headers_path': attr.label(default=Label('//Platforms/Linux:'+linux_lastest_path+'/build'),
                                         allow_single_file=True),
        'deps': attr.label_list(),
        'includes': attr.label_list(allow_files=True),
    },
    # The output .ko file. Maybe we should also save the .o files?
    outputs={"_output_ko": "%{name}.ko"},
)

def zq_linux_driver(**args):
    deps = []

    hdr_name = args['name'] + '_usb_ids'
    deps += [':' + hdr_name]
    input_data = ''

    if 'usb_ids' in args:
        input_data = str(args['usb_ids'])

        args.pop('usb_ids')
    else:
        input_data = '[]'

    generate_usb_ids(
        name=hdr_name,
        input_data=input_data,
        output=':' + hdr_name + '/driver_usb_ids.h'
    )

    args['deps'] = deps + args.get('deps', [])
    args['includes'] = [args['name'] + '_usb_ids'] + args.get('includes', [])

    linux_module(
        srcs = [
            '//Platforms/Linux:_linux_build_srcs'
        ],
        **args
    )

def zq_linux_library(**args):
    args['copts'] = args.get('copts', []) + config['cpp']['opts']

    args['linkstatic'] = 1

    cc_class_library(**args)
