/**
 * @file Macros.h
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

/**
  @brief A simple macro to safely ignore variables without a compiler warning.
  */
#define ZQ_UNUSED(expr) (void)expr

/**
  @brief Two macros for implementing nulti-expression macros.

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
#define ZQ_SGMT_END } while(1)

/* I HATE misleading titles! */
#define inline_hint inline

#endif
