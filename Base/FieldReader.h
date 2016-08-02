/**
 * @file FieldReader.h
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
#ifndef ZIQE_FIELDREADER_H
#define ZIQE_FIELDREADER_H

#include "Base/SharedVector.h"
#include "Base/Macros.h"

namespace Ziqe {

class FieldReader
{
public:
    FieldReader(SharedVector<Byte> &&vector);

    ALLOW_COPY_AND_MOVE (FieldReader)

    template <typename T, SizeType sByteLength=sizeof (T)>
    T readT() {
        DEBUG_CHECK (mVector.size() <= sByteLength);

        T value = *reinterpret_cast<T*> (&mVector[0]);
        mVector.increaseBegin (sByteLength);

        return value;
    }

private:
    SharedVector<Byte> mVector;
};

} // namespace Ziqe

#endif // ZIQE_FIELDREADER_H
