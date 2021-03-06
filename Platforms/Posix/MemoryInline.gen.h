#ifndef ZQ_API_LINUX_MEM_INLINE_H
#define ZQ_API_LINUX_MEM_INLINE_H

#ifdef ZQ_TEST_BUILD
#include <cstdlib>
static inline_hint void vfree (ZqKernelAddress addr)
{
    free (addr);
}
static inline_hint ZqKernelAddress vmalloc (ZqSizeType size)
{
    return malloc (size);
}
#else
extern void *vmalloc(unsigned long size);
extern void vfree(const void *addr);
#endif

static inline_hint void ZqDeallocateVirtual (ZqKernelAddress address)
{
    vfree ((void *) address);
}

static inline_hint ZqKernelAddress ZqAllocateVirtual (ZqSizeType size)
{
    return (ZqKernelAddress) vmalloc (size);
}

#define memcpy(d,s,l) __builtin_memcpy(d,s,l)
#define memset(d,c,l) __builtin_memset(d,c,l)
#define memcmp  __builtin_memcmp

#endif /* ZQ_API_LINUX_MEM_INLINE_H  */
