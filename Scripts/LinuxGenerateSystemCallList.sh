#!/usr/env sh
python2 LinuxGenerateSystemCallList.py /lib/modules/$(uname -r)/build/include/linux/syscalls.h -f 'STR_SYSCALL_NAME ({}),' > ../ZiqeAPI/Linux/SystemCallsList.inc.h
