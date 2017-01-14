/**
 * @file Memory.h
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2016 Shmuel Hazan
 *
 * Ziqe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ziqe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MEMORY_H
#define MEMORY_H

#include "Base/Optional.hpp"
#include "Base/UniquePointer.hpp"
#include "Base/Expected.hpp"

#include "Types.hpp"

typedef PVOID ZqKernelAddress;
typedef uint64_t ZqUserAddress;

ZQ_BEGIN_NAMESPACE
namespace OS {
namespace Allocators {
struct Kernel {
     static Base::Expected<ZqKernelAddress, Error>
             Allocate(ZqSizeType size)
     {
         return {::ExAllocatePoolWithTag(::NonPagedPool, size, 'Ziqe')};
     }

     static void Deallocate(ZqKernelAddress address)
     {
         ::ExFreePool (address);
     }
};

struct User {
    static ZqUserAddress allocate(ZqSizeType size) {
        // TODO: is there a way to implement that?
        // (except for actually mapping kernel memory to the user).
    }

    static void deallocate (ZqKernelAddress address)
    {
    }
};
}

namespace _WDK {
struct MDLDeleter {
    void operator () (::MDL *pMDL)
    {
        IoFreeMdl (pMDL);
    }
};

/**
   @brief A UniquePointer for WDK's MDLs.

   This pointer would call IoFreeMdl in its destructor,
   all other operations (such as MmUnlockPages) would not
   be called.
 */
typedef Base::UniquePointer<::MDL, MDLDeleter> MDLUniquePointer;
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

}

#endif // MEMORY_H
