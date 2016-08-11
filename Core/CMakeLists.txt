add_subdirectory (ZiqeProtocol)

zq_add_library(Core ProcessMemory GlobalProcess GlobalThread
                    ThreadOwnerWorker GlobalPeersWorker
                    GlobalThreadManager GlobalProcessManager)
