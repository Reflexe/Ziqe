int syscall_hook(resigter_t syscall)
{
	set_current_state(TASK_INTERRUPTIBLE);
        _add_sleeping_task(current);
}
