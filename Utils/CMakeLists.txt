zq_add_library(NAME Utils
               CLASSES
    #LocalProcess LocalThread SystemCalls ProcessManager
               Memory Checks
               Types   FunctionTools

               #SpinLock  Logger Socket  RWLock Mutex
               Optional
               Expected RawPointer Constructor DummyPointer

               ExtendedVectorWritable RedBlackTree FieldWriter FieldReader LinkedList Callback
                               HashTable Vector IteratorTools RangesMap ExtendedVector
                               UniquePointer SharedPointer Allocator)

# No need for ZiqeAPI dependency, all zq_add_driver targets depend on it by default.

add_subdirectory (Tests)
