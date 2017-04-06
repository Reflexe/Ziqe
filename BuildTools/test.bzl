load (':config.bzl', 'config')
                        
def cc_class_library(name, *argv, **args):
    srcs = []
    hdrs = []
    
    if 'classes' in args: 
        # For each class, add a src and hdr file with the defined ext.
        for c in args.get('classes'):        
            srcs.append(c + '.' + config['cpp']['class_src_ext'])
            hdrs.append(c + '.' + config['cpp']['class_hdr_ext'])

        # Remove 'classes'
        args.pop('classes')
        
        args['hdrs'] = args.get('hdrs', []) + hdrs
        args['srcs'] = args.get('srcs', []) + srcs

    if 'deps' not in args:
        args['deps'] = []

    args['copts'] = args.get('copts', []) + ['-Wall', '-Wundef', '-Wstrict-prototypes', '-Wno-trigraphs', '-fno-strict-aliasing', '-fno-common', '-Werror-implicit-function-declaration', '-Wno-format-security', '-std=gnu89', '-fno-PIE', '-mno-sse', '-mno-mmx', '-mno-sse2', '-mno-3dnow', '-mno-avx', '-m64', '-falign-jumps=1', '-falign-loops=1', '-mno-80387', '-mno-fp-ret-in-387', '-mpreferred-stack-boundary=3', '-mskip-rax-setup', '-mtune=generic', '-mno-red-zone', '-mcmodel=kernel', '-funit-at-a-time', '-maccumulate-outgoing-args', '-DCONFIG_AS_CFI=1', '-DCONFIG_AS_CFI_SIGNAL_FRAME=1', '-DCONFIG_AS_CFI_SECTIONS=1', '-DCONFIG_AS_FXSAVEQ=1', '-DCONFIG_AS_SSSE3=1', '-DCONFIG_AS_CRC32=1', '-DCONFIG_AS_AVX=1', '-DCONFIG_AS_AVX2=1', '-DCONFIG_AS_AVX512=1', '-DCONFIG_AS_SHA1_NI=1', '-DCONFIG_AS_SHA256_NI=1', '-pipe', '-Wno-sign-compare', '-fno-asynchronous-unwind-tables', '-fno-delete-null-pointer-checks', '-Wno-frame-address', '-O2', '--param=allow-store-data-races=0', '-Wframe-larger-than=2048', '-fstack-protector-strong', '-Wno-unused-but-set-variable', '-Wno-unused-const-variable', '-fno-omit-frame-pointer', '-fno-optimize-sibling-calls', '-fno-var-tracking-assignments', '-pg', '-mfentry', '-DCC_USING_FENTRY', '-Wdeclaration-after-statement', '-Wno-pointer-sign', '-fno-strict-overflow', '-fconserve-stack', '-Werror=implicit-int', '-Werror=strict-prototypes', '-Werror=date-time', '-Werror=incompatible-pointer-types', '-DCC_HAVE_ASM_GOTO']


    #run_platform_build_hook():
    #
    #args['deps'] += ['//Platforms:ZqAPI/X']
    # X would return struct(copts=kbuild_flags)
    # and its deps would be updated
    # Deps:
    #   2: Provider for headers and Build config.
    #   3: actual linking: depends on libraries.

    native.cc_library( 
        name = name,
#        linkopts=linkopts,
#        copts=copts,
        linkstatic=1,
        **args
    )
