
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
    zhttpd::api::IBuffer* _pendingData;

public:
    WindowsProcess();
    ~WindowsProcess();

    bool create(std::string const & executable, std::list<std::string> const& arguments, Environment const& environment);
    zhttpd::api::size_t write(char const* buffer, zhttpd::api::size_t length);
    zhttpd::api::size_t read(char* buffer, zhttpd::api::size_t length);
    Process::ProcessStatus getStatus() const;
};

typedef WindowsProcess CGIHandler;

# endif // WINDOWSCGI_HPP
#endif  /* WIN32 */
