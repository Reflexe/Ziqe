/**
 * @file Checks.hpp
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
#ifndef ZIQE_CHECKS_H
#define ZIQE_CHECKS_H

// For ZQ_UNUSED.
#include "Base/Macros.hpp"
#include "Base/Logger.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

#define ZQ_TEST_BUILD
#ifdef ZQ_TEST_BUILD
# define ZQ_ASSERT(expr) ZQ_SGMT_BEGIN if (expr) {} else { ::Ziqe::Base::Logger::logError (#expr " Failed"); } ZQ_SGMT_END

# define ZQ_ASSERT_REPORT(expr, msg) ZQ_SGMT_BEGIN if (expr) {} else { ::Ziqe::Base::Logger::logWarning (#expr " Failed: " msg); } ZQ_SGMT_END
# define ZQ_ASSERT_REPORT_NOT_REACHED(msg) ::Ziqe::Base::Logger::logWarning ("Reached: " msg)
# define ZQ_NOT_IMPLEMENTED() ZQ_ASSERT_REPORT_NOT_REACHED("NotImplemented reached")
#else
# define ZQ_ASSERT(expr) (void)(expr)

# define ZQ_ASSERT_REPORT(expr, msg) (void) msg, (void) expr
# define ZQ_ASSERT_REPORT_NOT_REACHED(msg) ZQ_UNUSED (msg)
# define ZQ_NOT_IMPLEMENTED() (void)0
#endif

// TODO: numeric_limits
template<class X, class Y>
bool Z_CHECK_ADD_OVERFLOW(X x, Y y)
{ // ((std::numeric_limits<decltype (x + y)>::max() - x) > y);
     return true;
}

#define DEBUG_CHECK_NOT_REACHED() ZQ_ASSERT(false)
#define DEBUG_CHECK_ADD_OVERFLOW(x, y) ZQ_ASSERT (Z_CHECK_ADD_OVERFLOW(x, y))

} // namespace Base
ZQ_END_NAMESPACE

#endif // ZIQE_CHECKS_H
