zq_add_library(CoreCommon   ProcessMemory Types ProcessPeersServer
                         ProcessPeersClient MemoryRevisionTree
                        MessageStreamFactoryInterface)

zq_add_depend_modules(CoreCommon CoreProtocol)
