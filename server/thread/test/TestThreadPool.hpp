
#include <sstream>

#include "Suite.hpp"
#include "String.hpp"
#include "ThreadPool.hpp"

#ifdef _WIN32
# define sleep(x) Sleep(x * 1000)
# define usleep(x) Sleep((x) / 1000)
#endif // _WIN32


class TestThreadPoolTask : public zhttpd::ITask
{
private:
    int _i;

public:
    TestThreadPoolTask(int i) : _i(i) {}

    virtual void run()
    {
        ::usleep(100000);
        this->_i = -1;
    }

    int GetI() const { return this->_i; }
};

class TestThreadPoolTask2 : public zhttpd::ITask
{
private:
    int _i;

public:
    TestThreadPoolTask2() : _i(0) {}

    virtual void run()
    {
        ::usleep(100000);
        ++this->_i;
    }

    int GetI() const { return this->_i; }
};

class TestThreadPool : public Test::Suite<TestThreadPool>
{
private:
    void _unitTest5Threads()
    {
        try
        {
            zhttpd::ThreadPool *pool = new zhttpd::ThreadPool(5);
            std::list<zhttpd::SmartPtr<TestThreadPoolTask> > list;
            for (int i = 0; i < 50; ++i)
            {
                zhttpd::SmartPtr<TestThreadPoolTask> ttp(new TestThreadPoolTask(i));
                list.push_back(ttp);
                pool->queueTask(ttp);
            }
            ::sleep(2);
            std::list<zhttpd::SmartPtr<TestThreadPoolTask> >::iterator it = list.begin();
            std::list<zhttpd::SmartPtr<TestThreadPoolTask> >::iterator ite = list.end();
            for (; it != ite; ++it)
                if ((*it)->GetI() != -1)
                    break;
            this->assert_test(it == ite, "50 tasks (100ms) / 5 threads in 2 seconds");
            delete pool;
        }
        catch (std::exception& ex)
        {
            this->assert_test(false, std::string("Exception: ") + ex.what());
        }
    }

    void _unitTest100Threads()
    {
        try
        {
            zhttpd::ThreadPool *pool = new zhttpd::ThreadPool(100);
            std::list<zhttpd::SmartPtr<TestThreadPoolTask> > list;
            for (int i = 0; i < 1000; ++i)
            {
                zhttpd::SmartPtr<TestThreadPoolTask> ttp(new TestThreadPoolTask(i));
                list.push_back(ttp);
                pool->queueTask(ttp);
            }
            ::sleep(5);
            std::list<zhttpd::SmartPtr<TestThreadPoolTask> >::iterator it = list.begin();
            std::list<zhttpd::SmartPtr<TestThreadPoolTask> >::iterator ite = list.end();
            for (; it != ite; ++it)
                if ((*it)->GetI() != -1)
                    break;
            this->assert_test(it == ite, "1000 tasks (100ms) / 100 threads in 5 seconds");
            delete pool;
        }
        catch (std::exception& ex)
        {
            this->assert_test(false, std::string("Exception: ") + ex.what());
        }
    }

    void _uintTest5ThreadsSameWork()
    {
        try
        {
            zhttpd::ThreadPool *pool = new zhttpd::ThreadPool(5);
            zhttpd::SmartPtr<TestThreadPoolTask2> ttp(new TestThreadPoolTask2());
            for (int i = 0; i < 50; ++i)
                pool->queueTask(ttp);
            ::sleep(2);
            this->assert_test(ttp->GetI() == 50, ttp->GetI() + std::string("/50 tasks (100ms) / 5 threads in 2 seconds"));
            delete pool;
        }
        catch (std::exception& ex)
        {
            this->assert_test(false, std::string("Exception: ") + ex.what());
        }
    }

    void _uintTest100ThreadsSameWork()
    {
        try
        {
            zhttpd::ThreadPool *pool = new zhttpd::ThreadPool(100);
            zhttpd::SmartPtr<TestThreadPoolTask2> ttp(new TestThreadPoolTask2());
            for (int i = 0; i < 1000; ++i)
                pool->queueTask(ttp);
            ::sleep(5);
            this->assert_test(ttp->GetI() == 1000, ttp->GetI() + std::string("/1000 tasks (100ms) / 100 threads in 5 seconds"));
            delete pool;
        }
        catch (std::exception& ex)
        {
            this->assert_test(false, std::string("Exception: ") + ex.what());
        }
    }

public:
    TestThreadPool() : Test::Suite<TestThreadPool>("Thread Pool")
    {
        this->addTestFunction(&TestThreadPool::_unitTest5Threads, "5 Threads working");
        this->addTestFunction(&TestThreadPool::_unitTest100Threads, "100 Threads working");
        this->addTestFunction(&TestThreadPool::_uintTest5ThreadsSameWork, "5 Threads working (same task)");
        this->addTestFunction(&TestThreadPool::_uintTest100ThreadsSameWork, "100 Threads working (same task)");
    }
};

// TestThreadPool registerTest;
