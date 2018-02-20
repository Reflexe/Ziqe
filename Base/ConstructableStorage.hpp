/**
 * @file ConstructableStorage.hpp
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
#ifndef CONSTRUCTABLESTORAGE_HPP
#define CONSTRUCTABLESTORAGE_HPP


#include "Base/Macros.hpp"
#include "Base/Constructor.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {
namespace Internal {

/**
 * A piece of memory you can consturct objects on.
 */
template<SizeType size, SizeType alignment>
class ConstructableStorage {
public:
    template<class T, class ...Args>
    T *construct(Args&&... args) {
        CustomStorageConstructor<T,char> consturctor;

        return consturctor.construct(mStorageData, Base::forward<Args>(args)...);
    }

    template<class T>
    void destruct () {
        CustomStorageConstructor<T,char> consturctor;
        consturctor.destruct(getAs<T>());
    }

    // Does not violate the strict aliasing rule:
    // mStorageData is no longer a array of chars but
    // a T.
    template<class T>
    T *getAs()
    {
        return static_cast<T*>(static_cast<void*>(mStorageData));
    }

    template<class T>
    T *getAs() const
    {
        return static_cast<const T*>(static_cast<void*>(mStorageData));
    }

private:
    /**
       @brief  Storage for the optional data.
     */
    alignas(alignment) char mStorageData[size];
};

} // namespace Internal
} // namespace Base
ZQ_END_NAMESPACE

#endif // CONSTRUCTABLESTORAGE_HPP
