def _generate_file_impl(ctx):
    ctx.actions.expand_template(
        template      = ctx.file.template,
        output        = ctx.outputs.output,
        substitutions = ctx.attr.substitutions,
    )

generate_file = rule(
    implementation=_generate_file_impl,
    output_to_genfiles = True,
    attrs = {
        'template':      attr.label(allow_files=True, single_file=True),
        'output':        attr.output(mandatory=True),
        'substitutions': attr.string_dict(),
    },
)

# Returns path to the output dir.
# The output file would be input name .out
def generate_per_driver_file(**args):
    label_file_name = Label(args['template']).name.split('/')[-1]
    input_name, input_extention = label_file_name.split('.')
    output_file_name = input_name + '.generated.' + input_extention

    target_name = args['driver_name'] + '_gen_' + output_file_name
    output_path = args['driver_name'] + '_gen_'
    output_label = output_path + '/' + output_file_name

    generate_file (
        name = target_name,
        template = args['template'],
        substitutions = args['substitutions'],
        output = output_label,
    )

    return {'output_label': output_label,
            'output_path': output_path + '/',
            'output' : output_label,
            'target_name': target_name}
