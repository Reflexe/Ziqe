/**
 * @file RWLock
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
#ifndef ZIQEAPI_RWLOCK_H
#define ZIQEAPI_RWLOCK_H

#include "ZiqeAPI/Macros.h"
#include "ZiqeAPI/Memory.h"

ZQ_BEGIN_C_DECL

#define ZQ_UNDEFINED_RWLOCK (0)

typedef ZqKernelAddress ZqRWLock;

void ZqRWLockInit(ZqRWLock *rw_lock);
void ZqRWLockDeinit(ZqRWLock *rw_lock);

ZqBool ZqRWLockTryLockWrite(ZqRWLock *rw_lock);
void ZqRWLockLockWrite(ZqRWLock *rw_lock);
void ZqRWLockUnlockWrite(ZqRWLock *rw_lock);

ZqBool ZqRWLockTryLockRead(ZqRWLock *rw_lock);
void ZqRWLockLockRead(ZqRWLock *rw_lock);
void ZqRWLockUnlockRead(ZqRWLock *rw_lock);

ZQ_END_C_DECL

#endif // ZIQEAPI_RWLOCK_H
