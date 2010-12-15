
#include "utils/macros.hpp"

#ifdef ZHTTPD_DEBUG
# include <iostream>
# include <iomanip>

std::map<std::string, zhttpd::api::uint64_t>* __ZHTTPD_LOCK_times;
zhttpd::Mutex*                                __ZHTTPD_LOCK_times_mutex;
std::map<std::string, zhttpd::api::uint64_t>* __ZHTTPD_MOD_times;
std::map<std::string, zhttpd::api::uint32_t>* __ZHTTPD_MOD_counts;
zhttpd::Mutex* __ZHTTPD_MOD_mutex;
zhttpd::Timer* __ZHTTPD_timer;

void ZHTTPD_DEBUG_init()
{
    __ZHTTPD_LOCK_times = new std::map<std::string, zhttpd::api::uint64_t>();
    __ZHTTPD_LOCK_times_mutex = new zhttpd::Mutex();
    __ZHTTPD_MOD_times = new std::map<std::string, zhttpd::api::uint64_t>();
    __ZHTTPD_MOD_counts = new std::map<std::string, zhttpd::api::uint32_t>();
    __ZHTTPD_MOD_mutex = new zhttpd::Mutex();
    __ZHTTPD_timer = new zhttpd::Timer();
}

void ZHTTPD_DEBUG_print_maps()
{
    std::map<std::string, zhttpd::api::uint64_t>::iterator it;
    std::map<std::string, zhttpd::api::uint64_t>::iterator end;
    zhttpd::api::uint64_t total;

    std::cout << std::endl << "***********************************************************" << std::endl;
    std::cout << "\t* Lock times" << std::endl;
    it = __ZHTTPD_LOCK_times->begin();
    end = __ZHTTPD_LOCK_times->end();
    total = 0;
    for (; it != end; ++it)
    {
        total += it->second;
        std::cout << std::setw(70) << std::right << it->first
            << ((double)it->second) / 1000000.0f << " sec"
            << std::endl;
    }
    std::cout << "\t* TOTAL: " << ((double) total) / 1000000.0f << " sec" << std::endl;

    std::cout << std::endl << "***********************************************************" << std::endl;
    std::cout << "\t* Modules times" << std::endl;
    it = __ZHTTPD_MOD_times->begin();
    end = __ZHTTPD_MOD_times->end();
    total = 0;
    for (; it != end; ++it)
    {
        total += it->second;
        double time = ((double)it->second) / 1000000.0f;
        std::cout << std::setw(70) << std::right << it->first << ": "
                  << (*__ZHTTPD_MOD_counts)[it->first] << " / "
                  << time << " sec";
        if (time > 0.01)
            std::cout << " = " << ((double) (*__ZHTTPD_MOD_counts)[it->first]) / time
                      << " request/sec";
        std::cout << std::endl;
    }
    std::cout << "\t* TOTAL: " << ((double) total) / 1000000.0f << " sec" << std::endl;
}

void ZHTTPD_DEBUG_release()
{
    ZHTTPD_DELETE(__ZHTTPD_LOCK_times);
    ZHTTPD_DELETE(__ZHTTPD_LOCK_times_mutex);
    ZHTTPD_DELETE(__ZHTTPD_MOD_times);
    ZHTTPD_DELETE(__ZHTTPD_MOD_counts);
    ZHTTPD_DELETE(__ZHTTPD_MOD_mutex);
    ZHTTPD_DELETE(__ZHTTPD_timer);
}

#endif
