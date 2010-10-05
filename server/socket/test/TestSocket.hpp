
#ifndef __TESTSOCKET_HPP__
# define __TESTSOCKET_HPP__

# include "Socket.hpp"
# include "Suite.hpp"

class TestSocket : public Test::Suite<TestSocket>
{
private:
    void _listenerCreate()
    {
        try {
            ZHTTPD::Socket(0, 1);
            this->assert_test(false, "Should throw with stupid port 1");
        } catch (...) {}
        try {
            ZHTTPD::Socket(0, 4000);
        } catch (...) {
            this->assert_test(false, "Should work with port 4000");
        }
    }

public:
    TestSocket() : Test::Suite<TestSocket>("Socket tests")
    {
        this->addTestFunction(&TestSocket::_listenerCreate, "Create a listener socket");
    }
};

#endif /* !__TESTSOCKET_HPP__ */

