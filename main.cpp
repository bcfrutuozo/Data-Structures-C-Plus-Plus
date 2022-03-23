#include "Timer.h"
#include "Array.h"
#include "List.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include <forward_list>
#include <array>
#include <list>

#include <iostream>

int main(int argc, char *argv[]) {
    Timer t;

    LinkedList<int> a{};
    for (auto it = a.cbegin(); it != a.cend(); ++it)
        std::cout << *it;
    std::cout << " | ";
    for (auto it = a.crbegin(); it != a.crend(); ++it)
        std::cout << *it;
    std::cout << std::endl;

    auto z = a.ToArray();
//    for (auto it = a.cbegin(); it != a.cend(); ++it)
//        std::cout << *it;
//    std::cout << " | ";
//    for (auto it = a.crbegin(); it != a.crend(); ++it)
//        std::cout << *it;
//    std::cout <<  std::endl;

    for (size_t i = 0; i < z.GetLength(); ++i)
        std::cout << z[i];


    return 0;
}