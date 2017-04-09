load("//BuildTools:test.bzl", "cc_class_library")

cc_class_library (
    'Utils',
    classes = [
        'Memory', 'Checks', 'Types', 'FunctionTools', 'Optional',
        'Expected', 'RawPointer', 'Constructor', 'DummyPointer',
        'ExtendedVectorWritable', 'RedBlackTree', 'FieldWriter', 
        'FieldReader', 'LinkedList', 'Callback', 'Logger',
        'HashTable', "Vector", 'IteratorTools', 'RangesMap', 'ExtendedVector',
        'UniquePointer', 'SharedPointer', 'Allocator'
    ],
    hdrs = ['Macros.hpp'],
    #includes = ['//.'],
    deps = [
            '//Platforms:ZqAPI',
    ],   
    copts = ['-std=gnu++14'],
    visibility = ["//visibility:public"],
)
