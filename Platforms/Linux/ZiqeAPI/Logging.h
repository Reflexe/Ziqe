#ifndef ZIQEAPI_LOGGING_H
#define ZIQEAPI_LOGGING_H

#include "Platforms/Macros.h"

ZQ_BEGIN_C_DECL

// TODO: support formatting.
//       need to make sure it is not a UB with different archs.

/**
   @brief Log text message.
   @param string    The message.
 */
void ZqLogText (const char *string);


/**
   @brief Log a warning message.
   @param string    The message.
 */
void ZqLogWarning (const char *string);

/**
   @brief Report a serious problem in the program excution.
   @param string    a message about the bug.
 */
void ZqOnBug (const char *string);

#define ZQ_LOG_TEXT(string) ZqLogText (string)
#define ZQ_LOG_WARNING(string) ZqLogWarning (string)
#define ZQ_ON_BUG(string) ZqOnBug (string)

#define _call_x_on_fail(expr, x) ZQ_SGMT_BEGIN if (expr) {} else { x(#expr " Failed!") } ZQ_SGMT_END

#define ZQ_BUG_IF_NOT(expr) ZQ_SGMT_BEGIN if (expr) {} else { ZQ_ON_BUG(#expr " Failed!"); } ZQ_SGMT_END
#define ZQ_WARN_IF_NOT(expr) ZQ_SGMT_BEGIN if (expr) {} else { ZQ_LOG_WARNING(#expr " Failed!"); } ZQ_SGMT_END
#define ZQ_LOG_IF_NOT(expr, msg) ZQ_SGMT_BEGIN if (expr) {} else { ZQ_LOG_TEXT(#expr #msg); } ZQ_SGMT_END

ZQ_END_C_DECL

#endif // ZIQEAPI_LOGGING_H
