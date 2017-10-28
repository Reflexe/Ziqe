load("//Platforms:api.bzl", "zq_library_no_auto_deps")

zq_library_no_auto_deps (
    name = 'Utils',
    classes = [
        'Memory', 'Checks', 'Types', 'FunctionTools', 'Optional',
        'Expected', 'RawPointer', 'Constructor', 'DummyPointer',
        'ExtendedVectorWritable', 'RedBlackTree', 'FieldWriter', 
        'FieldReader', 'LinkedList', 'Callback', 'Logger',
        'HashTable', "Vector", 'IteratorTools', 'RangesMap', 'ExtendedVector',
        'UniquePointer', 'SharedPointer', 'Allocator'
    ],
    hdrs = ['Macros.hpp'],
    srcs = ['CompilerSymbols.cpp'],


    #includes = ['//.'],
    platform_deps = [
            'CppCore',
    ],   
    copts = ['-std=gnu++14'],
    visibility = ["//visibility:public"],
)
