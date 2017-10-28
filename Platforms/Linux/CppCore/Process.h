/**
 * @file Process.h
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
 *
 * This file should be readed as a CPP file and a C file.
 */

#ifndef ZIQE_API_PROCESS_H
#define ZIQE_API_PROCESS_H

#include "CppCore/Macros.h"
#include "Types.h"
#include "Memory.h"

#include "ProcessConfig.h"

// You can implement these types as a real PID and TID or
// just pointers to the Process / Thread structure in the kernel.
typedef ZqRegisterType ZqProcessID;
typedef ZqRegisterType ZqThreadID;

typedef struct {
    ZqProcessID processID;

} ZqProcessInfo;

typedef struct
{
    int i;
} ZqProcessMemory;

typedef struct {
    ZqThreadID threadID;
    ZqThreadRegisters registers;
} ZqThreadInfo;

#define ZQ_NO_PROCESS_ID (0)

ZQ_BEGIN_C_DECL

/**
 * @brief ZqCreateProcessFromMemory  Create a process from a memory map.
 * @param memory  The memory for the process.
 * @param isMemoryMoved  After the call, will contain whether @arg memory
 *                       is still valid or get owned by the process.
 * @param processInfo [nullable] If not null, will contain the new process'
 *                    info on success.
 * @return The new process' ID on success, ZQ_NO_PROCESS_ID on fail.
 */
ZqProcessID ZqCreateProcessFromMemory (ZqProcessMemory *memory,
                                       ZqBool          *isMemoryMoved,
                                       ZqProcessInfo   *processInfo);

/**
 * @brief ZqGetProcessInfo  Get the process' info.
 * @param processID
 * @param processInfo Pointer to a ZqProcessInfo struct.
 * @return ZQ_TRUE on success; ZQ_FALSE on fail.
 */
ZqBool ZqGetProcessInfo (ZqProcessID processID,
                         ZqProcessInfo *processInfo);

/**
 * @brief ZqGetThreadInfo  Get the thread's info.
 * @param threadID
 * @param threadInfo Pointer to a ZqThreadInfo struct
 * @return ZQ_TRUE on success; ZQ_FALSE on fail.
 */
ZqBool ZqGetThreadInfo (ZqThreadID threadID,
                        ZqThreadInfo *threadInfo);

/**
 * @brief ZqCreateThread  Create a new thread for @arg processID.
 * @param processID
 * @param threadInfo
 * @return
 */
ZqThreadID ZqCreateThread (ZqProcessID processID,
                           ZqThreadInfo *threadInfo);

/**
 * @brief ZqGetCurrent  Get the thread ID of the caller.
 * @param threadInfo  [nullable] If not null, will contain
 *                    the current thread' info.
 * @return The current thread ID.
 */
ZqThreadID ZqGetCurrent (ZqThreadInfo *threadInfo);


/**
 * @brief ZqThreadCall  Run a function by a thread.
 * @param threadID
 * @param address
 * @param argument
 * @return
 */
ZqBool ZqThreadCall (ZqThreadID threadID,
                     ZqUserAddress address,
                     ZqRegisterType argument);

ZQ_END_C_DECL

#endif /* ZIQE_API_PROCESS_H */
