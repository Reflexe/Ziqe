/**
 * @file DriverContext.hpp
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
#ifndef COMMON_DRIVERCONTEXT_HPP
#define COMMON_DRIVERCONTEXT_HPP

#include "Base/Macros.hpp"
#include "Base/UniquePointer.hpp"

ZQ_BEGIN_NAMESPACE
namespace Common {
namespace OS {

class AbstractDriverContext
{
public:
    AbstractDriverContext();


    struct IDestructable {
    protected:
        IDestructable()
        {
        }

    public:
        //ZQ_PURE_VIRTUAL_DESTRUCTOR(IDestructable)
        virtual ~IDestructable();
    };

    template<class T>
    T &getUserData ()
    {
        return *static_cast<T*>(mUserData.get ());
    }

    void setUserData (Base::UniquePointer<IDestructable> &&destructable)
    {
        mUserData = Base::move(destructable);
    }

private:
    Base::UniquePointer<IDestructable> mUserData;
};

} // namespace OS
} // namespace Common
ZQ_END_NAMESPACE

#endif // COMMON_DRIVERCONTEXT_HPP
