zq_add_library(Base LocalProcess LocalThread SystemCalls ProcessManager
               Vector HashTable Memory IteratorTools Callback Checks
               Types LinkedList Mutex FunctionTools
               FieldReader FieldWriter RWLock RedBlackTree
               SpinLock RangesMap ExtendedVector Logger Socket Optional
               Expected)

# No need for ZiqeAPI dependency, all zq_add_driver targets depend on it by default.

add_subdirectory (Tests)
