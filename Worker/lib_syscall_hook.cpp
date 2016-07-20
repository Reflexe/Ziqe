#include "lib_syscall_hook.h"

int
set_syscall_hook(const syscall_hook_t *syscall_hook) {
#ifdef CONFIG_X86_64
         set_intr_gate(X86_TRAP_PF, page_fault);
#endif
}

syscall_hook_t *
get_current_syscall_hook()
{

}
