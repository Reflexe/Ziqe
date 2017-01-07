/**
 * @file Memory.cpp
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
#include "OS/Memory.h"

ZQ_BEGIN_NAMESPACE
namespace OS {
namespace Map {

Base::Expected<UserToKernel, Error> UserToKernel::Create(ZqUserAddress userAddress,
                                                         SizeType length)
{
    //       TODO: is that required? we're validating an alignment of 1 byte..
    //        __try{
    //            // do not trust if originated in user space
    //            ::ProbeForWrite(const_cast<void*>(data), size, 1);
    //        }__except(EXCEPTION_EXECUTE_HANDLER){
    //            try(status=::GetExceptionCode());
    //        }

    // Allocate a MDL for this user address.
    PMDL mdl = ::IoAllocateMdl(static_cast<PVOID>(userAddress), /* virtual addres */
                               length,   /* size */
                               FALSE,    /* secondaryBuffer */
                               FALSE,    /* ChargeQuouta */
                               nullptr); /* Irp */

    // Try to lock these pages, for kernel mode write access.
    __try{
        // wire down
        // Callers of MmProbeAndLockPages must be running at
        // IRQL <= APC_LEVEL for pageable addresses, or at
        // IRQL <= DISPATCH_LEVEL for nonpageable addresses.
        ::MmProbeAndLockPages(mdl,
                              ::KernelMode,
                              ::IoWriteAccess);
    }__except(EXCEPTION_EXECUTE_HANDLER){
        try(status=::GetExceptionCode());
    }



    if (result == ZQ_E_OK) {
        return {kernelMap};
    } else {
        return {Base::move (result)};
    }
}

Base::Expected<KernelToUser, Error>
KernelToUser::Create(ZqKernelAddress kernelAddress,
                     SizeType length)
{
    // Create a MDL with this kernel address and this length.
    auto pMDL = IoAllocateMdl (kernelAddress,
                               length,
                               FALSE,
                               FALSE,
                               nullptr);
    if (pMDL == nullptr) {
        return {ERR};
    }

    // Update this MDL's physical pages (no need to lock,
    // this MDL is already locked).
    ::MmBuildMdlForNonPagedPool(pMDL);

    ZqUserAddress userAddress;
    __try{
        // Map this MDL to user mode.
        userAddress = ::MmMapLockedPagesSpecifyCache (pMDL,
                                                      ::UserMode,
                                                      ::MmCached,
                                                      nullptr,
                                                      FALSE,
                                                      ::NormalPagePriority);
    } __except(EXCEPTION_EXECUTE_HANDLER)
    {
        return {};
    }

    return {userAddress, pMDL};
}

KernelToUser::~KernelToUser() {
    // If this is a valid map (not a moved map),
    // unmap it.
    if (mMaybeUserMapContext) {
        ::MmUnmapLockedPages(mMaybeUserMapContext->userAddress,
                             mMaybeUserMapContext->mdl);
    }
}

KernelToUser::KernelToUser(const KernelToUser::Context &context)
    : mMaybeUserMapContext{context}
{
}

UserToKernel::~UserToKernel() {
    // If this is a valid map (not a moved map),
    // unmap it.
    if (mMaybeMDL) {
        // (&mMaybeMDL.get ());
    }
}

} // namespace Map
} // namespace OS
ZQ_END_NAMESPACE
