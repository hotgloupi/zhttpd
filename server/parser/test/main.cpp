
#include "Test_Parser.hpp"

using namespace Test;
std::list<ISuite*>* ISuite::AllSuites = 0;

int main()
{
    Test_Parser test;
    test.run();
    return (0);
}
