#ifndef ZQ_API_LINUX_MEM_INLINE_H
#define ZQ_API_LINUX_MEM_INLINE_H

extern void *vmalloc(unsigned long size);
extern void vfree(const void *addr);

static inline_hint void ZqMmDeallocateVirtual (ZqKernelAddress address)
{
    vfree ((void *) address);
}

static inline_hint ZqKernelAddress ZqMmAllocateVirtual (ZqSizeType size)
{
    return (ZqKernelAddress) vmalloc (size);
}

#define memcpy(d,s,l) __builtin_memcpy(d,s,l)
#define memset(d,c,l) __builtin_memset(d,c,l)
#define memcmp  __builtin_memcmp

#endif /* ZQ_API_LINUX_MEM_INLINE_H  */
