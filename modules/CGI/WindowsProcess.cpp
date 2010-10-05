
#ifdef  WIN32

#include <stdexcept>
#include <string>

#include "api/constants.hpp"
#include "utils/Logger.hpp"
#include "utils/String.hpp"

#include "Process.hpp"
#include "Environment.hpp"

WindowsProcess::WindowsProcess() :
    _stdin(true, false),
    _stdout(false, true),
    _running(false),
    _pendingData(0)
{
}

WindowsProcess::~WindowsProcess()
{
    if (this->_running)
    {
        this->_stdin.CloseWritePipe();
        this->_stdout.CloseReadPipe();
        if (::GetProcessId(this->_processInfos.hProcess) != 0)
            ::TerminateProcess(this->_processInfos.hProcess, 0);
        CloseHandle(this->_processInfos.hProcess);
    }
}

bool WindowsProcess::create(std::string const & executable, std::list<std::string> const& arguments, Environment const& environment)
{
    char commandLine[MAX_PATH];
    std::string tmp = "";
    std::list<std::string>::const_iterator it = arguments.begin(), ite = arguments.end();
    for (; it != ite; ++it)
        tmp += "\"" + *it + "\" ";
    ::strcpy_s(commandLine, MAX_PATH, tmp.c_str());

    STARTUPINFO startInfo;
    ::memset(&startInfo, 0, sizeof(startInfo));
    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.hStdOutput = this->_stdout.GetWritePipe();
    startInfo.hStdInput = this->_stdin.GetReadPipe();
    startInfo.dwFlags |= STARTF_USESTDHANDLES;

    
    char *real_env = new char[8192];
    const std::map<std::string, std::string>& env = environment.getEnvironmentList();
    std::map<std::string, std::string>::const_iterator itEnv = env.begin();
    std::map<std::string, std::string>::const_iterator iteEnv = env.end();
    int i = 0;
    for (; itEnv != iteEnv; ++itEnv)
    {
        const char *entryKey = itEnv->first.c_str();
        const char *entryValue = itEnv->second.c_str();
        for (; *entryKey; real_env[i++] = *(entryKey++));
        real_env[i++] = '=';
        while (real_env[i++] = *(entryValue++));
    }
    real_env[i] = 0;

    if (!::CreateProcess(NULL, commandLine, NULL, NULL, true, 0, real_env, NULL, &startInfo, &this->_processInfos))
        throw std::runtime_error(std::string("CreateProcess(): fail (") + ::GetLastError() + ")");
    delete real_env;
    CloseHandle(this->_processInfos.hThread);
    this->_stdin.CloseReadPipe();
    this->_stdout.CloseWritePipe();
    this->_running = true;
    return true;
}

ZHTTPD::API::size_t WindowsProcess::write(char const* buffer, ZHTTPD::API::size_t length)
{
    return this->_stdin.write(buffer, length);
}

ZHTTPD::API::size_t WindowsProcess::read(char* buffer, ZHTTPD::API::size_t length)
{
    return this->_stdout.read(buffer, length);
}

Process::ProcessStatus WindowsProcess::getStatus() const
{
    if (this->_running)
    {
        if (::GetProcessId(this->_processInfos.hProcess) == 0)
            return Process::FINISHED;
        return Process::RUN;
    }
    else
        return Process::STARTED;
}

#endif  /* WIN32 */
