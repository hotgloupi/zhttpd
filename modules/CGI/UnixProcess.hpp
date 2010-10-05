
#ifndef WIN32
# ifndef __UNIXPROCESS_HPP__
#  define __UNIXPROCESS_HPP__

#  include <list>
#  include <string>

#  include "api/IBuffer.hpp"
#  include "api/IRequest.hpp"
#  include "api/IModule.hpp"
#  include "api/types.hpp"

#  include "Process.hpp"
#  include "Environment.hpp"

class   UnixProcess
{
private:
    pid_t                           _pid;
    bool                            _wait;
    mutable PROCESS_STATUS::Type    _status;
    std::string const *             _executable;
    std::list<std::string> const *  _arguments;
    Environment const *             _environment;
    int                             _read_pipe[2];
    int                             _write_pipe[2];

public:
    UnixProcess();
    ~UnixProcess();
    bool create(std::string const & executable,
                std::list<std::string> const& arguments,
                Environment const& environment,
                char const* root_directory);
    ZHTTPD::API::size_t write(char const * buffer, ZHTTPD::API::size_t size);
    ZHTTPD::API::size_t read(char* buffer, ZHTTPD::API::size_t size);
    PROCESS_STATUS::Type getStatus() const;

private:
    bool                            _parentProcessusHandler();
    bool                            _childProcessusHandler();
    void                            _execute();
    char * const *                  _getArguments();

};

typedef UnixProcess ProcessHandler;

# endif // UnixProcess_HPP
#endif  // !WIN32
