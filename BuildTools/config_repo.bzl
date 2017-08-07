load ('//Platforms:api.bzl', 'zq_config')

def _repo_impl(repo_ctx):
    repo_ctx.file("BUILD", "cc_library(name='ZiqeConfig', srcs=[], visibility=['//visibility:public'])")
    zq_config(repo_ctx)


ziqe_repo_rule = repository_rule(
                    _repo_impl,
                    local=True,
                )

def ziqe_repo():
    ziqe_repo_rule(name='ZiqeConfig')
