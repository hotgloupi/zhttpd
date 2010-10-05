#ifndef SUITE_HPP__
# define SUITE_HPP__

# include <list>
# include <string>

# include "TestInfo.hpp"
# include "ConsoleOutput.hpp"
# include "TesterClock.hpp"

namespace Test
{
    class ISuite
    {
    public:
        static std::list<ISuite*>* AllSuites;

        virtual ~ISuite() {}
        virtual void run() = 0;
    };

    template <typename T, class Output = ConsoleOutput>
    class Suite : public Output, public ISuite
    {
    public:
        typedef void (T::*testFunction_t)();

    protected:
        std::string _name;

    private:
        std::list<std::pair<TestInfo, testFunction_t> > _test_suite;
        static std::list<std::string> _failure_msgs;

        T* _selfRealType;

    public:
        Suite(std::string const& name) : _name(name)
        {
            //std::cout << "Register suite: " << this->_name << std::endl;
            this->_selfRealType = reinterpret_cast<T*>(this);

            if (ISuite::AllSuites == 0)
                ISuite::AllSuites = new std::list<ISuite*>();
            ISuite::AllSuites->push_back(this->_selfRealType);
        }

        virtual ~Suite() {}

        void addTestFunction(testFunction_t tf, std::string const & name)
        {
            this->_test_suite.push_back(std::pair<TestInfo, testFunction_t>(TestInfo(name), tf));
        }

        void assert_test(bool expr, std::string const & failureMsg)
        {
            if (!expr)
                this->_failure_msgs.push_back(failureMsg);
        }

        void run()
        {
            this->print_before_suite(this->_name);
            typename std::list<std::pair<TestInfo, testFunction_t> >::iterator it = this->_test_suite.begin();
            typename std::list<std::pair<TestInfo, testFunction_t> >::iterator ite = this->_test_suite.end();

            while (it != ite)
            {
                this->print_before_test(it->first);

                TesterClock t;
                (this->_selfRealType->*(it->second))();
                it->first.execution_time = t.getElapsedTime();

                if (!this->_failure_msgs.empty())
                {
                    it->first.is_successful = false;
                    this->print_failure(this->_failure_msgs, it->first);
                    this->_failure_msgs.clear();
                }
                this->print_after_test(it->first);
                ++it;
            }
            std::list<TestInfo>* tmp_list = getTestInfoList();
            this->print_after_suite(this->_name, tmp_list);
            delete tmp_list;
        }

    private:
        std::list<TestInfo>* getTestInfoList()
        {
            std::list<TestInfo>* list = new std::list<TestInfo>;
            typename std::list<std::pair<TestInfo, testFunction_t> >::iterator it = this->_test_suite.begin();
            typename std::list<std::pair<TestInfo, testFunction_t> >::iterator ite = this->_test_suite.end();

            while (it != ite)
            {
                list->push_back(it->first);
                ++it;
            }
            return list;
        }
    };

    template<typename T, class Output> std::list<std::string> Suite<T, Output>::_failure_msgs;
}



#endif // SUITE_HPP__

