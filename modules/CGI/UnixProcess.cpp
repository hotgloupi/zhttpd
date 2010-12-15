
#ifndef WIN32

# include <iostream>
# include <string>
# include <cstring>
# include <algorithm>
# include <stdexcept>

# include <unistd.h>
# include <sys/select.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <cstdio>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>

# include "api/IRequest.hpp"
# include "api/IBuffer.hpp"
# include "api/IBufferManager.hpp"
# include "api/constants.hpp"

# include "server/utils/Logger.hpp"
# include "Environment.hpp"
# include "Process.hpp"
# include "Utils.hpp"

# define READ_END   0
# define WRITE_END  1

UnixProcess::UnixProcess()
{
    this->_read_pipe[0] = -1;
    this->_read_pipe[1] = -1;
    this->_write_pipe[0] = -1;
    this->_write_pipe[1] = -1;
    this->_status = PROCESS_STATUS::STARTED;
    this->_wait = true;
}

UnixProcess::~UnixProcess()
{
    ::close(this->_write_pipe[1]);
    ::close(this->_read_pipe[0]);
}

char* const* UnixProcess::_getArguments()
{
    std::list<std::string>::const_iterator  it;
    std::list<std::string>::const_iterator  ite;
    char**                                  c_argv;
    std::string                             temp;
    zhttpd::api::uint32_t                      i;

    i = 0;
    it = this->_arguments->begin();
    ite = this->_arguments->end();
    c_argv = new char*[this->_arguments->size() + 1];
    for (; it != ite; it++)
    {
        c_argv[i] = new char[it->size() + 1];
        ::strncpy(c_argv[i], it->c_str(), it->size());
        c_argv[i][it->size()] = NULL;
        i++;
    }
    c_argv[i] = NULL;

    return (c_argv);
}

void UnixProcess::_execute()
{
    const char*       executable;
    char* const*      args;
    char* const*      env;

    executable = this->_executable->c_str();
    args = this->_getArguments();
    env = const_cast<char* const*>(this->_environment->getEnvironmentArray());
    ::execve(executable, args, env);
    // never reached !
    this->_status = PROCESS_STATUS::STARTED;
    _exit(EXIT_FAILURE);
}

bool UnixProcess::create(std::string const& executable,
                         std::list<std::string> const& arguments,
                         Environment const& environment,
                         char const* root_directory)
{
    if (::pipe(this->_read_pipe) < 0)
        throw std::runtime_error("pipe() failed: " + std::string(::strerror(errno)));
    if (::pipe(this->_write_pipe) < 0)
        throw std::runtime_error("pipe() failed: " + std::string(::strerror(errno)));
    this->_executable = &executable;
    this->_arguments = &arguments;
    this->_environment = &environment;

    if ((this->_pid = ::fork()) < 0)
        throw std::runtime_error("fork() failed: " + std::string(::strerror(errno)));
    if (this->_pid == 0)
    {
        ::chdir(root_directory);
        _childProcessusHandler();
    }
    else
        _parentProcessusHandler();
    this->_status = PROCESS_STATUS::RUN; // parent et fils ???!!!

    return(true);
}

bool UnixProcess::_childProcessusHandler()
{
    ::close(STDIN_FILENO);
    ::close(this->_write_pipe[WRITE_END]);
    if (::dup2(this->_write_pipe[READ_END], STDIN_FILENO) < 0)
        throw std::runtime_error("Child CGI process dup2() failed: " + std::string(::strerror(errno)));
    //::setvbuf(stdin,(char*)NULL, _IOLBF, 0);

    ::close(STDOUT_FILENO);
    ::close(this->_read_pipe[READ_END]);
    if (::dup2(this->_read_pipe[WRITE_END], STDOUT_FILENO) < 0)
        throw std::runtime_error("Child CGI process: dup2() failed: " + std::string(::strerror(errno)));
    //::setvbuf(stdout,(char*)NULL, _IONBF, 0);

    this->_execute();

    return (false);
}

bool UnixProcess::_parentProcessusHandler()
{
    ::close(this->_write_pipe[READ_END]);
    ::close(this->_read_pipe[WRITE_END]);
    return (false);
}

zhttpd::api::size_t UnixProcess::write(char const* buffer, zhttpd::api::size_t size)
{
    fd_set write_fd_set;
    struct timeval time;
    int nwrite = 0;
    int fd = this->_write_pipe[WRITE_END];

    if (buffer != NULL)
    {
        FD_ZERO(&write_fd_set);
        FD_SET(fd, &write_fd_set);
        time.tv_sec = 0;
        time.tv_usec = 0;
        if (::select(fd + 1, NULL,  &write_fd_set, NULL, &time) < 0)
           throw std::runtime_error("CGI module: select() failed on child read fd: " + std::string(::strerror(errno)));
        if (FD_ISSET(fd, &write_fd_set))
        {
            FD_CLR(fd, &write_fd_set);
            nwrite = ::write(fd, buffer, size);
            if (nwrite == -1)
                throw std::runtime_error("CGI: write() failed: " + std::string(::strerror(errno)));
            return (nwrite);
        }
    }

    return (0);
}

zhttpd::api::size_t UnixProcess::read(char* buffer, zhttpd::api::size_t size)
{
    fd_set read_set;
    struct timeval time;
    int nread = 0;
    int fd = this->_read_pipe[READ_END];

    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);
    time.tv_sec = 0;
    time.tv_usec = 0;
    if (this->_status != PROCESS_STATUS::FINISHED)
    {
        if (::select(fd + 1, &read_set, NULL, NULL, &time) < 0)
            throw std::runtime_error("CGI module: select() failed: " + std::string(::strerror(errno)));
    }
    if (this->_status == PROCESS_STATUS::FINISHED || FD_ISSET(this->_read_pipe[0], &read_set))
    {
        FD_CLR(fd, &read_set);
        nread = ::read(fd, buffer, size);
        if (nread == -1)
            throw std::runtime_error("CGI: read() failed: " + std::string(::strerror(errno)));
        return (nread);
    }
    else
        return (0);
}

PROCESS_STATUS::Type          UnixProcess::getStatus() const
{
    if (this->_status != PROCESS_STATUS::FINISHED)
    {
        int status;
        ::waitpid(this->_pid, &status, WNOHANG);
        if (WIFEXITED(status))
            this->_status = PROCESS_STATUS::FINISHED;
    }
    return (this->_status);
}

#endif  /* !WIN32 */
