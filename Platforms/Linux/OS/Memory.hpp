/**
 * @file Memory.hpp
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
#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Platforms/Macros.h"

#include "Base/Expected.hpp"
#include "Base/Optional.hpp"

#include "ZqAPI/Memory.h"

ZQ_BEGIN_NAMESPACE
namespace OS {
namespace Allocators {
/**
   @brief The regular kernel allocator.

   Allocates page-aligned non-contiguous memory.

   @note If it wasn't page aligned, we wouldn't be able
         to map it to usermode safely (other data can be in
         the same page).
 */
struct Kernel {
     static Base::Expected<ZqKernelAddress, Error>
             Allocate(ZqSizeType size)
     {
         return ZQ_SYMBOL(ZqAllocate) (size);
     }

     static void Deallocate(ZqKernelAddress address)
     {
         ZQ_SYMBOL(ZqDeallocate) (address);
     }
};

struct User {
    typedef ZqUserMemoryAreaProtection Protection;

    static Base::Expected<ZqUserAddress, Error>
            Allocate(ZqSizeType size, Protection protection) {
        ZqUserAddress addressResult;
        auto result = ZQ_SYMBOL(ZqMmAllocateUserMemory) (size, protection, &addressResult);

        if (result != ZQ_E_OK)
            return {Base::move (result)};
        else
            return {addressResult};
    }

    static void Deallocate(ZqUserAddress address, ZqSizeType size)
    {
        ZQ_SYMBOL(ZqMmDeallocateUserMemory) (address, size);
    }
};

} // Allocators

namespace Map {
struct UserToKernel {
    static Base::Expected<UserToKernel, Error>
            Create (ZqUserAddress userAddress, SizeType length)
    {
        ZqToKernelMapContext kernelMap;
        kernelMap.sourceAddress = userAddress;
        kernelMap.length        = length;

        auto result = ZQ_SYMBOL(ZqMmMapUserToKernel) (&kernelMap);

        if (result == ZQ_E_OK) {
            return {kernelMap};
        } else {
            return {Base::move (result)};
        }
    }

    // Optional should handle move.
    ZQ_ALLOW_MOVE (UserToKernel)
    ZQ_DISALLOW_COPY (UserToKernel)

    ~UserToKernel() {
        // If this is a valid map (not a moved map),
        // unmap it.
        if (mMaybeKernelMapContext)
            ZQ_SYMBOL(ZqMmUnmapUserToKernel) (&mMaybeKernelMapContext.get ());
    }

    ZqKernelAddress getKernelAddress () const
    {
        return mMaybeKernelMapContext->destinationAddress;
    }

    ZqUserAddress getUserAddress() const
    {
        return mMaybeKernelMapContext->sourceAddress;
    }

    SizeType getMapLength() const
    {
        return mMaybeKernelMapContext->length;
    }

private:
    ZQ_ALLOW_EXPECTED();

    UserToKernel(const ZqToKernelMapContext &context)
        : mMaybeKernelMapContext{context}
    {
    }

    Base::Optional<ZqToKernelMapContext> mMaybeKernelMapContext;
};

struct KernelToUser {
    static Base::Expected<KernelToUser, Error>
            Create (ZqKernelAddress kernelAddress, SizeType length)
    {
        ZqToUserMapContext context;
        context.sourceAddress = kernelAddress;
        context.length = length;

        auto result = ZQ_SYMBOL(ZqMmMapKernelToUser) (&context);

        if (result == ZQ_E_OK) {
            return {context};
        } else {
            return {static_cast<Error>(result)};
        }
    }

    // Optional should handle move.
    ZQ_ALLOW_MOVE (KernelToUser)
    ZQ_DISALLOW_COPY (KernelToUser)

    ~KernelToUser() {
        // If this is a valid map (not a moved map),
        // unmap it.
        if (mMaybeUserMapContext)
            ZQ_SYMBOL(ZqMmUnmapKernelToUser) (&mMaybeUserMapContext.get ());
    }

    ZqKernelAddress getKernelAddress () const
    {
        return mMaybeUserMapContext->sourceAddress;
    }

    ZqUserAddress getUserAddress() const
    {
        return mMaybeUserMapContext->destinationAddress;
    }

    SizeType getMapLength() const
    {
        return mMaybeUserMapContext->length;
    }

private:
    ZQ_ALLOW_EXPECTED();

    KernelToUser(const ZqToUserMapContext &context)
        : mMaybeUserMapContext{context}
    {
    }

    Base::Optional<ZqToUserMapContext> mMaybeUserMapContext;
};
} // namespace Map

}
ZQ_END_NAMESPACE

#endif // MEMORY_HPP
