#load ('@ZiqeConfig//:config.bzl', 'config')

#CcSkylarkApiProvider = provider()

## duplicate 'target'
#def _impl (target, ctx):
    #if ctx.rule.kind in ('cc_library', 'cc_class_library'):
        #target.cc.compile_flags.append ('-RRRRRRRRRRRRRRR')
        #print(target.cc.compile_flags)

    #return [CcSkylarkApiProvider(compile_flags=['ABCDEFG'])]

#configure_target_aspect = aspect(
    #implementation = _impl,
    #attr_aspects = ["deps"],
#)
