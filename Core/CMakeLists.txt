add_subdirectory (ZiqeProtocol)
add_subdirectory (Tasks)

zq_add_library(Core PeersManager Peer ProcessMemory Task GlobalProcess GlobalThread)
