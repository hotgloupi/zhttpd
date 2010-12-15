#ifndef __STATSMANAGER_HPP__
#define __STATSMANAGER_HPP__

#include "utils/Singleton.hpp"
#include "utils/Timer.hpp"
#include "api/types.hpp"
#include "thread/Mutex.hpp"
#include "utils/macros.hpp"

namespace zhttpd
{
    class StatsManager : public Singleton<StatsManager>
    {
        friend class Singleton<StatsManager>;
        private:
            Timer _timer;
            unsigned int _sentBytes;
            unsigned int _receivedBytes;
            unsigned int _requests;
            unsigned int _buffers;
            unsigned int _memory;

        public:
            StatsManager();
            ~StatsManager();
            api::uint32_t getUptime();
            unsigned int getSentBytes();
            void addSentBytes(unsigned int bytes);
            unsigned int getReceivedBytes();
            void addReceivedBytes(unsigned int bytes);
            unsigned int getRequests();
            void addRequest();
            unsigned int getBuffers();
            void addBuffer();
            void removeBuffer();
            unsigned int getMemory();
            void addMemory(int bytes);
    };
}

#endif

