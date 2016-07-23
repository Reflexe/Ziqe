/**
 * @file Callback.h
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
#ifndef ZIQE_CALLBACK_H
#define ZIQE_CALLBACK_H

#include "Memory.h"
#include "Macros.h"
#include "Checks.h"

namespace Ziqe {

template <typename F>
class Callback;

template <typename ReturnType_, typename... ArgsTypes>
class Callback<ReturnType_(ArgsTypes...)>
{
public:
    typedef ReturnType_ ReturnType;
    typedef ReturnType FunctionType(ArgsTypes...);

private:
    /// @brief Get a pointer to member function type for ClassName.
    template<typename ClassType>
    using ClassMemeberFunctionType=ReturnType (ClassType::*)(ArgsTypes...);

    struct ObjectManagerInterface
    {
        virtual ReturnType invoke(ArgsTypes&&... args) = 0;

        virtual bool isValid() const{ return true; }

        virtual ~ObjectManagerInterface() = 0;
    };

    template<FunctionType *sFunction>
    struct RegularFunctionObjectManager final : public ObjectManagerInterface
    {
        RegularFunctionObjectManager()
        {
        }

        ALLOW_MOVE (RegularFunctionObjectManager)
        DISALLOW_COPY (RegularFunctionObjectManager)

        virtual ReturnType invoke(ArgsTypes&&... args) override
        {
            return sFunction (std::forward<ArgsTypes> (args)...);
        }

        virtual ~RegularFunctionObjectManager() override = default;
    };

    template<typename ClassType, ClassMemeberFunctionType<ClassType> sClassFunction>
    struct RegularObjectManager final : public ObjectManagerInterface
    {
        RegularObjectManager(ClassType *classPointer)
            : mClassPointer{classPointer}
        {
        }

        ALLOW_MOVE (RegularObjectManager)
        DISALLOW_COPY (RegularObjectManager)

        virtual ReturnType invoke(ArgsTypes&&... args) override
        {
            return (mClassPointer->*sClassFunction)(std::forward<ArgsTypes>(args)...);
        }

        virtual bool isValid() const override
        {
            return !!mClassPointer;
        }

        virtual ~RegularObjectManager() override = default;

    private:
       /// The object's pointer.
       ClassType *mClassPointer;
    };

    template<typename ClassType, ClassMemeberFunctionType<ClassType> sClassFunction>
    struct UniqueObjectManager final : public ObjectManagerInterface
    {
        UniqueObjectManager(UniquePointer<ClassType> &&uniquePointer)
            : mClassPointer{std::move(uniquePointer)}
        {
        }

        ALLOW_MOVE (UniqueObjectManager)
        DISALLOW_COPY (UniqueObjectManager)

        virtual ReturnType invoke(ArgsTypes&&... args) override
        {
            return (mClassPointer.get()->*sClassFunction)(std::forward<ArgsTypes>(args)...);
        }

        virtual bool isValid() const override
        {
            return !!mClassPointer;
        }

        virtual ~UniqueObjectManager() override = default;

    private:
       /// The object's pointer.
       UniquePointer<ClassType> mClassPointer;
    };

    template<typename ClassType, ClassMemeberFunctionType<ClassType> sClassFunction>
    struct SharedObjectManager final : public ObjectManagerInterface
    {
        SharedObjectManager(const SharedPointer<ClassType> &sharedPointer)
            : mClassPointer{sharedPointer}
        {
        }

        ALLOW_MOVE (SharedObjectManager)
        DISALLOW_COPY (SharedObjectManager)

        virtual ReturnType invoke(ArgsTypes&&... args) override
        {
            return (mClassPointer.get()->*sClassFunction)(std::forward<ArgsTypes>(args)...);
        }

        virtual bool isValid() const override
        {
            return !!mClassPointer;
        }

        virtual ~SharedObjectManager() override = default;

    private:
       /// The object's pointer.
       SharedPointer<ClassType> mClassPointer;
    };

    UniquePointer<ObjectManagerInterface> mObjectManager;
public:

    ALLOW_MOVE (Callback)
    DISALLOW_COPY (Callback)

    /// @brief Default constructor.
    Callback() = default;

    /// Initilize from a regular function.
    template<FunctionType sFunction>
    Callback(StaticVariable<FunctionType, sFunction>)
        : mObjectManager{new RegularFunctionObjectManager <sFunction>}
    {
    }

    /// Initlize from a shared pointer.
    template<class ClassType, ClassMemeberFunctionType<ClassType> sFunction>
    Callback(StaticVariable<ClassMemeberFunctionType<ClassType>, sFunction>,
             const SharedPointer<ClassType> &sharedPointer)
        : mObjectManager{new SharedObjectManager<ClassType, sFunction>(sharedPointer)}
    {
    }

    /// Initlize from a unique pointer.
    template<class ClassType, ClassMemeberFunctionType<ClassType> sFunction>
    Callback(StaticVariable<ClassMemeberFunctionType<ClassType>, sFunction>,
             UniquePointer<ClassType> &&uniquePointer)
        : mObjectManager{new UniqueObjectManager<ClassType, sFunction>(std::move(uniquePointer))}
    {
    }

    /// Initlize from a regular pointer.
    template<class ClassType, ClassMemeberFunctionType<ClassType> sFunction>
    Callback(StaticVariable<ClassMemeberFunctionType<ClassType>, sFunction>, ClassType *pointer)
         : mObjectManager{new RegularObjectManager<ClassType, sFunction>(pointer)}
    {
    }

    /// Initilize from an object manager.
    Callback (UniquePointer<ObjectManagerInterface> &&objectManager)
        : mObjectManager{std::move(objectManager)}
    {
    }

    ReturnType operator() (ArgsTypes&&... args) {
        DEBUG_CHECK (mObjectManager->isValid());

        return mObjectManager->invoke (std::forward<ArgsTypes>(args)...);
    }

    operator bool ()
    {
        return (mObjectManager && mObjectManager->isValid());
    }
};

} // namespace Ziqe

#endif // ZIQE_CALLBACK_H
