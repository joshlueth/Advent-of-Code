#include <iostream>

int main() {
    long standard_value = __cplusplus;
    std::cout << "The value of __cplusplus is: " << standard_value << std::endl;

    if (standard_value == 199711L) {
        std::cout << "C++98/C++03 standard" << std::endl;
    } else if (standard_value == 201103L) {
        std::cout << "C++11 standard" << std::endl;
    } else if (standard_value == 201402L) {
        std::cout << "C++14 standard" << std::endl;
    } else if (standard_value == 201703L) {
        std::cout << "C++17 standard" << std::endl;
    } else if (standard_value == 202002L) {
        std::cout << "C++20 standard" << std::endl;
    } else if (standard_value == 202302L) {
        std::cout << "C++23 standard" << std::endl;
    } else {
        std::cout << "Unknown or newer C++ standard" << std::endl;
    }

    return 0;
}