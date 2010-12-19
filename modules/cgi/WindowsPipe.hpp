#ifdef WIN32
#ifndef WINDOWSPIPE_HPP
#define WINDOWSPIPE_HPP

#include <Windows.h>

#include "api/types.hpp"

using namespace zhttpd;

class WindowsPipe
{
private:
    HANDLE _read;
    bool _read_closed;
    HANDLE _write;
    bool _write_closed;

public:
    WindowsPipe(bool canRead, bool canWrite);
    ~WindowsPipe();

    HANDLE GetReadPipe() const;
    HANDLE GetWritePipe() const;

    void CloseReadPipe();
    void CloseWritePipe();

    api::size_t read(char* buffer, api::size_t length);
    api::size_t write(char const* buffer, api::size_t length);
};

#endif // WINDOWSPIPE_HPP
#endif // WIN32
