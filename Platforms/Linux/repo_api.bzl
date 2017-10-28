linux_lastest_path = 'LinuxHeaders/4.12.3-1-ARCH'

_INVALID_CPP_FLAGS = ['-Wstrict-prototypes',
                     '-Wdeclaration-after-statement',
                     '-Wno-pointer-sign',
                    ]

def zq_linux_config(repo_ctx):
    root_path = str(repo_ctx.path('./'))

    script_path = str(repo_ctx.path(Label('//Platforms/Linux:Scripts/CompileLinuxModule.sh')))
    makefile_path = repo_ctx.path(Label('//Platforms/Linux:CppCore/Makefile'))

    make_env_vars = {}

    # Set the header's path.
    headers = repo_ctx.path(Label('//Platforms/Linux:'+linux_lastest_path+'/build/Makefile'))

    make_env_vars['ZQ_LINUX_HEADERS_PATH'] = str(headers.dirname)
    make_env_vars['name'] = 'test'

    result = repo_ctx.execute(
        [
            script_path,
            'info',
            makefile_path,
            makefile_path
        ],
        environment=make_env_vars,
    )

    flags = result.stdout
    l = flags[1:].strip().split(' ')

    l = [e.strip() for e in l if e != '' and e[0:4] != '-std' and e not in _INVALID_CPP_FLAGS]
    l += ['-fno-exceptions', '-fno-rtti']

    repo_ctx.template(
            'linux_config.bzl',
            Label('//BuildTools:config.bzl'),
            {
                    '\'%{opts}\'': str(l)
            },
    )
