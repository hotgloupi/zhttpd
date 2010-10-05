
#include "TestUtils.hpp"

using namespace Test;
std::list<ISuite*>* ISuite::AllSuites = 0;

int main()
{
    TestUtils test;
    test.run();
    return (0);
}
