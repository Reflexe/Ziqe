/**
 * @file Stream.hpp
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
#ifndef ZIQE_NET_STREAM_HPP
#define ZIQE_NET_STREAM_HPP

#include "Base/ExtendedVector.hpp"
#include "Base/LinkedList.hpp"
#include "Base/Vector.hpp"
#include "Base/Expected.hpp"

#include "Base/Socket.hpp"

namespace Ziqe {
namespace Net {

class Stream
{
public:
    typedef ZqPort Port;
    typedef Zq_in6_addr Address;

    typedef Base::Vector<uint8_t> DataType;

    enum class ReceiveError {
        Timeout,
        Other
    };

    Stream() = default;
    virtual ~Stream();
    ZQ_ALLOW_COPY_AND_MOVE (Stream)


    virtual Base::Expected<DataType, ReceiveError> receive() const = 0;

    virtual void send(const DataType &data) const = 0;

    virtual Base::Pair<Address, Port> getStreamInfo () const = 0;

    /**
     * @brief The StreamVector struct
     */
    struct StreamVector {
        StreamVector(const Stream &stream)
            : mStream{&stream}, mCurrentVector{}, mVectorsSize{0}
        {
            receiveNewData ();
        }

        void increaseBegin(SizeType howMuch) {
            DEBUG_CHECK (howMuch <= mVectorsSize);

            // We should remove one or more vectors.
            while (howMuch >= mCurrentVector.size ()) {
                mVectorsSize -= mCurrentVector.size ();
                howMuch -= mCurrentVector.size ();
                switchToNextVector ();
            }

            // Move the required offset in the next vector.
            mCurrentVector.increaseBegin (howMuch);
            mVectorsSize -= howMuch;
        }

        uint8_t operator[] (SizeType index) {
            DEBUG_CHECK (index >= 0);
            DEBUG_CHECK (mVectorsSize > index);

            // Out of range of the current vector, we need to find the one for this index.
            if (mCurrentVector.getSize () <= index) {
                index -= mCurrentVector.size ();

                for (auto &vector : mVectors) {
                    if (vector.size () > index) {
                        return vector[index];
                    }

                    index -= vector.size ();
                }

                // Should not happen.
                DEBUG_CHECK_NOT_REACHED ();
                return 0;
            } else {
                return mCurrentVector[index];
            }
        }

        bool makeSureHasLength (const SizeType length) {
            while (mVectorsSize < length) {
                bool result = receiveNewData ();

                if (! result)
                    return false;
            }

            return true;
        }

        SizeType size () const
        {
            return mVectorsSize;
        }

    private:
        /**
           @brief Try to receive a new data vector and add it to mVectors.
           @return true if a vector added, false if not (receive error).
         */
        bool receiveNewData() {
            auto newData = mStream->receive();
            if (newData) {
                mVectorsSize += newData->size ();
                mVectors.emplace_back(Base::move (newData.get ()));
                return true;
            } else {
                return false;
            }
        }

        /**
           @brief Remove the current vector and get a new one from mVectors.
         */
        void switchToNextVector () {
            mCurrentVector = Base::ExtendedVector<uint8_t>{Base::move (mVectors.front ())};
            mVectors.pop_front ();
        }

        Base::RawPointer<const Stream> mStream;

        Base::ExtendedVector<uint8_t> mCurrentVector;
        Base::LinkedList<Base::Vector<uint8_t>> mVectors;
        SizeType mVectorsSize;
    };

    StreamVector getStreamVector() const
    {
        return StreamVector{*this};
    }
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_STREAM_HPP
