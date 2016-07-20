typedef int (*syscall_hook_t) (register_t *);

int set_syscall_hook(const syscall_hook_t *syscall_hook);
syscall_hook_t *get_current_syscall_hook();
