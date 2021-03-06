/**
 * @file FunctionTools.hpp
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
#ifndef ZIQE_FUNCTIONTOOLS_H
#define ZIQE_FUNCTIONTOOLS_H

#include "Base/Macros.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

template<ZQ_TemplateVariable (sFunc), class CallbackT, class... Args>
void runAndCallback(const CallbackT &callback, Args&&... args)
{
    callback(sFunc(Base::forward<Args>(args)...));
}

template<class CallbackT, class ParameterType>
struct runAndCallParam {
    CallbackT callback;

    ParameterType functionParameter;
};

template<class ClassType, class FunctionSignature>
struct MemberFunction;

template<class ClassType, class ReturnType, class...ArgsTypes>
struct MemberFunction<ClassType, ReturnType(ArgsTypes...)>
{
    using Type=ReturnType (ClassType::*)(ArgsTypes...);

    template<Type *sFunction>
    static ReturnType Call (ClassType *classToCall, ArgsTypes&&...args)
    {
        classToCall->*sFunction (Base::forward<ArgsTypes>(args)...);
    }
};

template<ZQ_TemplateVariable (sFunc), class ParameterType>
void runAndCallbackSingleParameter(ParameterType *parameter)
{
    parameter->callback(sFunc(&parameter->functionParameter));

    delete parameter;
}

} // namespace Base
ZQ_END_NAMESPACE

#endif // ZIQE_FUNCTIONTOOLS_H
