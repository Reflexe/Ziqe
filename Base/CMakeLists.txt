zq_add_library(Base LocalProcess LocalThread SystemCalls ProcessManager
               Memory Checks
               Types  Mutex FunctionTools
               RWLock
               SpinLock  Logger Socket Optional
               Expected)

# No need for ZiqeAPI dependency, all zq_add_driver targets depend on it by default.

add_subdirectory (Tests)
