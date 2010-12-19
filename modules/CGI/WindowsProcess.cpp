
#ifdef  WIN32

#include <stdexcept>
#include <string>
#include <cstdlib>

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

bool WindowsProcess::create(std::string const & executable, std::list<std::string> const& arguments, Environment const& environment, char const* root_directory)
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

    
    int length = 8192;
    char *real_env = static_cast<char *>(::malloc(length * sizeof(char)));
    const std::map<std::string, std::string>& env = environment.getEnvironmentList();
    std::map<std::string, std::string>::const_iterator itEnv = env.begin();
    std::map<std::string, std::string>::const_iterator iteEnv = env.end();
    int i = 0;
    for (; itEnv != iteEnv; ++itEnv)
    {
        const char *entryKey = itEnv->first.c_str();
        const char *entryValue = itEnv->second.c_str();
        int total_len = itEnv->first.length() + itEnv->second.length() + 3;
        if (total_len > length)
            real_env = static_cast<char *>(::realloc(real_env, length + ((total_len % 8192) + 1) * 8192));
        while (real_env[i++] = *(entryKey++));
        real_env[i++] = '=';
        while (real_env[i++] = *(entryValue++));
    }
    real_env[i] = 0;

    if (!::CreateProcess(NULL, commandLine, NULL, NULL, true, 0, real_env, root_directory, &startInfo, &this->_processInfos))
        throw std::runtime_error(std::string("CreateProcess(0, ") + commandLine + ", 0, 0, 1, 0, env, " + root_directory + ", info, info): fail (" + ::GetLastError() + ")");
    ::free(real_env);
    CloseHandle(this->_processInfos.hThread);
    this->_stdin.CloseReadPipe();
    this->_stdout.CloseWritePipe();
    this->_running = true;
    return true;
}

zhttpd::api::size_t WindowsProcess::write(char const* buffer, zhttpd::api::size_t length)
{
    return this->_stdin.write(buffer, length);
}

zhttpd::api::size_t WindowsProcess::read(char* buffer, zhttpd::api::size_t length)
{
    return this->_stdout.read(buffer, length);
}

PROCESS_STATUS::Type WindowsProcess::getStatus() const
{
    if (this->_running)
    {
        DWORD code;
        if (::GetExitCodeProcess(this->_processInfos.hProcess, &code) == 0 || code != STILL_ACTIVE)
            return PROCESS_STATUS::FINISHED;
        return PROCESS_STATUS::RUN;
    }
    else
        return PROCESS_STATUS::STARTED;
}

#endif  /* WIN32 */
