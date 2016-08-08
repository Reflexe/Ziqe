/**
 * @file Checks.h
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
#ifndef ZIQE_CHECKS_H
#define ZIQE_CHECKS_H

#include <limits>

namespace Ziqe {

// CHECK things that shouldn't happend.
#define DEBUG_CHECK(expr) (void)(expr)
#define DEBUG_CHECK_NOT_REACHED() DEBUG_CHECK(false)

#define DEBUG_CHECK_REPORT (expr)
#define DEBUG_CHECK_REPORT_NOT_REACHED()

template<class X, class Y>
bool Z_CHECK_ADD_OVERFLOW(X x, Y y)
{
     return ((std::numeric_limits<decltype (x + y)>::max() - x) < y);
}

#define DEBUG_CHECK_ADD_OVERFLOW(x, y) DEBUG_CHECK (Z_CHECK_ADD_OVERFLOW(x, y))

} // namespace Ziqe

#endif // ZIQE_CHECKS_H
