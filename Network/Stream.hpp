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
    typedef Base::Vector<uint8_t> OutputType;

    enum class ReceiveError {
        Timeout,
        ParseError,
        Other
    };

    struct InputStreamVector {
        InputStreamVector()
            : mCurrentVector{}, mVectorsSize{0}
        {
        }

        virtual ~InputStreamVector();

        void increaseBegin(SizeType howMuch);

        uint8_t operator[] (SizeType index);

        bool hasLength (const SizeType length);

        SizeType size () const
        {
            return mVectorsSize;
        }

    protected:
        virtual Base::Expected<DataType, ReceiveError>
        receiveData () const = 0;

        /**
           @brief Try to receive a new data vector and add it to mVectors.
           @return true if a vector added, false if not (receive error).
         */
        bool receiveNewData();

        /**
           @brief Remove the current vector and get a new one from mVectors.
         */
        void switchToNextVector ();

        Base::ExtendedVector<uint8_t> mCurrentVector;
        Base::LinkedList<Base::Vector<uint8_t>> mVectors;
        SizeType mVectorsSize;
    };


    struct OutputStreamVector {
    public:
        void sync ()
        {
            sendCurrentSegment ();
        }

        SizeType size() const
        {
            return mVector.size ();
        }

        ZQ_ALLOW_COPY_AND_MOVE (OutputStreamVector)
        virtual ~OutputStreamVector();

        void expand (SizeType howMuch) {
            // If index is OOB,
            if (isAutoSendSizePassed ())
                sync ();
            else
                mVector.expand (howMuch);
        }

        void increaseBegin (SizeType howMuch)
        {
            mVector.increaseBegin (howMuch);
        }

        uint8_t &operator [] (SizeType index)
        {
            return mVector[index];
        }

    protected:
        OutputStreamVector(SizeType autoSendSize);
        OutputStreamVector(DataType &&vectorInitilizer, SizeType autoSendSize);

    private:
        bool isAutoSendSizePassed () const
        {
            return mVector.size () >= mAutoSendDataSize;
        }

        virtual void sendCurrentSegment () = 0;

    protected:
        Base::ExtendedVector<uint8_t> mVector;

        /**
           @brief On TCP, MSS-TCP-Header len.
                  On UDP, MSS.
         */
        SizeType mAutoSendDataSize;
    };

    Stream() = default;
    virtual ~Stream();
    ZQ_ALLOW_COPY_AND_MOVE (Stream)

    virtual Base::Pair<Address, Port> getStreamInfo () const = 0;

    virtual Base::UniquePointer<InputStreamVector> getInputStreamVector() const = 0;

    virtual Base::UniquePointer<OutputStreamVector> getOutputStreamVector() const = 0;
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_STREAM_HPP
