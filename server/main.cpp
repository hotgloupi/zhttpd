
#include "utils/macros.hpp"
#include "utils/Logger.hpp"
#include "core/Server.hpp"
#include "core/ServerState.hpp"
#include "utils/StatsManager.hpp"

ZHTTPD::Server* server;

#ifndef WIN32
# include <signal.h>
void sigint(int)
{
    if (server != 0)
    {
        LOG_INFO("Stopping server now");
        server->stop();
    }
}

void sigpipe(int)
{
    LOG_WARN("Broken pipe catched");
}
#endif

int main(int argc, char **argv)
{
#ifdef ZHTTPD_DEBUG
    ZHTTPD_DEBUG_init();
#endif

    std::cout << "ZHTTPD Server v0.1 "
#ifdef ZHTTPD_DEBUG
              << "(DEBUG)"
#else
              << "(RELEASE)"
#endif
              << std::endl;

#ifndef ZHTTPD_DEBUG
     ZHTTPD::Logger::getInstance()->setLevel(ZHTTPD::Logger::LVL_INFO);
#endif
    server = new ZHTTPD::Server();
#ifndef WIN32
    (void) signal(SIGINT, sigint);
    (void) signal(SIGPIPE, sigpipe);
    //signal(SIGPIPE, SIG_IGN);
#endif


    LOG_INFO("Server is starting");
    ZHTTPD::StatsManager::getInstance();
    int res = server->run(argc > 1 ? argv[1] : "doc/conf.xml");


#ifdef ZHTTPD_DEBUG
    ZHTTPD_DEBUG_print_maps();
    ZHTTPD_DEBUG_release();
    std::cout << "\t* Memory status: " << std::endl
              << "\t\t- Memory in use: " << ZHTTPD::StatsManager::getInstance()->getMemory() / 1000 << " Kbytes" << std::endl
              << "Up time: " << ((double) ZHTTPD::StatsManager::getInstance()->getUptime()) / 1000.0f
              << " sec" << std::endl;
#endif
    ZHTTPD::Logger::delInstance();
    return res;
}
