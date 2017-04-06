#ifndef ZQ_API_LINUX_MEM_CONFIG_H
#define ZQ_API_LINUX_MEM_CONFIG_H 

#define ZQ_PAGE_SIZE (4096)
#define ZQ_PAGE_ALIGN(addr) (addr % ZQ_PAGE_SIZE)

typedef void*          ZqKernelAddress;
typedef ZqRegisterType ZqMapableAddress;
typedef ZqRegisterType ZqUserAddress;
typedef ZqRegisterType ZqVirtualAddress;

#endif /* ZQ_API_LINUX_MEM_CONFIG  */
