add_subdirectory (Protocol)

zq_add_library(Core ThreadOwnerServer ThreadOwnerClient
                    ProcessMemory GlobalProcess GlobalThread
                    GlobalThreadManager GlobalProcessManager Types
                    ProcessPeersServer
                    GlobalPeers ProcessPeersClient MemoryRevisionTree
                    MessageStreamFactoryInterface)
