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

def _impl(ctx):
    # Add env vars to the default env.
    make_env_vars = ctx.configuration.default_shell_env

    includes_paths_depset = depset(ctx.attr.includes)
    includes_files_depset = depset()

    # Get the relative path from our makefile's dir to the workspace.
    zqapi_to_workspace = get_relative_file_path_to_workspace (ctx.file._makefile)

    # Get the module sources and add it the env var.
    make_env_vars['ZQ_MODULE_SOURCES'] = ''

    for f in ctx.files.srcs:
        make_env_vars['ZQ_MODULE_SOURCES'] += escape_spaces(zqapi_to_workspace + f.path) + ' '


    libs_depset = depset()
    make_env_vars['ZQ_OBJECTS'] = ''
    for dep in ctx.attr.deps:
        #print('dep: %s; dep.lib: %s; dep.cc.libs: %s' % (dep.label, str(dep.files.to_list()), str(dep.cc.libs.to_list())))
        #print(dir(dep.cc.libs.to_list))
        #print(dep.cc.libs.to_list())
        libs_depset += dep.cc.libs.to_list()
        includes_paths_depset += dep.cc.system_include_directories
        includes_files_depset += dep.cc.transitive_headers

    for object in libs_depset.to_list():
        make_env_vars['ZQ_OBJECTS'] += escape_spaces(zqapi_to_workspace + object.path) + ' '

    # Set the target's name.
    make_env_vars['name'] = ctx.attr.name

    # Set the header's path.
    make_env_vars['ZQ_LINUX_HEADERS_PATH'] = escape_spaces(ctx.file.linux_headers_path.path)
    make_env_vars['ZQ_MAKEFILE_PATH']      = escape_spaces(ctx.file._makefile.dirname)

    linux_headers_path_to_workspace = get_relative_file_path_to_workspace (ctx.file.linux_headers_path)

    # Add includes as compiler arguments.
    make_env_vars['ZQ_INCLUDE_PATHS'] = ''
    for d in includes_paths_depset.to_list():
        make_env_vars['ZQ_INCLUDE_PATHS'] += escape_spaces(d) + ' '

    # build the linux targets with {linux_headers_path}/build;
    # compile it to the our Makefile's path and read
    # our Makefile from stdin to make its path simpler.
    #
    # FIXME: a script instead of this ugly hackish thingy.
    command = ('make -f "-" zq_modules < "$1" 2>&1 && tee | tee /dev/pts/0 | grep "WARNING" && { echo "Error: Found a warning" ; exit 1; } || '
    # Then, copy the built target (X.ko) to the output directory.
    + 'cp -r "$2/$3" "$4"')

    # Run the actual action.
    ctx.action(
        inputs=ctx.files.srcs
             + ctx.files._makefile
             + ctx.files.linux_headers_path
             + ctx.files.deps
             + includes_files_depset.to_list()
             + libs_depset.to_list(),
        outputs=[ctx.outputs._output_ko],
        command=command,
        arguments=[ctx.file._makefile.path,
                   ctx.file._makefile.dirname,
                   ctx.outputs._output_ko.basename,
                   ctx.outputs._output_ko.path],
        progress_message= "Compiling and linking Linux Module: %s" % ctx.attr.name,
        env = make_env_vars)

linux_module = rule(
    implementation=_impl,
    attrs={
        '_makefile': attr.label(default=Label('//Platforms/Linux:ZqAPI/Makefile'),
                                allow_single_file=True),
        'srcs': attr.label_list(allow_files=True),
        'linux_headers_path': attr.label(default=Label('//Platforms/Linux:LinuxHeaders/4.10.6-1-ARCH/build'),
                                         allow_single_file=True),
        'deps': attr.label_list(),
        'includes': attr.label_list(),
    },
    # The output .ko file. Maybe we should also save the .o files?
    outputs={"_output_ko": "%{name}.ko"},
)

def zq_linux_driver(**args):
    args['deps'] = ['//Platforms/Linux:ZqAPI'] + args.get('deps', [])

    linux_module(
        srcs = [
            '//Platforms/Linux:_linux_build_srcs'
        ],
        **args
    )
