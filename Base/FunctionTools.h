/**
 * @file FunctionTools.h
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
#ifndef ZIQE_FUNCTIONTOOLS_H
#define ZIQE_FUNCTIONTOOLS_H

#include "Base/Callback.h"
#include "Base/Macros.h"

namespace Ziqe {

template<TemplateVariable (sFunc), class CallbackT, class... Args>
void runAndCallback(const CallbackT &callback, Args&&... args)
{
    callback(sFunc(std::forward(args)...));
}


template<class CallbackT, class ParameterType>
struct runAndCallParam {
    CallbackT callback;

    ParameterType functionParameter;
};

template<TemplateVariable (sFunc), class ParameterType>
void runAndCallbackSingleParameter(ParameterType *parameter)
{
    parameter->callback(sFunc(&parameter->functionParameter));

    delete parameter;
}

} // namespace Ziqe

#endif // ZIQE_FUNCTIONTOOLS_H
