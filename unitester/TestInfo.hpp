#ifndef TESTINFO_HPP__
# define TESTINFO_HPP__

namespace Test
{
    typedef struct TestInfo
    {
        TestInfo() : name("unnamed"), is_successful(true), execution_time(0) {}
        TestInfo(std::string const & pname) : name(pname), is_successful(true), execution_time(0) {}

        std::string name;
        bool        is_successful;
        float       execution_time;
    } TestInfo;
}

#endif // TESTINFO_HPP__
