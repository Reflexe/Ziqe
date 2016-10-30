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

#define _LINUX

#include "../SystemCalls.h"

#include "../Memory.h"

#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/kprobes.h>

#include <linux/thread_info.h>

#include <asm/syscalls.h>

#ifdef __x86_64__
#define SYSCALL_ID_FROM_REGISTERS(regs) (regs->ax)
#define SYSCALL_FIRST_ARG(regs)         (ZqRegisterType *)(&regs->di)
#define SYSCALL_RESULT_REGS(regs)       (regs->ax)
#define SYSCALL_MAX_ARGS                (5)
#define SYSCALL_INVOKE_REGS(regs)       (syscall_function(regs),(ZqRegisterType) regs->ax)

typedef void(*syscall_function_t)(struct pt_regs *regs);

/* #define SYSCALL_SYMBOL  "entry_SYSCALL64_slow_path" */

// The slow path's symbol, only threads with TIF_SYSCALL_TRACE go there.
// That will reduce the overhead for regular user mode tasks.
#define SYSCALL_SYMBOL "syscall_trace_enter" /* TODO: other syscalls entries - vDSO, 32bit */
#define SYSCALL_END_OFFSET 1
#endif

static ZqBool init_system_calls_hook(void);

ZqBool is_syscall_hook_initialized = ZQ_FALSE;
ZqSystemCallHookType syscall_hook  = NULL;

syscall_function_t syscall_function;

static int syscall_kprobe_pre_handler (struct kprobe *kprobe, struct pt_regs *regs) {
    // Call our hook.
    if (syscall_hook ((ZqThreadRegisters *) regs, &SYSCALL_RESULT_REGS (regs)) == ZQ_FALSE)
    {
        // The hook says that it don't want this thread.
        return 0;
    }

    // Skip the kernel's actual syscall.
    regs->ip = (ZqRegisterType) kprobe->addr + SYSCALL_END_OFFSET;

    /* Notify the kernel that we've changed regs->ip */
    return 1;
}

static int kprobe_empty_break_handler (struct kprobe *p, struct pt_regs *a)
{
    return 0;
}

struct kprobe syscall_kprobe = {
    .symbol_name = SYSCALL_SYMBOL,
    .pre_handler = syscall_kprobe_pre_handler,


    /* Force the kernel not to use optimization so we can
     * change the flow with regs->ip in the pre_handler.
     * (See Documentation/kprobes.txt:266)
     */
    .break_handler = kprobe_empty_break_handler
};

static ZqBool init_system_calls_hook () {
    if (register_kprobe (&syscall_kprobe) < 0)
        return ZQ_FALSE;

    return ZQ_TRUE;
}

void ZqInitSystemCallsHook(ZqSystemCallHookType hook) {
    syscall_hook = hook;

    if (is_syscall_hook_initialized == ZQ_FALSE)
        init_system_calls_hook ();
}

ZqRegisterType ZqCallSyscall(ZqThreadRegisters *regs)
{
    return SYSCALL_INVOKE_REGS ((struct pt_regs *) regs);
}
