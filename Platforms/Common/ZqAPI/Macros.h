/**
 * @file Macros.h
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
 *
 * This file should be readed as a CPP file and a C file.
 */
#ifndef ZIQE_API_MACROS_H
#define ZIQE_API_MACROS_H

/**
  @brief A few macros for declaring function for C and C++.

  To enable declaring functions for both C and C++,
  we should declare them in C linkage on C++.
  */
#ifdef __cplusplus
# define ZQ_BEGIN_C_DECL extern "C" {
# define ZQ_END_C_DECL }
#else
# define ZQ_BEGIN_C_DECL
# define ZQ_END_C_DECL
#endif

#ifdef __cplusplus
# define ZQ_NULL nullptr
#else
# define ZQ_NULL NULL
#endif

/**
  @brief A simple macro to safely ignore variables without a compiler warning.
  */
#define ZQ_UNUSED(expr) (void)expr

/**
  @brief Two macros for implementing nultiple-expression macros.

  To enable the user to use a macro as an expression, it should
  be one expression:
  @code
  #define FOO foo()

  // OK
  FOO;
  @endcode

  However, with more that one expression, it is a problem. Therefore, we should
  use these macros:
  @code
  #define DO_A_THING(foo, bar) ZQ_SGMT_BEGIN int i; while(foo < i) { ++foo, ++bar ; } ZQ_SGMT_END

  // OK; would be a problem without ZQ_SGMT_BEGIN and ZQ_SGMT_END in DO_A_THING.
  DO_A_THING(foo, bar);
  @endcode
  */
#define ZQ_SGMT_BEGIN do {
#define ZQ_SGMT_END } while(0)

/**
  It is not simply A ## B because that we want macro arguments to be
  replace before the actual combination of the arguments (e.g. 1_A instead of
  VAR_NAME_A).
  */
#define ZQ_CAT(arg1, arg2) _ZQ_CAT(arg1, arg2)
#define _ZQ_CAT(arg1, arg2) arg1 ## arg2

/* I HATE misleading titles! */
#define inline_hint inline

#define ZQ_VERSION_BUGFIX 1
#define ZQ_VERSION_MINOR 0
#define ZQ_VERSION_MAJOR 0

#define ZQ_ADD_UNDERSCORE(arg) ZQ_CAT(_, arg)

#define ZQ_SYMBOL_VERSION ZQ_CAT(ZQ_CAT(ZQ_ADD_UNDERSCORE(ZQ_VERSION_MAJOR),\
    ZQ_ADD_UNDERSCORE(ZQ_VERSION_MINOR)), ZQ_ADD_UNDERSCORE(ZQ_VERSION_BUGFIX))

#define ZQ_FUNCTION_STR __PRETTY_FUNCTION__

#define ZQ_BEGIN_NAMESPACE namespace Ziqe {\
    inline namespace ZQ_SYMBOL_VERSION {

#define ZQ_END_NAMESPACE } }

/**
  @brief A per version symbol.
  */
#define ZQ_SYMBOL(name) ZQ_CAT(ZQ_SYMBOL_VERSION, name)

#define ZQ_ARRAY_LENGTH(symbol) (sizeof (symbol[0]) * sizeof (symbol))

#endif
