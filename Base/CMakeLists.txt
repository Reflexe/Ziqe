add_subdirectory (ZiqeAPI)

zq_add_library(Base LocalProcess LocalThread SystemCalls ProcessManager
               Signal Vector HashTable Memory IteratorTools Callback Checks
               Types LinkedList ForwardList InputStreamInterface
               OutputStreamInterface SharedVector Mutex FunctionTools)
