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
#include <linux/kprobes.h>

#include <linux/thread_info.h>

#include <asm/syscalls.h>

static ZqBool init_system_calls_hook(void);

#ifdef __x86_64__
#define SYSCALL_ID_FROM_REGISTERS(regs) (regs->ax)
#define SYSCALL_FIRST_ARG(regs)         (ZqRegisterType *)(&regs->di)
#define SYSCALL_RESULT_REGS(regs)       (regs->ax)
#define SYSCALL_MAX_ARGS                (5)
#define SYSCALL_INVOKE_REGS(regs)       (original_sys_call_table[regs->ax](regs->di, regs->si, regs->dx, regs->r10, regs->r8, regs->r9))
#define SYSCALL_INVOKE(id,regs)         (original_sys_call_table[id]((regs)[0], (regs)[1], (regs)[2], (regs)[3], (regs)[4], (regs)[5]))

#define SYSCALL_SYMBOL  "entry_SYSCALL64_slow_path"
#define SYSCALL_END_OFFSET 1
#endif

ZqBool is_syscall_hook_initialized = ZQ_FALSE;
ZqSystemCallHookType syscall_book  = NULL;

static int kprobe_pre_handler (struct kprobe *kprobe, struct pt_regs *regs) {
    if (test_thread_flag (TIF_SYSCALL_TRACE)) {
        // Call our hook.
        SYSCALL_RESULT_REGS(regs) = syscall_book (SYSCALL_ID_FROM_REGISTERS (regs),
                                                  SYSCALL_FIRST_ARG (regs));

        // Skip the kernel's actual syscall.
        regs->ip = (ZqRegisterType) kprobe->addr + SYSCALL_END_OFFSET;

        /* Notify the kernel that we've changed regs->ip */
        return 1;
    }

    return 0;
}

static int kprobe_empty_break_handler (struct kprobe *p, struct pt_regs *a)
{
    return 0;
}

struct kprobe kprobe = {
    .symbol_name = SYSCALL_SYMBOL
};

static ZqBool init_system_calls_hook () {
    kprobe.pre_handler = kprobe_pre_handler;

    /* Force the kernel not to use optimization so we can
     * change regs->ip in the pre_handler.
     * (See Documentation/kprobes.txt:266)
     */
    kprobe.break_handler = kprobe_empty_break_handler;

    if (register_kprobe (&kprobe) < 0)
        return ZQ_FALSE;

    return ZQ_TRUE;
}

void ZqInitSystemCallsHook(ZqSystemCallHookType hook) {
    syscall_book = hook;

    if (is_syscall_hook_initialized == ZQ_FALSE)
        init_system_calls_hook ();
}

ZqRegisterType ZqCallSyscall(const ZqSyscallParameter *parameter)
{
//    return SYSCALL_INVOKE (*id, params);
}
