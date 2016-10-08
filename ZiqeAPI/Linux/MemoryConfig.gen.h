#ifndef ZQ_API_LINUX_MEM_CONFIG_H
#define ZQ_API_LINUX_MEM_CONFIG_H 

// TODO: make this code for more archs.
#define ZQ_PAGE_SIZE (4096)
#define ZQ_PAGE_ALIGN(addr) (addr % ZQ_PAGE_SIZE)
typedef uint64_t _ZqLinuxPTE;

typedef void*          ZqKernelAddress;
typedef const void *   ZqConstKernelAddress;
typedef ZqRegisterType ZqMapableAddress;
typedef ZqRegisterType ZqUserAddress;
typedef ZqRegisterType ZqVirtualAddress;

/* Pysical Page * */
typedef struct _ZqPhysicalPage {
    /* An struct page* ptr */
    ZqKernelAddress _page;
} ZqPhysicalPage;

/* Virtual Pages */
typedef _ZqLinuxPTE ZqVirtualPage;
typedef int ZqVirtualPagePermissions;

#define ZQ_VPAGE_PERM_WRITE ()
#define ZQ_VPAGE_PERM_READ ()
#define ZQ_VPAGE_PERM_EXEC ()

/* Virtual Areas - one or more vm_area_struct */
typedef struct _ZqVirtualMemoryArea
{
    ZqKernelAddress _vma;

    ZqKernelAddress *_flags;
} ZqVirtualMemoryArea;

typedef int ZqMemoryFaultFlags;

#endif /* ZQ_API_LINUX_MEM_CONFIG  */
