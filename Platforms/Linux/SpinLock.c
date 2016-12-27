/**
 * @file SpinLock.c
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

#include <linux/spinlock.h>

#include "ZiqeAPI/SpinLock.h"

void ZqSpinLockInit(ZqSpinLock *spinlock) {
    *spinlock = (ZqSpinLock) ZqMmAllocateVirtual (sizeof (spinlock_t));

    spin_lock_init ((spinlock_t *) *spinlock);
}

void ZqSpinLockDeinit(ZqSpinLock *spinlock)
{
    ZqMmDeallocateVirtual ((ZqKernelAddress) *spinlock);
}

void ZqSpinLockLock(ZqSpinLock *spinlock)
{
    spin_lock ((spinlock_t*)  spinlock);
}

void ZqSpinLockUnlock(ZqSpinLock *spinlock)
{
    spin_unlock ((spinlock_t*)  spinlock);
}

ZqBool ZqSpinLockTryLock(ZqSpinLock *spinlock)
{
    return (spin_trylock ((spinlock_t *) spinlock) == 1) ? ZQ_TRUE : ZQ_FALSE;
}
