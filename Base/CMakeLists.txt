zq_add_library(Base LocalProcess LocalThread SystemCalls ProcessManager
               Vector HashTable Memory IteratorTools Callback Checks
               Types LinkedList Mutex FunctionTools
               FieldReader FieldWriter RWLock RedBlackTree
               SpinLock RangesMap ExtendedVector Logger Socket Optional)

add_subdirectory (Tests)
