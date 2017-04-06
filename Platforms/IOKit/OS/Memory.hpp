#ifndef IOKIT_MEMORY_H
#define IOKIT_MEMORY_H

#include "Platforms/Macros.h"

#include "Base/Optional.hpp"
#include "Base/UniquePointer.hpp"
#include "Base/Expected.hpp"

#include "ZqAPI/Memory.h"

ZQ_BEGIN_NAMESPACE

namespace OS {
namespace Allocators {
struct Kernel {
     static Base::Expected<ZqKernelAddress, Error>
             Allocate(ZqSizeType size)
     {
         return {ZQ_SYMBOL (ZqAllocate)(size)};
     }

     static void Deallocate(ZqKernelAddress address)
     {
         ZQ_SYMBOL(ZqDeallocate) (address);
     }
};

struct User {
    static Base::Expected<ZqUserAddress, Error>
        allocate(ZqSizeType size) {
        ::IOBufferMemoryDescriptor::withOptions(::kIODirectionOutIn || ((scope_t==user)?::kIOMemoryKernelUserShared:0),
                                                size,
                                                alignement);
    }

    static void deallocate (ZqKernelAddress address)
    {
    }
};
}

namespace _IOKit {
struct IOMemoryDescriptorDeleter {
    void operator () (::IOMemoryDescriptor *pMemory)
    {
        pMemory->release ();
    }
};

/**
   @brief A UniquePointer for WDK's MDLs.

   This pointer would call IoFreeMdl in its destructor,
   all other operations (such as MmUnlockPages) would not
   be called.
 */
typedef Base::UniquePointer<::IOMemoryDescriptor, IOMemoryDescriptorDeleter> MDLUniquePointer;
}

namespace Map {

/**
   @brief   Create a mapping of a user memory area to
            The kernel memory area.

    In WDK, there is no way to map a real usermode address
    to the kernel. We'll be using IOCTL to pass a user pointer
    and then, in this class, we'll only make sure we can
    read from it.
 */
struct UserToKernel {
    static Base::Expected<UserToKernel, Error>
            Create (ZqUserAddress userAddress, SizeType length);

    // Optional should handle move.
    ZQ_ALLOW_MOVE (UserToKernel)
    ZQ_DISALLOW_COPY (UserToKernel)

    ~UserToKernel();

    ZqKernelAddress getKernelAddress () const
    {
        return MmGetMdlVirtualAddress ();
    }

    ZqUserAddress getUserAddress() const
    {
        return mMappedAddress;
    }

    SizeType getMapLength() const
    {
        return MmGetMdlByteCount();
    }

private:
    ZQ_ALLOW_EXPECTED();

    UserToKernel(const ZqToKernelMapContext &context)
        : mMaybeMDL{context}
    {
    }

    ZqUserAddress mMappedAddress;
    _WDK::MDLUniquePointer mMaybeMDL;
};

struct KernelToUser {
    static Base::Expected<KernelToUser, Error>
            Create (ZqKernelAddress kernelAddress, SizeType length);

    // Optional should handle move.
    ZQ_ALLOW_MOVE (KernelToUser)
    ZQ_DISALLOW_COPY (KernelToUser)

    ~KernelToUser();

    ZqKernelAddress getKernelAddress () const
    {
        return  MmGetMdlVirtualAddress (mMaybeUserMapContext->mdl.get ());
    }

    ZqUserAddress getUserAddress() const
    {
        return mMaybeUserMapContext->userAddress;
    }

    SizeType getMapLength() const
    {
        return MmGetMdlByteCount (mMaybeUserMapContext->mdl.get ());
    }

private:
    ZQ_ALLOW_EXPECTED();

    struct Context {
        MDLUniquePointer mdl;
        ZqUserAddress userAddress;
    };

    KernelToUser(const Context &context);

    Base::Optional<Context> mMaybeUserMapContext;
};
} // namespace Map
ZQ_END_NAMESPACE

#endif // IOKIT_MEMORY_H
