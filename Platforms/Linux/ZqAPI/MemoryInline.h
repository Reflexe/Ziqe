#ifndef ZQ_API_LINUX_MEM_INLINE_H
#define ZQ_API_LINUX_MEM_INLINE_H

extern void *vmalloc(unsigned long size);
extern void vfree(const void *addr);

may_inline void ZqDeallocateVirtual (ZqKernelAddress address)
{
    vfree ((void *) address);
}

may_inline ZqKernelAddress ZqAllocateVirtual (ZqSizeType size)
{
    return (ZqKernelAddress) vmalloc (size);
}

#endif /* ZQ_API_LINUX_MEM_INLINE_H  */
