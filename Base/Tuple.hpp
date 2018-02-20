/**
 * @file Tuple.hpp
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
#ifndef TUPLE_HPP
#define TUPLE_HPP

#include "Base/Macros.hpp"
#include "Base/Callback.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

namespace Internal {

template<SizeType Index, class T, class...Args>
class TupleContainer
{
    template<SizeType GetIndex, class This, class Next>
    struct GetHelper {
        decltype(auto) get(This &&_this, Next &&next)
        {
            return Base::forward<Next>(next).template get<GetIndex>();
        }
    };

    template<class This, class Next>
    struct GetHelper<Index, This, Next> {
        constexpr auto &get(This &_this, Next &next) &
        {
            return _this.value;
        }

        constexpr const auto &get(This &_this, Next &next) const&
        {
            return _this.value;
        }

        constexpr auto &&get(This &&_this, Next &&) &&
        {
            return Base::move(_this.value);
        }
    };

public:
    TupleContainer(T&&value, Args&&...args)
        : value{Base::forward<T>(value)},
          next{Base::forward<Args>(args)...}
    {
    }


    template<SizeType index>
    constexpr decltype(auto)
            get()
    {
        return GetHelper<index, decltype(*this), decltype(next)>{}.get(*this, Base::forward<decltype(next)>(next));
    }

private:
    Base::RemoveReference<T> value;
    TupleContainer<Index+1, Args...> next;
};

template<SizeType Index, class T>
class TupleContainer<Index, T>
{
public:
    TupleContainer(T&&value)
        : value{Base::forward<T>(value)}
    {
    }

    template<SizeType index>
    constexpr auto &get() &
    {
        return value;
    }

    template<SizeType index>
    constexpr const auto &get() const
    {
        return value;
    }

    template<SizeType index>
    constexpr auto &&get() &&
    {
        return Base::move(value);
    }

private:
    Base::RemoveReference<T> value;
};

}

template<class ...Args>
class Tuple
{
public:
    constexpr static SizeType Size = sizeof...(Args);

    Tuple(Args&&...values)
        : mContainer(Base::forward<Args>(values)...)
    {
    }

    constexpr SizeType size() const
    {
        return sizeof...(Args);
    }

    template<SizeType index>
    decltype(auto)
            get() &
    {
        static_assert(index < sizeof...(Args), "Index is out-of-bounds");

        return mContainer.template get<index>();
    }
    template<SizeType index>
    decltype(auto)
            get() const &
    {
        static_assert(index < sizeof...(Args), "Index is out-of-bounds");

        return mContainer.template get<index>();
    }

    template<SizeType index>
    decltype(auto)
            get() &&
    {
        static_assert(index < sizeof...(Args), "Index is out-of-bounds");

        return Base::move (mContainer).template get<index>();
    }

    Internal::TupleContainer<0, Base::RemoveReference<Args>...> mContainer;
};

/// An empty tuple
template<>
struct Tuple<>
{
    static constexpr SizeType Size = 0;
};


namespace Internal {
template <class T, class Tuple, SizeType... Ints>
constexpr T internalMakeFromTuple( Tuple&& t, IndexSequence<Ints...>)
{
    return T{Base::forward<Tuple>(t).template get<Ints>()...};
}

template<class Callable, class Tuple, SizeType... Ints>
constexpr decltype(auto)
        internalApply(Callable &&callable, Tuple&&tuple, IndexSequence<Ints...>)
{
    return callable(Base::forward<Tuple>(tuple).template get<Ints>()...);
}

template<class Callable, class Tuple, class Class, SizeType... Ints>
constexpr decltype(auto)
        internalApply(Callable &&callable, Tuple&&tuple, IndexSequence<Ints...>, Class *callableClass)
{
    return callableClass->*callable(tuple.template get<Ints>()...);
}

}

template<class T, class Tuple>
constexpr T makeFromTuple( Tuple&& t )
{
    return Internal::internalMakeFromTuple<T>(
                Base::forward<Tuple>(t),
                Base::MakeIndexSequence<Tuple::Size>{});
}

template<class Callable, class Tuple>
constexpr decltype(auto)
        apply(Callable &&callable, Tuple&&tuple)
{
    return Internal::internalApply(Base::forward<Callable>(callable),
                                   Base::forward<Tuple>(tuple),
                                   Base::MakeIndexSequence<Base::remove_reference<Tuple>::type::Size>{});
}

template<class Callable, class Tuple, class Class>
constexpr decltype(auto)
        apply(Callable &&callable, Tuple&&tuple, Class *callableClass)
{
    return Internal::internalApply(Base::forward<Callable>(callable),
                                   Base::forward<Tuple>(tuple),
                                   Base::MakeIndexSequence<Base::remove_reference<Tuple>::type::Size>{},
                                   callableClass);
}

} // namespace Base
ZQ_END_NAMESPACE

#endif // TUPLE_HPP
