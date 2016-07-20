/**
 * @file SystemCalls.c
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
 *
 * Implementation of the SystemCalls.h ZiqeAPI for Linux.
 */

#include "../SystemCalls.h"

#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/syscalls.h>

static void init_system_calls_hook (void);

#ifdef __x86_64__
#define __NR_max 500
#define SYSCALL_ID_FROM_REGISTERS(regs) (regs->ax)
#define SYSCALL_FIRST_ARG(regs)         (RegisterType *)(&regs->di)
#define SYSCALL_MAX_ARGS                (5)
#define SYSCALL_INVOKE_REGS(regs)       (original_sys_call_table[regs->ax](regs->di, regs->si, regs->dx, regs->r10, regs->r8, regs->r9))
#define SYSCALL_INVOKE(id,regs)         (original_sys_call_table[id]((regs)[0], (regs)[1], (regs)[2], (regs)[3], (regs)[4], (regs)[5]))

typedef ZqRegisterType (* syscall_entry_t) (ZqRegisterType,
                                          ZqRegisterType,
                                          ZqRegisterType,
                                          ZqRegisterType,
                                          ZqRegisterType,
                                          ZqRegisterType);

static syscall_entry_t *sys_call_table = (void *)0xffffffff8143b1a0;

#endif

syscall_entry_t original_sys_call_table[__NR_max+1];

static ZqSystemCallHookType system_call_hook = NULL;
static char               is_syscall_hook_initilized = 0;

static void copy_sys_call_table(const ZqRegisterType *source,
                                ZqRegisterType *destination,
                                ZqRegisterType size)
{
    while (size--)
    {
        *destination++ = *source++;
    }
}

static void set_sys_call_table(ZqRegisterType *syscall_table,
                               ZqRegisterType new_value,
                               ZqRegisterType size) {
    while (size--)
    {
        *syscall_table++ = new_value;
    }
}

static void sys_call_table_make_rw(void **addr) {
        unsigned int lvl;

        pte_t *pte = lookup_address((unsigned long) addr, &lvl);

        if (pte -> pte &~ _PAGE_RW)
                pte -> pte |= _PAGE_RW;

        write_cr0(read_cr0() & (~ 0x10000));
}

static void sys_call_table_make_ro(void **addr) {
        unsigned int lvl;

        pte_t *pte = lookup_address((unsigned long) addr, &lvl);
        pte -> pte = pte -> pte &~_PAGE_RW;

        write_cr0(read_cr0() | 0x10000);
}

static ZqRegisterType hook_function (struct pt_regs *registers) {
    ZqRegisterType syscall_id = SYSCALL_ID_FROM_REGISTERS (registers);

    if (system_call_hook (syscall_id,
                          SYSCALL_FIRST_ARG (registers)) < 0)
    {
        return -EPERM;
    }

    return SYSCALL_INVOKE_REGS (registers);
}

static void init_system_calls_hook () {
    copy_sys_call_table ((ZqRegisterType *) sys_call_table,
                         (ZqRegisterType *)original_sys_call_table,
                         __NR_max);

    sys_call_table_make_rw((void **) sys_call_table);

    set_sys_call_table ((ZqRegisterType *)sys_call_table,
                        (ZqRegisterType) &hook_function,
                        __NR_max);

    sys_call_table_make_ro ((void **) sys_call_table);


    // Set all the system calls to our handler.
}

void ZqInitSystemCallsHook(ZqSystemCallHookType hook) {
    system_call_hook = hook;

    if (!is_syscall_hook_initilized)
        init_system_calls_hook ();
}

ZqRegisterType ZqCallSyscall(const ZqSystemCallIDType *id,
                             const ZqRegisterType *params)
{
    return SYSCALL_INVOKE (*id, params);
}
