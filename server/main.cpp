
#include "utils/macros.hpp"
#include "utils/Logger.hpp"
#include "core/Server.hpp"
#include "core/ServerState.hpp"
#include "utils/StatsManager.hpp"

zhttpd::Server* server;

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
    std::cout << "Starting " << ZHTTPD_FULLNAME << " "
#ifdef ZHTTPD_DEBUG
              << "(DEBUG)"
#else
              << "(RELEASE)"
#endif
              << std::endl;
#ifndef ZHTTPD_DEBUG
     zhttpd::Logger::getInstance()->setLevel(zhttpd::Logger::LVL_INFO);
#endif
    server = new zhttpd::Server();
#ifndef WIN32
    (void) signal(SIGINT, sigint);
    (void) signal(SIGPIPE, sigpipe);
#endif

    LOG_INFO("Server is starting");
    zhttpd::StatsManager::getInstance();
    int res = server->run(argc > 1 ? argv[1] : "doc/conf.xml");

#ifdef ZHTTPD_DEBUG
    ZHTTPD_DEBUG_print_maps();
    ZHTTPD_DEBUG_release();
    std::cout << "Uptime: " << ((double) zhttpd::StatsManager::getInstance()->getUptime()) / 1000.0f
              << " sec" << std::endl;
#endif
    delete server;
    server = 0;
    zhttpd::StatsManager::delInstance();
    zhttpd::Logger::delInstance();
    return res;
}
