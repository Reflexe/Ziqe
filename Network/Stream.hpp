/**
 * @file Stream.hpp
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
#ifndef ZIQE_NET_STREAM_HPP
#define ZIQE_NET_STREAM_HPP

#include "Base/ExtendedVector.hpp"
#include "Base/Vector.hpp"

namespace Ziqe {
namespace Net {

class Stream
{
public:
    typedef Base::Vector<uint8_t> DataType;

    Stream() = default;
    virtual ~Stream();
    ZQ_ALLOW_COPY_AND_MOVE (Stream)


    virtual DataType receive() const = 0;

    virtual void send(const DataType &data) const = 0;

    /**
     * @brief The StreamVector struct
     *
     * @todo Take an onTimeout / onError function argument (Instead of using exceptions).
     */
    struct StreamVector {
        StreamVector(const Stream &stream)
            : mStream{stream}, mCurrentVector{receiveNewData ()}
        {
        }

        void increaseBegin(DifferenceType howMuch) {
            DEBUG_CHECK (howMuch <= static_cast<DifferenceType>(mCurrentVector.size ()));
            mCurrentVector.increaseBegin (howMuch);
        }

        uint8_t operator[] (const DifferenceType index) {
            // Out of range.
            if (static_cast<DifferenceType>(mCurrentVector.getSize ()) <= index) {
                // Receive more data and make the new extended vector to start from the index
                // -index (Then, fouther calls for operator[] would work).
                mCurrentVector = Base::ExtendedVector<uint8_t>{receiveNewData (),
                                                               static_cast<DifferenceType>(-index)};
            }

            return mCurrentVector[index];
        }

        Stream::DataType receiveNewData() const
        {
            return mStream.receive();
        }

    private:
        const Stream &mStream;

        Base::ExtendedVector<uint8_t> mCurrentVector;
    };

    StreamVector getStreamVector() const
    {
        return StreamVector{*this};
    }
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_STREAM_HPP
