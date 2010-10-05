
#include "Suite.hpp"

class Example : public Test::Suite<Example>
{
private:
    void _unitTestOk();
    void _unitTestFail();
    void _multipleFailure();
    
public:
    Example() : Test::Suite<Example>("Example")
    {
        //this->_name = "Example";
        this->addTestFunction(&Example::_unitTestOk, "success test");
        this->addTestFunction(&Example::_unitTestFail, "failure test");
        this->addTestFunction(&Example::_multipleFailure, "multiple failure test");
    }
};

void Example::_unitTestOk()
{
    this->assert_test(1 == 1, "1 egal 1");
}

void Example::_unitTestFail()
{
    this->assert_test(1 == 0, "1 egal 0");
}

void Example::_multipleFailure()
{
    for (int i = 0; i < 10; i++)
        this->assert_test(i == 5, "i est different de 5");
}

// Register Example
// Example test;
