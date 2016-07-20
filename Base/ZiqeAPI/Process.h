/**
 * @file Process.h
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
 *
 * This file should be readed as a CPP file and a C file.
 */

#ifndef ZIQE_API_PROCESS_H
#define ZIQE_API_PROCESS_H

#include "Macros.h"
#include "Types.h"
#include "Memory.h"

typedef ZqRegisterType ZqProcessID;
typedef ZqRegisterType ZqThreadID;

typedef struct {
    ZqProcessID processID;

} ZqProcessInfo;

typedef struct {
    ZqThreadID threadID;
} ZqThreadInfo;

typedef struct {
    ZqMemoryEntry *entries;
    ZqRegisterType entriesLength;


} ZqProcessMemory;

ZQ_BEGIN_C_DECL

ZqProcessID ZqCreateProcessFromMemory (ZqProcessMemory *memory,
                                       ZqBool          *isMemoryMoved,
                                       ZqProcessInfo  **processInfo);

ZqBool ZqGetProcessInfo (ZqProcessID processID, ZqProcessInfo *processInfo);

ZqBool ZqGetThreadInfo (ZqThreadID threadID, ZqThreadInfo *threadInfo);

ZqThreadID ZqCreateThread (ZqProcessID processID, ZqThreadInfo **threadInfo);

ZQ_END_C_DECL

#endif /* ZIQE_API_PROCESS_H */
