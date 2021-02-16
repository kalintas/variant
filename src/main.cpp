#include "variant.h"

#include <iostream>
#include <string>
#include <variant>

struct someObject
{
    someObject()  { std::cout << "con of someObject\n";   }
    ~someObject() { std::cout << "decon of someObject\n"; }
};

template<typename Arg, typename ... Args>
[[nodiscard]] inline constexpr std::variant<Arg, Args...> 
getStandardVariant(const variant<Arg, Args...>&) noexcept { return {}; } 

int main()
{
    variant<float, int, double, someObject, std::string> v;

    const auto stdVariant = getStandardVariant(v);

    std::cout << "std::variant size = " << 
    sizeof(stdVariant) << "\nvariant size = " << sizeof(v) << "\n";

    // someObject is created
    v = someObject{};

    // someObject is destroyed
    v = "test, test\n";
    
    try
    {
        // reading value of type std::string
        const auto value = v.m_getValue<std::string>();

        std::cout << value;

        // throws an exception when trying to read int
        // std::cout << v.m_getValue<int>();
    }
    catch(const char* e)
    {
        std::cerr << "exception : " << e;
    }
    
    return 0;
}
