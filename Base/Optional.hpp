#ifndef ZIQE_BASE_OPTIONAL_HPP
#define ZIQE_BASE_OPTIONAL_HPP

#include "Base/Types.hpp"
#include "Base/Macros.hpp"
#include "Base/Checks.hpp"

// For Base::CustomStorageConstructor.
#include "Base/Constructor.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

/**
  @brief A strict-aliasing-safe boost::optional implementation.
 */
template<class T, class _Constructor=CustomStorageConstructor<T, char>>
class Optional
{
public:
    typedef _Constructor ConsturctorType;

    /**
       @brief  Default constructor: this optional is not valid.
     */
    Optional()
        : mConsturctedObject{nullptr}
    {
    }

    ~Optional()
    {
        destruct ();
    }

    ZQ_DISALLOW_COPY (Optional)

    template<class... Args>
    Optional(Args&&... args)
    // Construct T{args} on mStorageData.
        : mConsturctedObject{mConstructor.construct (mStorageData, Base::forward<Args>(args)...)}
    {
    }

    /**
       @brief Move constructor
       @param optional

       Construct a new @tparam T from @a optional 's object.
     */
    Optional(Optional &&optional)
        : mConsturctedObject{mConstructor.construct (Base::move (optional.get ()))}
    {
    }

    /**
       @brief Move operator
       @param optional
       @return

       Construct a new @tparam T from @a optional 's object.
     */
    Optional &operator =(Optional &&optional) {
        construct (Base::move (optional.get ()));
        return *this;
    }

    template<class... Args>
    Optional &operator =(Args&&... args) {
        construct(Base::forward<Args>(args)...);
        return *this;
    }

    /**
      @brief Destruct the current data the costruct a new @tparam T from @tparam Args .
     */
    template<class... Args>
    void construct (Args&&... args) {
        destruct ();

        mConsturctedObject = mConstructor.construct (mStorageData, Base::forward<Args>(args)...);
    }

    /**
       @brief If it is exist, destruct the current data.
     */
    void destruct ()  {
        if (isValid ()) {
            mConstructor.destruct (mConsturctedObject);
            mConsturctedObject = nullptr;
        }
    }

    /**
      @brief If exists, return the current data.

      @note undefined behaviour if this optional is not valid.
      */
    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, get, (), {
        ZQ_ASSERT(isValid ());
        return *mConsturctedObject;
    })

    /**
       @brief Check if this optional is valid.
     */
    operator bool () const
    {
        return isValid ();
    }

    /**
       @brief Check if this optional is valid.
     */
    bool isValid () const
    {
        return mConsturctedObject != nullptr;
    }

    /// Pointer-Like operators
    ///
    // ->
    ZQ_DEFINE_CONST_AND_NON_CONST (const T*, T*, operator ->, (), {
        ZQ_ASSERT (isValid ());
        return mConsturctedObject;
    })

    // *
    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, operator *, (), {
        return get ();
    })

private:
    ConsturctorType mConstructor;

    /**
       @brief  Storage for the optional data.
     */
    alignas(T) char mStorageData[sizeof (T)];

    /**
       @brief  Pointer to the constucted object, or nullptr.
     */
    T *mConsturctedObject;
};

} // namespace Base
ZQ_END_NAMESPACE

#endif // ZIQE_BASE_OPTIONAL_HPP
