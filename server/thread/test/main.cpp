
#include <list>
#include <iostream>

//#include "Suite.hpp"
#include "TestThreadPool.hpp"

using namespace Test;
std::list<ISuite*>* ISuite::AllSuites = 0;

int main()
{
    TestThreadPool t;
    t.run();

#ifdef _WIN32
    std::cin.get();
#endif
    return (0);
}
