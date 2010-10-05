
#include "TestSocket.hpp"
#include "TestSocketPool2.hpp"
#include "Socket.hpp"

#include <errno.h>
#include <stdio.h>

using namespace Test;
std::list<ISuite*>* ISuite::AllSuites = 0;

int main()
{
    {
        TestSocket test;
        test.run();
    }

    {
        TestSocketPool2 test;
        test.run();
    }
    return (0);
}
