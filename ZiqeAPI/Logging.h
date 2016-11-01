#ifndef ZIQEAPI_LOGGING_H
#define ZIQEAPI_LOGGING_H

#include "Macros.h"

ZQ_BEGIN_C_DECL

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

ZQ_END_C_DECL

#endif // ZIQEAPI_LOGGING_H
