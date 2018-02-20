/**
 * @file Logging.h
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2017 Shmuel Hazan
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
#ifndef LOGGING_H
#define LOGGING_H

#include "CppCore/Macros.h"

#include <cstdio>

inline_hint void ZQ_SYMBOL(ZqLogText) (const char *text)
{
    ::puts (text);
}
inline_hint void ZQ_SYMBOL(ZqLogWarning) (const char *text)
{
    ::fputs (text, stderr);
}
inline_hint void ZQ_SYMBOL(ZqOnBug) (const char *text)
{
    ::fputs (text, stderr);
}

#endif // LOGGING_H
