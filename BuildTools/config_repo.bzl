load('//Platforms:repo_api.bzl', 'platform_functions')

def _repo_impl(repo_ctx):
    repo_ctx.file("BUILD", "")

    for name, functions in platform_functions.items():
        f = functions['zq_config']
        f(repo_ctx)

ziqe_config_repo_rule = repository_rule(
                    _repo_impl,
                    local=False
                )

def config_repo():
    ziqe_config_repo_rule(
        name='ZiqeConfig'
    )
