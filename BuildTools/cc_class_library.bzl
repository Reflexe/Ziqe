load ('//BuildTools:config.bzl', 'config')

def cc_class_library(**args):
    srcs = []
    hdrs = []
    
    if args.get('classes') != None:
        # For each class, add a src and hdr file with the defined ext.
        for c in args.get('classes'):        
            srcs.append(c + '.' + config['cpp']['class_src_ext'])
            hdrs.append(c + '.' + config['cpp']['class_hdr_ext'])

        # Remove 'classes'
        args.pop('classes')
        
        args['hdrs'] = args.get('hdrs', []) + hdrs
        args['srcs'] = args.get('srcs', []) + srcs

    #args['copts'] = args.get('copts', []) + ['-Wall', '-Wundef',  '-Wno-trigraphs', '-fno-strict-aliasing', '-fno-common', '-Werror-implicit-function-declaration', '-Wno-format-security', '-fno-PIE', '-mno-sse', '-mno-mmx', '-mno-sse2', '-mno-3dnow', '-mno-avx', '-m64', '-falign-jumps=1', '-falign-loops=1', '-mno-80387', '-mno-fp-ret-in-387', '-mpreferred-stack-boundary=3', '-mskip-rax-setup', '-mtune=generic', '-mno-red-zone', '-mcmodel=kernel', '-funit-at-a-time', '-maccumulate-outgoing-args', '-DCONFIG_AS_CFI=1', '-DCONFIG_AS_CFI_SIGNAL_FRAME=1', '-DCONFIG_AS_CFI_SECTIONS=1', '-DCONFIG_AS_FXSAVEQ=1', '-DCONFIG_AS_SSSE3=1', '-DCONFIG_AS_CRC32=1', '-DCONFIG_AS_AVX=1', '-DCONFIG_AS_AVX2=1', '-DCONFIG_AS_AVX512=1', '-DCONFIG_AS_SHA1_NI=1', '-DCONFIG_AS_SHA256_NI=1', '-pipe', '-Wno-sign-compare', '-fno-asynchronous-unwind-tables', '-fno-delete-null-pointer-checks', '-Wno-frame-address', '-O2', '--param=allow-store-data-races=0', '-Wframe-larger-than=2048', '-fstack-protector-strong', '-Wno-unused-but-set-variable', '-Wno-unused-const-variable', '-fno-omit-frame-pointer', '-fno-optimize-sibling-calls', '-fno-var-tracking-assignments', '-pg', '-mfentry', '-DCC_USING_FENTRY', '-fno-strict-overflow', '-fconserve-stack', '-Werror=implicit-int', '-Werror=strict-prototypes', '-Werror=date-time', '-Werror=incompatible-pointer-types', '-DCC_HAVE_ASM_GOTO', '-fno-exceptions', '-fno-rtti']
    #args['copts'] = args.get('copts', []) + config['cpp']['opts']
    #print( config['cpp']['opts'])

    native.cc_library( 
#        linkopts=linkopts,
#        copts=copts,
#        linkstatic=1,
        **args
    )
