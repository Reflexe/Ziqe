load("//Platforms/Linux:CompileLinuxModule.bzl", "zq_driver")
load("//BuildTools:test.bzl", "cc_class_library")

cc_class_library('ExpectedTestLib', srcs=['ExpectedTest.cpp'], deps=['//Utils:Utils'])
zq_driver(name='ExpectedTest', deps=[':ExpectedTestLib'])
