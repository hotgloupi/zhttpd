#ifdef WIN32
#include <stdexcept>

#include "utils/Logger.hpp"
#include "utils/String.hpp"
#include "WindowsPipe.hpp"

WindowsPipe::WindowsPipe(bool canRead, bool canWrite) : _read_closed(true), _write_closed(true)
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    if (!::CreatePipe(&this->_read, &this->_write, &saAttr, 0))
        throw std::runtime_error(std::string("CreatePipe() fail (") + ::GetLastError() + ")");
    this->_read_closed = false;
    this->_write_closed = false;
    if (!canRead && !::SetHandleInformation(this->_read, HANDLE_FLAG_INHERIT, 0))
        throw std::runtime_error(std::string("SetHandleInformation() fail (") + ::GetLastError() + ")");
    if (!canWrite && !::SetHandleInformation(this->_write, HANDLE_FLAG_INHERIT, 0))
        throw std::runtime_error(std::string("SetHandleInformation() fail (") + ::GetLastError() + ")");
}

WindowsPipe::~WindowsPipe()
{
    this->CloseReadPipe();
    this->CloseWritePipe();
}

HANDLE WindowsPipe::GetReadPipe() const
{
    return this->_read;
}

HANDLE WindowsPipe::GetWritePipe() const
{
    return this->_write;
}

void WindowsPipe::CloseReadPipe()
{
    if (!this->_read_closed)
        CloseHandle(this->_read);
    this->_read_closed = true;
}

void WindowsPipe::CloseWritePipe()
{
    if (!this->_write_closed)
        CloseHandle(this->_write);
    this->_write_closed = true;
}

API::size_t WindowsPipe::read(char* buffer, API::size_t length)
{
    DWORD bytesRead;
    if (!::ReadFile(this->_read, buffer, length, &bytesRead, 0) && ::GetLastError() != ERROR_BROKEN_PIPE)
        throw std::runtime_error(std::string("ReadFile() fail (") + ::GetLastError() + ")");
    return bytesRead;
}

API::size_t WindowsPipe::write(char const* buffer, API::size_t length)
{
    DWORD bytesWrote;
    if (!::WriteFile(this->_write, buffer, length, &bytesWrote, 0))
        throw std::runtime_error(std::string("WriteFile() fail (") + ::GetLastError() + ")");
    return bytesWrote;
}

#endif // WIN32
