/**
 * @file Logger.cpp
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
#include "Logger.hpp"

#include "ZiqeAPI/Logging.h"

namespace Ziqe {

Logger::Logger()
{

}

void Logger::logError(const char *string)
{
    ZqOnBug (string);
}

void Logger::logWarning(const char *string)
{
    ZqLogWarning (string);
}

void Logger::logMessage(const char *string)
{
    ZqLogText (string);
}

} // namespace Ziqe
