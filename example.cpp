#include <iostream>
#include <utility>
#include <type_traits>
#include <string>


template<typename T>
struct some_value_impl{};


template<typename T>
class has_some_value_function
{
    using one = char;
    struct two
    {
        char x[2];
    };

    template<typename C>
    static one test(decltype(&C::get_value));
    template<typename C>
    static two test(...);

public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
};

template<typename T>
static constexpr bool has_some_value_v = has_some_value_function<some_value_impl<T>>::value;

struct some_value {
    template<typename T>
    operator T() const{
        static_assert(has_some_value_v<T>, "conversion to T not supported");
        return some_value_impl<T>::get_value();
    }
};
some_value get_some_value() 
{ 
    return some_value{}; 
}


template <>
struct some_value_impl<int> 
{
    static int get_value() 
    {
        return 154;
    }
};

template <>
struct some_value_impl<std::string>
{
    static std::string get_value()
    {
        return "some value ...";
    }
};


static_assert(has_some_value_v<int>);
static_assert(has_some_value_v<std::string>);
static_assert(!has_some_value_v<double>);
static_assert(!has_some_value_v<float>);



int main()
{
    int i = get_some_value(); // returns: 154
    std::string s = get_some_value(); // returns some value ...

    std::cout << "values: \n"
        << "int i = " << i << '\n' 
        << "std::string s = " << s << '\n';

    return 0;
}
