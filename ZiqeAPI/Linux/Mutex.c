/**
 * @file Mutex.c
 * @author shrek0 (shrek0.tk@gmail.com)
 *
 * Ziqe: copyright (C) 2016 shrek0
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

#include "Mutex.h"

#include <linux/mutex.h>

void ZqMutexInit(ZqMutex *mutex) {
    *mutex = ZqAllocateVirtual (sizeof (struct mutex));

    mutex_init ((struct mutex *) *mutex);
}

void ZqMutexDeinit(ZqMutex *mutex)
{
    ZqDeallocateVirtual ((ZqAddress) *mutex);
}

void ZqMutexLock(ZqMutex *mutex)
{
    mutex_lock ((struct mutex *) *mutex);
}

void ZqMutexUnlock(ZqMutex *mutex)
{
    mutex_unlock ((struct mutex *) *mutex);
}

ZqBool ZqMutexTryLock(ZqMutex *mutex)
{
    return mutex_trylock ((struct mutex *) *mutex) == 1 ? ZQ_TRUE : ZQ_FALSE;
}
