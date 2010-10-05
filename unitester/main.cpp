
#include <list>
#include <iostream>

#include "Example.hpp"

using namespace Test;
std::list<ISuite*>* ISuite::AllSuites = 0;

int main()
{
    Example e;
    e.run();

#ifdef _WIN32
    std::cin.get();
#endif
    return (0);
}
