zq_add_library(Base LocalProcess LocalThread SystemCalls ProcessManager
               Vector HashTable Memory IteratorTools Callback Checks
               Types LinkedList InputStreamInterface
               OutputStreamInterface Mutex FunctionTools
               FieldReader FieldWriter IOStreamInterface RWLock RedBlackTree
               SpinLock RangesMap ExtendedVector Logger Socket)

add_subdirectory (Tests)
