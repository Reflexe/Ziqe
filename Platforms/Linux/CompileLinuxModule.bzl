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
    
def escape_quote(string):
    return escape(string, ('"'))

def _impl(ctx):
    # Add env vars to the default env.
    make_env_vars = ctx.configuration.default_shell_env
    
    includes = [f.path for f in ctx.attr.includes]
    
    # Get the relative path from our makefile's dir to the workspace.
    zqapi_to_workspace = get_relative_file_path_to_workspace (ctx.file._makefile)

    # Get the module sources and add it the env var.
    make_env_vars['ZQ_MODULE_SOURCES'] = ''
    for f in ctx.files.srcs:
        make_env_vars['ZQ_MODULE_SOURCES'] += escape_spaces(zqapi_to_workspace + f.path) + ' '
        
    make_env_vars['ZQ_OBJECTS'] = ''
    for dep in ctx.attr.deps:
        # An hack to get only the .a library, it there another way?
        # Please, don't let it stay like that.
        files_list = dep.files.to_list()
        if files_list:
            a_lib = files_list[0]
            make_env_vars['ZQ_OBJECTS'] += escape_spaces(zqapi_to_workspace + a_lib.path) + ' '
            
        includes += dep.cc.system_include_directories
        
    # Set the target's name.
    make_env_vars['name'] = ctx.attr.name
    # Set the header's path.
    make_env_vars['ZQ_LINUX_HEADERS_PATH'] = escape_spaces(ctx.file.linux_headers_path.path)
    make_env_vars['ZQ_MAKEFILE_PATH']      = escape_spaces(ctx.file._makefile.dirname)
    
    linux_headers_path_to_workspace = get_relative_file_path_to_workspace (ctx.file.linux_headers_path)
    
    # Add includes as compiler arguments.
    make_env_vars['ZQ_COMPILER_ARGUMENTS'] = ''
    for d in includes:
        make_env_vars['ZQ_COMPILER_ARGUMENTS'] += ' ' + escape_spaces(d)
    
    # build the linux targets with {linux_headers_path}/build;
    # compile it to the our Makefile's path and read 
    # our Makefile from stdin to make its path simpler.
    #
    command = ('make -f "-" zq_modules < "{0}" && '
    # Then, copy the built target (X.ko) to the output directory.
    + 'cp -r "{1}/{2}" "{3}"').format(escape_quote(ctx.file._makefile.path),
                                      escape_quote(ctx.file._makefile.dirname),
                                      escape_quote(ctx.outputs._output_ko.basename),
                                      escape_quote(ctx.outputs._output_ko.path))

    # Run the actual action.
    ctx.action(
        inputs=ctx.files.srcs 
             + ctx.files._makefile 
             + ctx.files.linux_headers_path 
             + ctx.files.deps 
             + [f for t in ctx.attr.deps for f in t.cc.transitive_headers.to_list()],
        outputs=[ctx.outputs._output_ko],
        command=command,
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

def zq_driver(**args):
    args['deps'] = ['//Platforms/Linux:ZqAPI'] + args.get('deps', [])

    linux_module(
        srcs = [
            '//Platforms/Linux:_linux_build_srcs'
        ],
        **args
    )


