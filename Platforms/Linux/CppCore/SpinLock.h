/**
 * @file SpinLock.h
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
#ifndef ZIQE_API_SPINLOCK_H
#define ZIQE_API_SPINLOCK_H

#include "CppCore/Macros.h"
#include "Memory.h"

ZQ_BEGIN_C_DECL

typedef ZqKernelAddress ZqSpinLock;
#define ZQ_UNDEFINED_SPINLOCK ZQ_NULL

void ZqSpinLockInit(ZqSpinLock *spinlock);
void ZqSpinLockDeinit(ZqSpinLock *spinlock);

ZqBool ZqSpinLockTryLock(ZqSpinLock *spinlock);
void ZqSpinLockLock(ZqSpinLock *spinlock);
void ZqSpinLockUnlock(ZqSpinLock *spinlock);

ZQ_END_C_DECL

#endif // ZIQE_API_SPINLOCK_H
