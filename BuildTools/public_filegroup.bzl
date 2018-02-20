def _public_filegroup_impl(ctx):
    sym_map = {}

    for input_file in ctx.files.srcs:
        sym_map[input_file.path] = input_file

    return [DefaultInfo(files=depset(ctx.files.srcs), runfiles=ctx.runfiles(root_symlinks=sym_map))]

_public_filegroup = rule(
    implementation=_public_filegroup_impl,
    output_to_genfiles = True,
    attrs = {
        'srcs':          attr.label_list(allow_files=True),
    },
)

def public_filegroup(**vargs):
    return _public_filegroup(name = vargs['name'], srcs=vargs['srcs'])
