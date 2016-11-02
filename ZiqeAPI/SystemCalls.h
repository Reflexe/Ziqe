/**
 * @file SystemCalls.h
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
#ifndef ZIQE_API_SYSTEMCALLS_H
#define ZIQE_API_SYSTEMCALLS_H

#include "Types.h"
#include "Macros.h"

typedef ZqBool (* ZqSystemCallHookType) (ZqThreadRegisters *regs,
                                         ZqRegisterType *result);

ZQ_BEGIN_C_DECL

/**
 * @brief ZiqeInitSystemCallsHook  Init a hook that will be called every time
 *                                 a thread with the flag WORKER is set.
 * @param hook
 */
void ZqInitSystemCallsHook(ZqSystemCallHookType hook);
void ZqUninitSystemCallsHook(void);

/**
 * @brief ZiqeCallSyscall  Blocking!!
 * @param id
 * @return
 */
ZqRegisterType ZqCallSyscall(ZqThreadRegisters *regs);

ZQ_END_C_DECL

#endif /* ZIQE_API_SYSTEMCALLS_H */
