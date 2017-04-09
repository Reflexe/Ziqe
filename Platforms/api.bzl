load(":Linux/api.bzl", "zq_linux_driver")

platform = 'Linux'

def zq_driver (**args):
    zq_linux_driver (**args)
