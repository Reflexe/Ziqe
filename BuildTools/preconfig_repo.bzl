def _repo_impl(repo_ctx):
    repo_ctx.file("BUILD", "")

    workspace_file_path = repo_ctx.path (repo_ctx.attr.workspace)

    blacklisted_symlink_paths = [
        'BUILD',
    ]

    for child in workspace_file_path.dirname.readdir():
        print (workspace_file_path.dirname.readdir())

        if child.basename not in blacklisted_symlink_paths:
            repo_ctx.symlink(child, child.basename)

    platform_path = repo_ctx.path('Platforms')

    blacklisted_paths = [
        'Tests',
        'api.bzl',
        'Common',
        'Docs',
        'Platforms',
        'Readme.md',
        'repo_api.bzl',
        'BUILD',
        'Posix',
        'WDK',
        'IOKit',
    ]

    platforms = []

    for path in platform_path.readdir():
        platform_name = path.basename

        if platform_name not in blacklisted_paths:
            platforms.append (platform_name)

    # Generate a build script that load each platform and generate
    # the driver_functions dict.
    #zq_config(repo_ctx)

    content_lines = []

    # FIXME: would be broken when api.bzl would load() because we're running in the ZiqeConfig workspace.
    for platform in platforms:
        content_lines.append ('load("//Platforms/{0}:api.bzl", "zq_{1}_driver",  "zq_{1}_config")'.format (platform, platform.lower()))

    content_lines.append ('platform_functions = {')
    for platform in platforms:
        content_lines.append ('    "{0}": {{ '.format(platform))
        content_lines.append ('        "zq_config": zq_{0}_config,'.format(platform.lower()))
        content_lines.append ('        "zq_driver": zq_{0}_driver,'.format(platform.lower()))
        content_lines.append ('    },')
    content_lines.append ('}')

    content = '\r\n'.join(content_lines)
    repo_ctx.file("platform_functions.bzl", content)

ziqe_repo_rule = repository_rule(
                    _repo_impl,
                    local=False,
                    attrs = {
                        'workspace': attr.label(),
                    },
                )

def ziqe_preconfig():
    ziqe_repo_rule(name='',
                   workspace = Label('//:WORKSPACE'))

ziqe_preconfig()
