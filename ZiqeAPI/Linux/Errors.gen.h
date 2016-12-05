/**
 * @file Errors.gen.h
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
 */
#ifndef ERRORS_GEN_H
#define ERRORS_GEN_H

#include <sys/errno.h>

/* Error types & values */
typedef int ZqError;

#define ZQ_E_AGAIN EAGAIN
#define ZQ_E_CON_RESET ECONNRESET
#define ZQ_E_DEST_REQUIRED EDESTADDRREQ
#define ZQ_E_MEM_FAULT EFAULT
#define ZQ_E_INVALID_ARG EINVAL
#define ZQ_E_SIZE EMSGSIZE
#define ZQ_E_OK 0

#endif // ERRORS_GEN_H
