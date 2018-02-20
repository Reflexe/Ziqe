/**
 * @file ZQObject.hpp
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
#ifndef ZQOBJECT_HPP
#define ZQOBJECT_HPP

#include "Base/Macros.hpp"
#include "Base/Tuple.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {
    template<class BaseClass, class Value>
    struct BaseClassInit
    {
        typedef BaseClass Type;

        Value value;
    };

    template<class Value>
    struct DataInit
    {
        Value value;
    };

    template<class...Args>
    decltype(auto) makeDataInit(Args&&...args)
    {
        // I should be comparing two type packs.
        static_assert(Base::IsSame<ParameterPack<Args...>, ParameterPack<Base::RemoveReference<Args>...>>::value,
                      "Please use Base::move for arguments Base::makeDataInit");

        return DataInit<Tuple<Args...>>{{Base::forward<Args>(args)...}};
    }

    template<class BaseClass, class...Args>
    decltype(auto) makeBaseClassInit(Args&&...args)
    {
        static_assert(Base::IsSame<ParameterPack<Args...>, ParameterPack<Base::RemoveReference<Args>...>>::value,
                      "Please use Base::move for arguments Base::makeBaseClassInit");

        return BaseClassInit<BaseClass, Tuple<Args...>>{{Base::forward<Args>(args)...}};
    }

#define ZQ_OBJECT(name)\
    ZQ_DEFINE_D_FORWARDING_CONSTRUCTOR(name) \
    ZQ_ALLOW_EXPECTED();

    /**
      @brief Define a consturctor that forward its arguments to
             member @a forward_to.
      */

#define ZQ_DEFINE_FORWARDING_CONSTRUCTOR(class_name, member_forward_to) \
    template<class BaseClass, class TupleType, class DataTuple> \
    explicit class_name(::Ziqe::Base::BaseClassInit<BaseClass, TupleType> &&baseClassInit, ::Ziqe::Base::DataInit<DataTuple> &&dataInit)\
        : BaseClass{::Ziqe::Base::makeFromTuple<BaseClass> (::Ziqe::Base::move(baseClassInit.value))},\
          member_forward_to{::Ziqe::Base::makeFromTuple<decltype(member_forward_to)> (::Ziqe::Base::move(dataInit.value))}\
        {\
        }\
    template<class DataTuple>\
    explicit class_name(::Ziqe::Base::DataInit<DataTuple> &&dataInit)\
        : member_forward_to{::Ziqe::Base::makeFromTuple<decltype(member_forward_to)> (::Ziqe::Base::move(dataInit.value))}\
    {\
    }\
    template<class BaseClass, class TupleType>\
    explicit class_name(::Ziqe::Base::BaseClassInit<BaseClass, TupleType> &&baseClassInit)\
        : BaseClass{::Ziqe::Base::makeFromTuple<BaseClass> (::Ziqe::Base::move(baseClassInit.value))}\
    {\
    }
#define ZQ_DEFINE_D_FORWARDING_CONSTRUCTOR(class_name) ZQ_DEFINE_FORWARDING_CONSTRUCTOR(class_name, d)
}
ZQ_END_NAMESPACE


#endif // ZQOBJECT_HPP
