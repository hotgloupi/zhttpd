#ifndef CONSOLEOUTPUT_HPP__
# define CONSOLEOUTPUT_HPP__

# include <iostream>
# include <iomanip>
# include "Suite.hpp"

namespace Test
{
    class ConsoleOutput
    {
    public:
        ConsoleOutput() {}
        virtual ~ConsoleOutput() {}

        void print_before_suite(std::string const & name)
        {
            std::cout << "\033[01;34mLauching test suite: " << name << "\033[22;37m" << std::endl << std::endl;
        }

        void print_after_suite(std::string const & name, std::list<TestInfo>* testInfoList)
        {
            float nbSuccess = 0;
            float nbTests = 0;
            float totalTime = 0;
            std::list<TestInfo>::iterator it = testInfoList->begin();
            std::list<TestInfo>::iterator ite = testInfoList->end();

            while (it != ite)
            {
                nbTests++;
                totalTime += it->execution_time;
                if (it->is_successful)
                    nbSuccess++;
                ++it;
            }
            std::cout << std::endl << "\033[01;34m" << name  << ": " << std::setprecision(0) << nbSuccess << "/" << nbTests << " (" << (nbSuccess / nbTests * 100) << "%) successful tests in " << std::setprecision(6) << totalTime << " s\033[22;37m" << std::endl;
        }

        void print_before_test(TestInfo const & ti)
        {
            std::cout << "\033[22;34mTesting " << ti.name << " ...\033[22;37m" << std::endl;
        }

        void print_after_test(TestInfo const & ti)
        {
            if (ti.is_successful)
                std::cout << "\033[22;32m\tOK in " << std::fixed << std::setprecision(6) << ti.execution_time << " s\033[22;37m" << std::endl;
        }

        void print_failure(std::list<std::string>& failure_msgs, TestInfo const & ti)
        {
            std::list<std::string>::iterator it = failure_msgs.begin();
            std::list<std::string>::iterator ite = failure_msgs.end();

            std::cout << "\033[22;31m\tFAIL in " << std::fixed << std::setprecision(6) << ti.execution_time << " s" << std::endl;
            while (it != ite)
            {
                std::cout << "\tfailure: " << *it << std::endl;
                ++it;
            }
            std::cout << "\033[22;37m";
        }
    };
}

#endif // CONSOLEOUTPUT_HPP__

