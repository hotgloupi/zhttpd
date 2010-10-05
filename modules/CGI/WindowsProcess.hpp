
#ifdef WIN32
# ifndef WINDOWSCGI_HPP
#  define WINDOWSCGI_HPP

#  include <string>

#  include "api/IRequest.hpp"
#  include "api/IBuffer.hpp"
#  include "api/constants.hpp"

#  include "Environment.hpp"
#  include "WindowsPipe.hpp"

class WindowsProcess
{
private:
    WindowsPipe _stdin;
    WindowsPipe _stdout;

    bool _running;
    PROCESS_INFORMATION _processInfos;
    ZHTTPD::API::IBuffer* _pendingData;

public:
    WindowsProcess();
    ~WindowsProcess();

    bool create(std::string const & executable, std::list<std::string> const& arguments, Environment const& environment);
    ZHTTPD::API::size_t write(char const* buffer, ZHTTPD::API::size_t length);
    ZHTTPD::API::size_t read(char* buffer, ZHTTPD::API::size_t length);
    Process::ProcessStatus getStatus() const;
};

typedef WindowsProcess CGIHandler;

# endif // WINDOWSCGI_HPP
#endif  /* WIN32 */
