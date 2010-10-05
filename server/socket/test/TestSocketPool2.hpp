
#ifndef TESTSOCKETPOOL2_HPP_
# define TESTSOCKETPOOL2_HPP_

#include <vector>

# include "SocketPool.hpp"
# include "Suite.hpp"
# include "SelfConnector.hpp"
# include "thread/Thread.hpp"
# include "utils/Timer.hpp"

using namespace ZHTTPD;

class TestSocketPool2 : public Test::Suite<TestSocketPool2>
{
private:
    class HandlerALaCon : public ISocketHandler
    {
    public:
        virtual ~HandlerALaCon() {}
        void handle(Socket& s, ISocketHandler::SocketEvent)
        {
            std::cout << "handler de HandlerALaCon" << std::endl;
            delete s.accept();
        }
    };

    class HandlerPourTimeOut : public ISocketHandler
    {
    public:
        virtual ~HandlerPourTimeOut() {}
        void handle(Socket&, ISocketHandler::SocketEvent e)
        {
            std::cout << "Timeout !!!" << std::endl;
            std::cout << "Elapsed time : " << t.getElapsedTime() << std::endl;
            std::cout << "Socket Event : " << e << std::endl;
        }
        Timer t;
    };

    void _basicTest()
    {
        SocketPool poule;
        {
            Thread t(&poule);
            HandlerALaCon con;
            try
            {
                std::vector<Socket*> socketz;
                size_t const nb_sockets = 100;

                std::cout << "Creation de 100 sockets" << std::endl;
                for (size_t i = 0; i < nb_sockets ; ++i)
                    socketz.push_back(new Socket(0, 4000 + i));

                std::cout << "register 100 handlers" << std::endl;
                for (size_t i = 0; i < nb_sockets ; ++i)
                    poule.registerHandler(ISocketHandler::CAN_READ, *socketz[i], con, &HandlerALaCon::handle);

                std::vector<SelfConnector*> connectors;
                std::vector<Thread*> threads;

                std::cout << "Creation et lancement de 100 selfconnectors" << std::endl;
                for (size_t i = 0 ; i < nb_sockets ; ++i)
                {
                    connectors.push_back(new SelfConnector(4000 + i));
                    threads.push_back(new Thread(connectors.back()));
                }

                Timer::sleep(100);

                for (size_t i = 0; i < nb_sockets ; ++i)
                    delete connectors[i]->getSocket();

                std::cout << "delete 100 everything" << std::endl;
                for (size_t i = 0; i < nb_sockets ; ++i)
                {
                    delete socketz[i];
                    delete connectors[i];
                    delete threads[i];
                }
            }
            catch (std::exception& e)
            {
                std::cout << "fuckin exception: " << e.what() << std::endl;
                this->assert_test(false, "Exception de merde");
            }
        }
        Timer::sleep(200);
    }

    void _timeoutTest()
    {
        Timer::sleep(100);
        SocketPool poule;
        {
            Thread t(&poule);
            try
            {
                Socket s(0, 5000);
                std::cout << "Timeout dans 500ms : " << std::endl;
                HandlerPourTimeOut h;
                poule.registerHandler(ISocketHandler::CAN_READ, s, h, &HandlerPourTimeOut::handle, 500);
                Timer::sleep(1000);
            }
            catch (std::exception& e)
            {
                std::cout << "fuckin exception: " << e.what() << std::endl;
                this->assert_test(false, "Exception de merde");
            }
        }
        Timer::sleep(200);
    }

public:
    TestSocketPool2() : Test::Suite<TestSocketPool2>("SocketPool test 2")
    {
        this->addTestFunction(&TestSocketPool2::_basicTest, "test a la con");
        this->addTestFunction(&TestSocketPool2::_timeoutTest, "test timeout");
    }
};

#endif // !TESTSOCKETPOOL2_HPP_
