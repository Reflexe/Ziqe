/**
 * @file ScopedContainer.hpp
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
#ifndef SCOPEDCONTAINER_HPP
#define SCOPEDCONTAINER_HPP

#include "Base/Macros.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

template<class _T, class _Deleter>
class _ScopedContainerBase
{
public:
    using T=_T;
    using Deleter=_Deleter;

    _ScopedContainerBase(T &&value, Deleter &&deleter = Deleter{})
        : mValue{Base::move(value)}, mDeleter{Base::move(deleter)}
    {
    }

    ~_ScopedContainerBase()
    {
        mDeleter (mValue);
    }

    _ScopedContainerBase(_ScopedContainerBase &&container)
        : mValue{Base::move(container.mValue)},
          mDeleter{Base::move(container.mDeleter)}
    {
        container.mValue = T();
    }

    _ScopedContainerBase(const _ScopedContainerBase &container)
        : mValue{container.mValue}, mDeleter{container.mDeleter}
    {
    }

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (T, mValue)

    _ScopedContainerBase &operator = (_ScopedContainerBase &&container) {
        Base::swap(container.mValue, mValue);
        Base::swap(container.mDeleter, mDeleter);

        return *this;
    }

    _ScopedContainerBase &operator = (const _ScopedContainerBase &container) {
        mValue = container.mValue;
        mDeleter = container.mDeleter;

        return *this;
    }

    operator T& ()
    {
        return mValue;
    }

    operator const T& () const
    {
        return mValue;
    }

private:
    T mValue;
    Deleter mDeleter;
};

template<class T, class Deleter>
class MovableScopedContainer : public _ScopedContainerBase<T, Deleter>
{
public:
    ZQ_DISALLOW_COPY(MovableScopedContainer)
    ZQ_ALLOW_MOVE(MovableScopedContainer)
    using _ScopedContainerBase<T, Deleter>::_ScopedContainerBase;

    using _ScopedContainerBase<T, Deleter>::operator =;
};

template<class T, class Deleter>
class CopyableScopedContainer : public _ScopedContainerBase<T, Deleter>
{
public:
    using _ScopedContainerBase<T, Deleter>::_ScopedContainerBase;

    using _ScopedContainerBase<T, Deleter>::operator =;
};

} // namespace Base
ZQ_END_NAMESPACE

#endif // SCOPEDCONTAINER_HPP
