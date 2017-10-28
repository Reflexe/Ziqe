/**
 * @file Logger.hpp
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
#ifndef ZIQE_LOGGER_H
#define ZIQE_LOGGER_H

#include "CppCore/Logging.h"
#include "Macros.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

class Logger
{
public:
    /**
       @brief Log and report an error.
       @param string    The message.

       @note This function should get used in a very rare cases in release:
             on some architectures, it will stop the system.
     */
    static void logError (const char *string)
    {
        ZQ_SYMBOL(ZqOnBug) (string);
    }

    /**
       @brief Log a warning message.
       @param string  The message.
     */
    static void logWarning (const char *string)
    {
        ZQ_SYMBOL(ZqLogWarning) (string);
    }

    /**
       @brief Log a regular information message.
       @param string  The message.
     */
    static void logMessage (const char *string)
    {
        ZQ_SYMBOL(ZqLogText) (string);
    }

private:
    Logger();

};

#define ZQ_LOG(msg) ::Ziqe::Utils::Logger::logMessage (": \"" msg "\"\n")
#define ZQ_WARNING(msg) ::Ziqe::Utils::Logger::logWarning (": \"" msg "\"\n")
#define ZQ_ERROR(msg) ::Ziqe::Utils::Logger::logError (": \"" msg "\"\n")

}
ZQ_END_NAMESPACE

#endif // ZIQE_LOGGER_H
