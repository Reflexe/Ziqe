#include <wdk.h>

#include "Platforms/Macros.h"

#include "Types.hpp"

ZQ_BEGIN_C_DECL

typedef PVOID ZqKernelAddress;
typedef uint64_t ZqUserAddress;

static inline_hint 
ZqKernelAddress ZQ_SYMBOL(ZqAllocate) (ZqSizeType size) 
{
    return ::ExAllocatePoolWithTag(::NonPagedPool, size, 'Ziqe');
}

static inline_hint 
void ZQ_SYMBOL(ZqDeallocate) (ZqKernelAddress address) 
{
   ::ExFreePool (address);
}

ZQ_END_C_DECL
