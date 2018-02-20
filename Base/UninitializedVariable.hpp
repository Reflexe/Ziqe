/**
 * @file UninitializedVariable.hpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2017 Shmuel Hazan
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
#ifndef UNINITIALIZEDVARIABLE_HPP
#define UNINITIALIZEDVARIABLE_HPP

#include "ConstructableStroage.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {


template<class T>
class UninitializedVariable
{
public:
    typedef Internal::ConstructableStorage<sizeof(T), alignof(T)>
            StorageType;

    UninitializedVariable();

    ZQ_DEFINE_CONST_AND_NON_CONST(const T*, T*, operator ->, (), {
        return mStorage;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST(const T&, T&, get, (), {
        return mStorage.getAs<T>();
    })

    template<class...Args>
    void construct(Args&&...args)
    {
    }

protected:
    ZQ_DEFINE_CONST_AND_NON_CONST(const T&, T&, super, (), {
        return get();
    })

private:
    StorageType mStorage;
};

} // namespace Base
ZQ_END_NAMESPACE

#endif // UNINITIALIZEDVARIABLE_HPP
