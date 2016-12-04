#ifndef ZQ_API_LINUX_MEM_CONFIG_H
#define ZQ_API_LINUX_MEM_CONFIG_H

typedef void*          ZqKernelAddress;
typedef const void *   ZqConstKernelAddress;
typedef ZqRegisterType ZqMapableAddress;
typedef ZqRegisterType ZqUserAddress;
typedef ZqRegisterType ZqVirtualAddress;

typedef int            ZqUserMemoryAreaProtection;

#define ZQ_VA_PERM_WRITE ()
#define ZQ_VA_PERM_READ ()
#define ZQ_VA_PERM_EXEC ()
#define ZQ_VA_PERM_IO ()

/* Virtual Areas - one or more vm_area_struct */
typedef struct {
    ZqKernelAddress _pVmArea;
} ZqVirtualMemoryArea;

typedef unsigned long ZqMemoryMemoryAreaFlags;

#endif /* ZQ_API_LINUX_MEM_CONFIG  */
