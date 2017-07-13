load("//Platforms:api.bzl", "zq_driver")
load("//BuildTools:test.bzl", "cc_class_library")

cc_class_library('ExpectedTestLib', srcs=['ExpectedTest.cpp'], deps=['//Utils:Utils'])
zq_driver(name='ExpectedTest', deps=[':ExpectedTestLib'])

cc_class_library('HashTableTestLib', srcs=['HashTableTest.cpp'], deps=['//Utils:Utils'])
zq_driver(name='HashTableTest', deps=[':ExpectedTestLib'])

cc_class_library('LinkedListTestLib', srcs=['LinkedListTest.cpp'], deps=['//Utils:Utils'])
zq_driver(name='LinkedListTest', deps=[':LinkedListTestLib'])
