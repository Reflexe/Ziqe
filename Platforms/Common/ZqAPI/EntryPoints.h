/**
 * @file EntryPoints
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
#ifndef ENTRYPOINTS_H
#define ENTRYPOINTS_H

#include "ZqAPI/Macros.h"

ZQ_BEGIN_C_DECL

void ZQ_SYMBOL(ZqOnLoad) (void *private_data_ptr);

void ZQ_SYMBOL(ZqOnUnload) (void *private_data_ptr);

ZQ_END_C_DECL

#endif // ENTRYPOINTS_H
