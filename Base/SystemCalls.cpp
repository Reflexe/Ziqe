/**
 * @file SystemCalls.cpp
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
 */
#include "SystemCalls.h"

namespace Ziqe {

ZqRegisterType SystemCalls::runSystemCall(LocalThread &thread, SystemCalls::SystemCallID id,
                                        const ZqRegisterType *parameters) {
    // TODO: switch to @thread.

    return ZqCallSyscall (&id, parameters);
}

void SystemCalls::setSystemCallHook(ZqSystemCallHookType hook)
{
    ZqInitSystemCallsHook (hook);
}

void SystemCalls::init() {
    /*
     * There two options: one is a custom kernel, and just change the tracehook_report_syscall_entry
     * function.
     *
     * The other option is to patch the kernel when the module's loading.
     * We can change all of the system call table to our function and trace them.
     * We can also patch the code of tracehook_report_syscall_entry.
     */


}

} // namespace Ziqe
