/**
 * @file Stream.cpp
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
#include "Stream.hpp"

namespace Ziqe {
namespace Net {

Stream::~Stream()
{
}

Stream::OutputStreamVector::~OutputStreamVector()
{
}

Stream::OutputStreamVector::OutputStreamVector(SizeType autoSendSize)
    : mAutoSendDataSize{autoSendSize}
{

}

Stream::OutputStreamVector::OutputStreamVector(DataType &&vectorInitilizer, SizeType autoSendSize)
    : mVector{Base::move (vectorInitilizer)}, mAutoSendDataSize{autoSendSize}
{

}

Stream::InputStreamVector::~InputStreamVector()
{
}

void Stream::InputStreamVector::increaseBegin(SizeType howMuch) {
    ZQ_ASSERT (howMuch <= mVectorsSize);

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

uint8_t Stream::InputStreamVector::operator[] (SizeType index) {
    ZQ_ASSERT (index >= 0);
    ZQ_ASSERT (mVectorsSize > index);

    // Out of range of the current vector, we need to find the one for this index.
    if (mCurrentVector.getSize () <= index) {
        index -= mCurrentVector.size ();

        for (auto &vector : mVectors) {
            if (vector.size () > index) {
                return vector[index];
            }

            index -= vector.size ();
        }

        // Should not happen: OOB read, should check with makeSureHasLength before.
        DEBUG_CHECK_NOT_REACHED ();
        return 0;
    } else {
        return mCurrentVector[index];
    }
}

bool Stream::InputStreamVector::hasLength(const SizeType length) {
    while (mVectorsSize < length) {
        bool result = receiveNewData ();

        if (! result)
            return false;
    }

    return true;
}

bool Stream::InputStreamVector::receiveNewData() {
    auto newData = receiveData ();
    if (newData) {
        mVectorsSize += newData->size ();
        mVectors.emplace_back(Base::move (newData.get ()));
        return true;
    } else {
        return false;
    }
}

void Stream::InputStreamVector::switchToNextVector() {
    mCurrentVector = Base::ExtendedVector<uint8_t>{Base::move (mVectors.front ())};
    mVectors.pop_front ();
}

} // namespace Net
} // namespace Ziqe
