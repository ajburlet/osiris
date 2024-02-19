#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OException.h"

class OAPI ONonCopiable
{
public:
    /**
     @brief Default class constructor.
     */
    ONonCopiable() = default;

    /**
     @brief Deleted copy constructor.
     */
    ONonCopiable(const ONonCopiable& aOther) = delete;

    /**
     @brief Class destructor.
     */
    virtual ~ONonCopiable() = default;
    
    /**
     @brief Deleted copy assignment operator.
     */
    ONonCopiable& operator=(const ONonCopiable& aOther) = delete;
};

/**
 @brief Templated interface that estabilishes a non-copiable class.
 @tparam T The non-copiable class that derives this.
 */
template <class T, bool Clonable=false>
class ONonCopiableT : public ONonCopiable
{
public:
    /**
     @brief Default class constructor.
     */
    ONonCopiableT() = default;

    /**
     @brief Class destructor.
     */
    virtual ~ONonCopiableT() = default;

    /**
     @brief Clones the object into another.
     @param aDestination Destination object. 
     */
    virtual void cloneTo (T& aDestination) const;

    /**
     @brief Creates a clone of the object.
     @return A newly allocated object with the contents copied.
     */
    T* clone () const;
};

template <class T, bool Clonable>
inline void ONonCopiableT<T,Clonable>::cloneTo (T& aDestination) const
{
    if constexpr(!Clonable) {
        throw OEx("Attempted to clone an unclonable object");
    }
}

template <class T, bool Clonable>
inline T* ONonCopiableT<T,Clonable>::clone() const
{
    if constexpr(Clonable) {
        auto obj = new T();
        OExPointerCheck(obj);
        cloneTo(*obj);
        return obj;
    } else {
        throw OEx("Attempted to clone an unclonable object.");
        return nullptr;
    }
}