/**
 * @file IRawStream.hpp
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
#ifndef IRAWSTREAM_HPP
#define IRAWSTREAM_HPP

#include "Utils/Macros.hpp"
#include "Utils/Expected.hpp"
#include "Utils/Vector.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

// Read and write raw bytes.
template<class SendError, class ReceiveError, class RawUnit = Vector<uint8_t>>
class IRawStream
{
public:
    IRawStream();
    ZQ_PURE_VIRTUAL_DESTRUCTOR (IRawStream)

    ZQ_ALLOW_COPY_AND_MOVE (IRawStream)

    virtual Expected<SizeType, SendError> send(const RawUnit &unit) = 0;

    virtual Expected<RawUnit, ReceiveError> receive () = 0;
};

} // namespace Utils
ZQ_END_NAMESPACE

#endif // IRAWSTREAM_HPP
