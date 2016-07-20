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

    template<typename ClassType>
    struct RegularObjectManager final : public ObjectManagerInterface
    {
        RegularObjectManager(ClassMemeberFunctionType<ClassType> function,
                            ClassType *classPointer)
            : mClassFunction{function}, mClassPointer{classPointer}
        {
        }

        ALLOW_MOVE (RegularObjectManager)
        DISALLOW_COPY (RegularObjectManager)

        virtual ReturnType invoke(ArgsTypes&&... args) override
        {
            return (mClassPointer->*mClassFunction)(std::forward<ArgsTypes>(args)...);
        }

        virtual bool isValid() const override
        {
            return mClassFunction && mClassPointer;
        }

        virtual ~RegularObjectManager() override = default;

    private:
       /// The object's pointer.
       ClassMemeberFunctionType<ClassType> mClassFunction;
       ClassType *mClassPointer;
    };

    template<typename ClassType>
    struct UniqueObjectManager final : public ObjectManagerInterface
    {
        UniqueObjectManager(ClassMemeberFunctionType<ClassType> function,
                            UniquePointer<ClassType> &&uniquePointer)
            : mClassFunction{function}, mClassPointer{std::move(uniquePointer)}
        {
        }

        ALLOW_MOVE (UniqueObjectManager)
        DISALLOW_COPY (UniqueObjectManager)

        virtual ReturnType invoke(ArgsTypes&&... args) override
        {
            return (mClassPointer.get()->*mClassFunction)(std::forward<ArgsTypes>(args)...);
        }

        virtual bool isValid() const override
        {
            return mClassFunction && !!mClassPointer;
        }

        virtual ~UniqueObjectManager() override = default;

    private:
       /// The object's pointer.
       ClassMemeberFunctionType<ClassType> mClassFunction;
       UniquePointer<ClassType> mClassPointer;
    };

//    template<typename ClassType>
//    struct WeakObjectManager final : public ObjectManagerInterface
//    {
//        WeakObjectManager(ClassMemeberFunctionType<ClassType> function,
//                          const SharedPointer<ClassType> &sharedPointer)
//            : mClassFunction{function}, mClassPointer{sharedPointer}
//        {
//        }

//        ALLOW_MOVE (WeakObjectManager)
//        DISALLOW_COPY (WeakObjectManager)

//        virtual ReturnType invoke(ArgsTypes&&... args) override{
//            auto sharedPointer = mClassPointer.lock();

//            return (sharedPointer.get()->*mClassFunction)(std::forward<ArgsTypes>(args)...);
//        }

//        virtual bool isValid() const override
//        {
//            return mClassFunction && !mClassPointer.expired();
//        }

//        virtual ~WeakObjectManager() override = default;

//    private:
//       /// The object's pointer.
//       ClassMemeberFunctionType<ClassType> mClassFunction;
//       WeakPointer<ClassType> mClassPointer;
//    };

    template<typename ClassType>
    struct SharedObjectManager final : public ObjectManagerInterface
    {
        SharedObjectManager(ClassMemeberFunctionType<ClassType> function,
                          const SharedPointer<ClassType> &sharedPointer)
            : mClassFunction{function}, mClassPointer{sharedPointer}
        {
        }

        ALLOW_MOVE (SharedObjectManager)
        DISALLOW_COPY (SharedObjectManager)

        virtual ReturnType invoke(ArgsTypes&&... args) override
        {
            return (mClassPointer.get()->*mClassFunction)(std::forward<ArgsTypes>(args)...);
        }

        virtual bool isValid() const override
        {
            return mClassFunction && mClassPointer;
        }

        virtual ~SharedObjectManager() override = default;

    private:
       /// The object's pointer.
       ClassMemeberFunctionType<ClassType> mClassFunction;
       SharedPointer<ClassType> mClassPointer;
    };

    UniquePointer<ObjectManagerInterface> mObjectManager;
    FunctionType *mFunction = nullptr;
public:

    ALLOW_MOVE (Callback)
    DISALLOW_COPY (Callback)

    /// @brief
    Callback() = default;

    /// Initilize from a regular function.
    Callback(FunctionType *function)
        : mObjectManager{},
          mFunction{function}
    {
    }

    /// Initlize from a shared pointer.
    template<class ClassType>
    Callback(ClassMemeberFunctionType<ClassType> function,
             const SharedPointer<ClassType> &sharedPointer)
        : mObjectManager{makeUnique<SharedObjectManager<ClassType>>(function, sharedPointer)}
    {
    }

    /// Initlize from a unique pointer.
    template<class ClassType>
    Callback(ClassMemeberFunctionType<ClassType> function,
             UniquePointer<ClassType> &&uniquePointer)
        : mObjectManager{makeUnique<UniqueObjectManager<ClassType>>(function, std::move(uniquePointer))}
    {
    }

    /// Initlize from a regular pointer.
    template<class ClassType>
    Callback(ClassMemeberFunctionType<ClassType> function,
             ClassType *pointer)
         : mObjectManager{makeUnique<RegularObjectManager<ClassType>>(function, pointer)}
    {
    }

    /// Initilize from an object manager.
    Callback (UniquePointer<ObjectManagerInterface> &&objectManager)
        : mObjectManager{std::move(objectManager)}
    {
    }

    ReturnType operator() (ArgsTypes&&... args) {
        // if it has a interface.
        if (mObjectManager) {
            CHECK (mObjectManager->isValid());

            return mObjectManager->invoke (std::forward<ArgsTypes>(args)...);
        } else {
            CHECK(mFunction);

            return mFunction (std::forward<ArgsTypes>(args)...);
        }
    }

    operator bool () {
        return ((mObjectManager == nullptr && mFunction != nullptr)
                || mObjectManager->isValid());
    }
};

} // namespace Ziqe

#endif // ZIQE_CALLBACK_H
