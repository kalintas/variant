#pragma once

#include <type_traits>
#include <cstdint>
#include <new>


using variantIdType = std::uint8_t;

template<typename BaseType, typename Type>
struct isValidVariantType
{
    constexpr static bool value = std::conditional<
    std::is_trivially_constructible<BaseType, Type>::value, 
    std::is_same<BaseType, Type>, std::is_constructible<BaseType, Type>>::type::value;

};

template<typename Arg, typename ... Args>
union variantUnion
{
    variantUnion()  {}
    ~variantUnion() {}

    constexpr static variantIdType id = sizeof ... (Args) + 1;

    Arg arg;
    variantUnion<Args...> args;

    template<typename Type>
    constexpr variantIdType m_getId() const noexcept
    {
        if constexpr (std::is_same<Arg, Type>::value) return id;
        else return args.template m_getId<Type>();
    }

    template<typename Type>
    constexpr const Type& m_getValue() const
    {
        if constexpr(std::is_same<Arg, Type>::value)
        {
            return arg;
        }
        else
        {
            return args.template m_getValue<Type>();
        }
    }

    template<typename Type>
    constexpr variantIdType operator= (const Type& value) noexcept
    {
        if constexpr(!isValidVariantType<Arg, Type>::value) return args = value;
        else
        {
            if constexpr(std::is_trivially_constructible<Arg>::value) arg = value;
            else arg = *(new (this) Arg(value));

            return id;
        }
    }

    constexpr void m_deleteUnion(const variantIdType typeId) noexcept
    {
        if constexpr (!std::is_trivially_destructible<Arg>::value)
        {
            if(id == typeId)
            {
                arg.~Arg();
                return;
            } 
        }

        args.m_deleteUnion(typeId);
    }

};

template<typename Arg>
union variantUnion<Arg>
{
    variantUnion()  {}
    ~variantUnion() {}

    constexpr static variantIdType id = 1;

    Arg arg;

    template<typename Type>
    constexpr variantIdType m_getId() const noexcept
    {
        static_assert(std::is_same<Arg, Type>::value, "cannot find type");

        return id;
    }

    template<typename Type>
    constexpr const Type& m_getValue() const
    {
        static_assert(std::is_same<Arg, Type>::value, "loll2");

        return arg;
    }

    template<typename Type>
    constexpr variantIdType operator= (const Type& value) noexcept
    {
        static_assert(isValidVariantType<Arg, Type>::value, "lolll");

        if constexpr(std::is_trivially_constructible<Arg>::value) arg = value;
        else arg = *(new (this) Arg(value));

        return id;
    }
    
    constexpr void m_deleteUnion(const variantIdType typeId) noexcept
    {
        if constexpr (!std::is_trivially_destructible<Arg>::value)
        {
            if(id == typeId) arg.~Arg();
        }
    }
};

template<typename Arg, typename ... Args>
class variant
{
public:
    ~variant()
    {
        args.m_deleteUnion(m_id);
    }

    template<typename Type>
    constexpr variant& operator= (const Type& value) noexcept
    {
        args.m_deleteUnion(m_id);

        m_id = (args = value);
                
        return *this;
    }

    template<typename Type>
    [[nodiscard]] constexpr const Type& m_getValue() const
    {
        if(m_id != args.template m_getId<Type>())
        {
            throw "bad values";
        }

        return args.template m_getValue<Type>();
    }

private:

    variantUnion<Arg, Args...> args;
    variantIdType m_id = 0;

};
